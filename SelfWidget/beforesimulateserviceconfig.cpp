#include "beforesimulateserviceconfig.h"
#include "ui_beforesimulateserviceconfig.h"

#include "mychoosebar.h"
#include "../global.h"
#include "../item/myitem.h"
#include "../modelview/serviceinputtableview.h"
#include "../modelview/serviceinputmodel.h"
#include "../modelview/serviceinputdelegate.h"
#include "../sql/serviceinfoprocess.h"
#include "../util.h"
#include "../simulate/simulateutil.h"

#include <QDebug>
#include <QListView>

BeforeSimulateServiceConfig::BeforeSimulateServiceConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BeforeSimulateServiceConfig)
{
    ui->setupUi(this);

    this->setWindowTitle("��������");

    isComboxAutoChanged = false;
    currServiceIndex = -1;
    int width = 900;
    int height = 700;

    this->setGeometry((ScreenWidth-width)/2,(ScreenHeight-height)/2,width,height);

    chooseBar = new MyChooseBar(ui->widget_3);

    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(layout);

    inputTableView = new ServiceInputTableView(0,3);

    outputTableView = new ServiceInputTableView(0,3);
    outputTableView->delegate()->setColumnState(false);

    QHBoxLayout * inputLayout = new QHBoxLayout;
    inputLayout->addWidget(inputTableView);
    inputLayout->setContentsMargins(2,2,2,2);
    ui->inputWidget->setLayout(inputLayout);

    QHBoxLayout * outputtLayout = new QHBoxLayout;
    outputtLayout->addWidget(outputTableView);
    outputtLayout->setContentsMargins(2,2,2,2);
    ui->outputWidget->setLayout(outputtLayout);

    ui->serviceName->setView(new QListView());
    ui->serviceName->setStyleSheet(COMBOX_STYLESHEET);

    connect(ui->serviceList,SIGNAL(currentRowChanged(int)),this,SLOT(respSwitchService(int)));
    connect(ui->serviceName,SIGNAL(currentIndexChanged(int)),this,SLOT(switchServiceInfo(int)));
    connect(ui->applyService,SIGNAL(clicked()),this,SLOT(applyService()));
}

//�������д���Ԫ���н���
void BeforeSimulateServiceConfig::setProcedureData(QList<ProcessUnit *> &procUnits)
{
    initServiceData();

    foreach(ProcessUnit * unit,procUnits)
    {
        if(unit->ptype == PRO_PROCESS)
        {
            ui->serviceList->addItem(unit->item->getText());
            this->procUnits.append(unit);
        }
    }

    if(this->procUnits.size() > 0)
    {
        currServiceIndex = 0;
        updateDelegateList(SimulateUtil::instance()->getCurrParentItem(this->procUnits.at(0)->item));
        findHasExistedService(this->procUnits.at(0)->item->getServiceProp());
    }
}

//�л�������Ϣ
void BeforeSimulateServiceConfig::respSwitchService(int currIndex)
{
    if(currIndex >=0 && currIndex < procUnits.size())
    {
        currServiceIndex = currIndex;
        updateDelegateList(SimulateUtil::instance()->getCurrParentItem(procUnits.at(currIndex)->item));
        findHasExistedService(procUnits.at(currIndex)->item->getServiceProp());
    }
}

//����ÿؼ��Ѿ����÷�����ֱ�Ӷ�λ�б���ʾ
void BeforeSimulateServiceConfig::findHasExistedService(ServiceProperty * prop)
{
    if(prop->hasSettInfo)
    {
        inputTableView->clearTable();
        outputTableView->clearTable();

        isComboxAutoChanged = true;

        inputTableView->model()->setPara(prop->inputParas);
        foreach(Parameter * para,prop->outputParas)
        {
            para->pValue = "";
        }
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
        isComboxAutoChanged = false;
    }
}

//�л����з��������б�
void BeforeSimulateServiceConfig::switchServiceInfo(int currIndex)
{
    if(!isComboxAutoChanged && currIndex >=0 && currIndex < GlobalServiceProperties.size())
    {
        ServiceProperty * prop = GlobalServiceProperties.at(currIndex);

        inputTableView->clearTable();
        for(int i = 0;i<prop->inputParas.size(); i++)
        {
            inputTableView->insertRow(prop->inputParas.at(i));
        }

        outputTableView->clearTable();
        for(int i = 0;i<prop->outputParas.size(); i++)
        {
            prop->outputParas.at(i)->pValue = "";
            outputTableView->insertRow(prop->outputParas.at(i));
        }
    }
}

//�����ݿ��������,ֻ�ڵ�һ�μ��غ�ˢ�������б�
void BeforeSimulateServiceConfig::initServiceData()
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

//Ӧ�ô˷��񵽵�ǰ�ؼ�
void BeforeSimulateServiceConfig::applyService()
{
    if(currServiceIndex <0 || currServiceIndex > procUnits.size())
    {
        return;
    }

    ServiceProperty * currItemProp = procUnits.at(currServiceIndex)->item->getServiceProp();

    currItemProp->hasSettInfo = true;
    currItemProp->serviceName = ui->serviceName->currentText();

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

//���´��������б�
void BeforeSimulateServiceConfig::updateDelegateList(QList<MyItem *> pItems)
{
    QStringList list;
    int index = 1;

    if(pItems.size() >= 1)
    {
        foreach(MyItem * tmpItem,pItems)
        {
            ServiceProperty * prop = tmpItem->getServiceProp();
            foreach(Parameter * para,prop->outputParas)
            {
                QString newItem = QString(COMBOX_START_FLAG)+QString::number(index)+"]"+tmpItem->getText()+":"+para->pName;
                list<<newItem;
                index++;
            }
        }
    }

    inputTableView->delegate()->updateStringList(list);
    updateLeftQuoteTable(pItems);
}

//��������������б���Ϣ
void BeforeSimulateServiceConfig::updateLeftQuoteTable(QList<MyItem *> & pItems)
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

BeforeSimulateServiceConfig::~BeforeSimulateServiceConfig()
{
    delete chooseBar;
    delete inputTableView;
    delete outputTableView;
    delete ui;
}
