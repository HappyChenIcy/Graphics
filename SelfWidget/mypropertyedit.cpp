#include "mypropertyedit.h"
#include "ui_mypropertyedit.h"

#include "../global.h"
#include "../modelview/serviceinputtableview.h"
#include "../modelview/serviceinputmodel.h"
#include "../modelview/serviceinputdelegate.h"
#include "../sql/serviceinfoprocess.h"
#include "../item/myitem.h"
#include "../util.h"

#include <QHBoxLayout>
#include <QListView>
#include <QDebug>

MyPropertyEdit::MyPropertyEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPropertyEdit)
{
    ui->setupUi(this);

    setWindowTitle("服务编辑");

    isComboxAutoChanged = false;

    setGeometry((ScreenWidth - POP_SIMULATE_DIALOG_WIDTH)/2,(ScreenHeight - POP_SIMULATE_DIALOG_HEIGHT)/2,POP_SIMULATE_DIALOG_WIDTH,POP_SIMULATE_DIALOG_HEIGHT);

    inputTableView = new ServiceInputTableView(0,4);

    outputTableView = new ServiceInputTableView(0,4);
    outputTableView->delegate()->setColumnState(false);

    ui->serviceName->setView(new QListView());

    QHBoxLayout * inputLayout = new QHBoxLayout;
    inputLayout->addWidget(inputTableView);
    inputLayout->setContentsMargins(2,2,2,2);
    ui->inputWidget->setLayout(inputLayout);

    QHBoxLayout * outputtLayout = new QHBoxLayout;
    outputtLayout->addWidget(outputTableView);
    outputtLayout->setContentsMargins(2,2,2,2);
    ui->outputWidget->setLayout(outputtLayout);

    currItemProp = NULL;

    connect(ui->serviceName,SIGNAL(currentIndexChanged(int)),this,SLOT(switchServiceInfo(int)));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(confirmPropety()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(cancelProperty()));
}

//初始显示信息
void MyPropertyEdit::initProp(ServiceProperty *prop)
{
    currItemProp = prop;

    if(prop->hasSettInfo)
    {
        inputTableView->model()->setPara(prop->inputParas);
        outputTableView->model()->setPara(prop->outputParas);

        isComboxAutoChanged = true;
        int index = -1;
        for(int i = 0;i<GlobalServiceProperties.size();i++)
        {
            ui->serviceName->addItem(GlobalServiceProperties.at(i)->serviceName);
            if(GlobalServiceProperties.at(i)->serviceName == prop->serviceName)
            {
                index = i;
            }
        }

        ui->serviceName->setCurrentIndex(index);

        isComboxAutoChanged = false;

        ui->serviceState->setText(prop->status);
        ui->serviceDesc->setText(prop->descirption);
        ui->servicePath->setText(prop->servicePath);
        ui->servicePort->setText(prop->method);
        ui->outputAsNextInput->setChecked(prop->isAsNextInput);
    }
    else
    {
        initServiceData();
    }
}

//从数据库加载数据
void MyPropertyEdit::initServiceData()
{
    if(GlobalServiceProperties.size() == 0)
    {
       bool flag = ServiceInfoProcess::instance()->getServiceProperty(GlobalServiceProperties);
       if(!flag)
       {
           Util::showWarn(ServiceInfoProcess::instance()->getLastError());
       }
    }

    foreach(ServiceProperty * prop,GlobalServiceProperties)
    {
        ui->serviceName->addItem(prop->serviceName);
    }
}

//切换显示某个属性的详细信息
void MyPropertyEdit::switchServiceInfo(int index)
{
    if(!isComboxAutoChanged && index >= 0 && index < GlobalServiceProperties.size())
    {
        ServiceProperty * prop = GlobalServiceProperties.at(index);
        ui->serviceState->setText(prop->status);
        ui->serviceDesc->setText(prop->descirption);
        ui->servicePath->setText(prop->servicePath);
        ui->servicePort->setText(prop->method);

        inputTableView->clearTable();
        if(prop->inputParas.size() > 0)
        {
            for(int i = 0;i<prop->inputParas.size(); i++)
            {
                inputTableView->insertRow(prop->inputParas.at(i));
            }
        }

        outputTableView->clearTable();
        if(prop->outputParas.size() > 0)
        {
            outputTableView->insertRow(prop->outputParas.at(0));
        }
    }
}

//将数据保存至当前Item属性
void MyPropertyEdit::confirmPropety()
{
    MY_ASSERT(currItemProp)

    currItemProp->hasSettInfo = true;
    currItemProp->serviceName = ui->serviceName->currentText();
    currItemProp->status = ui->serviceState->text();
    currItemProp->descirption = ui->serviceDesc->text();
    currItemProp->servicePath = ui->servicePath->text();
    currItemProp->method = ui->servicePort->text();
    currItemProp->inputParas = inputTableView->model()->getPara();
    currItemProp->outputParas = outputTableView->model()->getPara();

    cancelProperty();
}

//更新代理下拉列表显示引单的列表
void MyPropertyEdit::updateDelegateList(QList<MyItem *> pItems)
{
    QStringList list;
    int index = 0;
    foreach(MyItem * item,pItems)
    {
        ServiceProperty * prop = item->getServiceProp();
        if(prop->outputParas.size() == 1)
        {
            QString newItem = QString(COMBOX_START_FLAG)+QString::number(index)+"]"+item->getText()+":"+prop->outputParas.at(0)->pName;
            list<<newItem;
            index++;
        }
    }

    inputTableView->delegate()->updateStringList(list);
}

//取消
void MyPropertyEdit::cancelProperty()
{
    this->close();
}

MyPropertyEdit::~MyPropertyEdit()
{
    delete ui;
}
