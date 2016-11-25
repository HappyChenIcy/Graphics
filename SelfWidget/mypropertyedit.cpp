#include "mypropertyedit.h"
#include "ui_mypropertyedit.h"

#include "../global.h"
#include "../modelview/serviceinputtableview.h"
#include "../modelview/serviceinputmodel.h"
#include "../modelview/serviceinputdelegate.h"
#include "../sql/serviceinfoprocess.h"
#include "../item/myitem.h"
#include "../util.h"
#include "mychoosebar.h"

#include <QHBoxLayout>
#include <QListView>
#include <QDebug>

MyPropertyEdit::MyPropertyEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPropertyEdit)
{
    ui->setupUi(this);

    setWindowTitle("����༭");

    isComboxAutoChanged = false;

    int width = POP_SIMULATE_DIALOG_WIDTH + 100;

    setGeometry((ScreenWidth - width)/2,(ScreenHeight - POP_SIMULATE_DIALOG_HEIGHT)/2,width,POP_SIMULATE_DIALOG_HEIGHT);

    inputTableView = new ServiceInputTableView(0,3);

    outputTableView = new ServiceInputTableView(0,3);
    outputTableView->delegate()->setColumnState(false);

    ui->serviceName->setView(new QListView());
    ui->quoteTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    chooseBar = new MyChooseBar(ui->widget_2);
    chooseBar->setParetWidget(this);
    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(confirmPropety()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_2->setLayout(layout);

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
}

//��ʼ��ʾ��Ϣ,������ȫ�ֵķ�����Ϣ��������б�Ȼ����ݵ�ǰ�ؼ��Ƿ��Ѿ���ӷ������趨�����б�����
void MyPropertyEdit::initProp(ServiceProperty *prop,bool isEditable)
{
    currItemProp = prop;

    isEditState = isEditable;
    ui->serviceName->setEnabled(isEditable);
    inputTableView->setEnabled(isEditable);
    outputTableView->setEnabled(isEditable);

    initServiceData();

    if(prop->hasSettInfo)
    {
        inputTableView->clearTable();
        outputTableView->clearTable();

        isComboxAutoChanged = true;

        inputTableView->model()->setPara(prop->inputParas);
        outputTableView->model()->setPara(prop->outputParas);


        int index = -1;
        for(int i = 0;i<GlobalServiceProperties.size();i++)
        {
            if(GlobalServiceProperties.at(i)->serviceName == prop->serviceName)
            {
                index = i;
                break;
            }
        }

        ui->serviceName->setCurrentIndex(index);

        ui->serviceState->setText(prop->status);
        ui->serviceDesc->setText(prop->descirption);
        ui->servicePath->setText(prop->servicePath);
        ui->servicePort->setText(prop->method);
        ui->outputAsNextInput->setChecked(prop->isAsNextInput);
    }

    isComboxAutoChanged = false;
}

//�����ݿ��������,ֻ�ڵ�һ�μ��غ�ˢ�������б�
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

//�л���ʾĳ�����Ե���ϸ��Ϣ
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
        for(int i = 0;i<prop->inputParas.size(); i++)
        {
            inputTableView->insertRow(prop->inputParas.at(i));
        }

        outputTableView->clearTable();
        for(int i = 0;i<prop->outputParas.size(); i++)
        {
            outputTableView->insertRow(prop->outputParas.at(i));
        }
    }
}

//�����ݱ�������ǰItem����
void MyPropertyEdit::confirmPropety()
{
    MY_ASSERT(currItemProp)

    if(!isEditState)
    {
        return;
    }

    currItemProp->hasSettInfo = true;
    currItemProp->serviceName = ui->serviceName->currentText();
    currItemProp->status = ui->serviceState->text();
    currItemProp->descirption = ui->serviceDesc->text();
    currItemProp->servicePath = ui->servicePath->text();
    currItemProp->method = ui->servicePort->text();

    foreach(Parameter * para,currItemProp->inputParas)
    {
        delete para;
    }
    currItemProp->inputParas.clear();

    foreach(Parameter * para,currItemProp->outputParas)
    {
        delete para;
    }
    currItemProp->outputParas.clear();

    foreach(Parameter * para,inputTableView->model()->getPara())
    {
        Parameter * pp = new Parameter;
        pp->pId = para->pId;
        pp->pName = para->pName;
        pp->pValue = para->pValue;
        pp->pType = para->pType;
        pp->pRemark = para->pRemark;
        currItemProp->inputParas.append(pp);
    }

    foreach(Parameter * para,outputTableView->model()->getPara())
    {
        Parameter * pp = new Parameter;
        pp->pId = para->pId;
        pp->pName = para->pName;
        pp->pValue = para->pValue;
        pp->pType = para->pType;
        pp->pRemark = para->pRemark;
        currItemProp->outputParas.append(pp);
    }
}

/*!
  ���´��������б���ʾ�������б�
  ��20161125���޸ĳ�ֻ������һ������
*/
void MyPropertyEdit::updateDelegateList(QList<MyItem *> pItems)
{
    QStringList list;
    int index = 1;

    if(pItems.size() >= 1)
    {
        ServiceProperty * prop = pItems.at(0)->getServiceProp();
        foreach(Parameter * para,prop->outputParas)
        {
            QString newItem = QString(COMBOX_START_FLAG)+QString::number(index)+"]"+pItems.at(0)->getText()+":"+para->pName;
            list<<newItem;
            index++;
        }
    }

    inputTableView->delegate()->updateStringList(list);
    updateLeftQuoteTable(pItems);
}

//��������������б���Ϣ
void MyPropertyEdit::updateLeftQuoteTable(QList<MyItem *> & pItems)
{
    Util::removeTableItem(ui->quoteTable);

    if(pItems.size() >= 1)
    {
        ServiceProperty * prop = pItems.at(0)->getServiceProp();
        Util::createTableItem(prop->outputParas.size(),ui->quoteTable);
        for(int i = 0 ;i < prop->outputParas.size() ;i++)
        {
            Parameter * para = prop->outputParas.at(i);
            ui->quoteTable->item(i,0)->setText(para->pName);
            ui->quoteTable->item(i,0)->setToolTip(para->pName);
            ui->quoteTable->item(i,1)->setText(para->pType);
            ui->quoteTable->item(i,1)->setToolTip(para->pType);
        }
    }
    else
    {
        Util::createTableItem(1,ui->quoteTable);
        ui->quoteTable->item(0,0)->setText("��");
        ui->quoteTable->item(0,1)->setToolTip("��");
        ui->quoteTable->item(0,0)->setText("��");
        ui->quoteTable->item(0,1)->setToolTip("��");
    }
}

MyPropertyEdit::~MyPropertyEdit()
{
    delete ui;
}
