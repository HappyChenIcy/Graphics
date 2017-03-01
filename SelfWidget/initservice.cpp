#include "initservice.h"
#include "ui_initservice.h"

#include "mychoosebar.h"
#include "initservice.h"
#include "../global.h"
#include "../item/myitem.h"
#include "../modelview/serviceinputtableview.h"
#include "../modelview/serviceinputmodel.h"
#include "../modelview/serviceinputdelegate.h"
#include "../sql/serviceinfoprocess.h"
#include "../util.h"
#include "../simulate/simulateutil.h"
#include "../fileoperate.h"

#include <QFileDialog>

InitService::InitService(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitService)
{
    ui->setupUi(this);

    int width = parent->width();
    int height = parent->height();

    isComboxAutoChanged = false;
    isPreModel = true;
    currServiceIndex = -1;

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

    connect(ui->serviceList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(respSwitchService(QListWidgetItem*)));
    connect(ui->serviceName,SIGNAL(currentIndexChanged(int)),this,SLOT(switchServiceInfo(int)));
    connect(ui->addService,SIGNAL(clicked()),this,SLOT(addCurrService()));
    connect(ui->removeService,SIGNAL(clicked()),this,SLOT(removeCurrService()));
    connect(ui->applyUpdate,SIGNAL(clicked()),this,SLOT(applyUpdateService()));
    connect(ui->saveToDisk,SIGNAL(clicked()),this,SLOT(saveToDisk()));
    connect(ui->loadFromDisk,SIGNAL(clicked()),this,SLOT(loadFromDisk()));
}

//����ǰ�������ñ��������ش���
void InitService::saveToDisk()
{
    QString openFileName = QFileDialog::getSaveFileName(this,"ѡ��·��");
    if(!openFileName.isEmpty())
    {
        ReturnType type = FileOperate::instance()->saveServiceToFile(openFileName,isPreModel);
        if(type == RETURN_SUCCESS)
        {
            QMessageBox::information(this,"��ʾ","������Ϣ����ɹ�!");
        }
    }
}

//�ӱ��ش��̼���
void InitService::loadFromDisk()
{
    int result = QMessageBox::Yes;

    if(ui->serviceList->count()>0)
    {
        result = QMessageBox::warning(this,"����","�˲�������յ�ǰ�Ѿ���ӵķ���,�Ƿ����?",QMessageBox::Yes|QMessageBox::No);
    }

    if(result == QMessageBox::Yes)
    {
        if(isPreModel)
        {
            for(int i=0;i<PreExeServices.size();i++)
            {
                delete PreExeServices.at(i);
            }
            PreExeServices.clear();
        }
        else
        {
            for(int i=0;i<ResetExeServices.size();i++)
            {
                delete ResetExeServices.at(i);
            }
            ResetExeServices.clear();
        }
        ui->serviceList->clear();

        QString openFileName = QFileDialog::getOpenFileName(this,"ѡ����ļ�","","Files(*"+SaveFileSuffix+")");
        if(!openFileName.isEmpty())
        {
            FileOperate::instance()->loadServiceFromFile(openFileName,isPreModel);
            if(isPreModel)
            {
                for(int i = 0;i<PreExeServices.size();i++)
                {
                    ui->serviceList->addItem(PreExeServices.at(i)->serviceName);
                }
            }
            else
            {
                for(int i = 0;i<ResetExeServices.size();i++)
                {
                    ui->serviceList->addItem(ResetExeServices.at(i)->serviceName);
                }
            }
        }
    }
}

//ִ��Ԥ�������
void InitService::preService()
{
    initServiceData();
    foreach(ServiceProperty * prop,PreExeServices)
    {
        ui->serviceList->addItem(prop->serviceName);
    }
}

//ִ�����÷���
void InitService::resetService()
{
    isPreModel = false;
    initServiceData();
    foreach(ServiceProperty * prop,ResetExeServices)
    {
        ui->serviceList->addItem(prop->serviceName);
    }
}

//���˷�������б�
void InitService::addCurrService()
{
    ServiceProperty * currItemProp = new ServiceProperty;

    currItemProp->hasSettInfo = true;
    currItemProp->serviceName = ui->serviceName->currentText();

    //�������ӵ�ַ
    for(int i=0;i<GlobalServiceProperties.size();i++)
    {
        if(GlobalServiceProperties.at(i)->serviceName==currItemProp->serviceName)
        {
            currItemProp->servicePath = GlobalServiceProperties.at(i)->servicePath;
            currItemProp->method = GlobalServiceProperties.at(i)->method;
            break;
        }
    }

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

    ui->serviceList->addItem(currItemProp->serviceName);

    setButtState(false);
    ui->saveToDisk->setEnabled(true);

    if(isPreModel)
    {
        PreExeServices.append(currItemProp);
    }
    else
    {
        ResetExeServices.append(currItemProp);
    }
}

//�Ƴ�����
void InitService::removeCurrService()
{
    QString currServiceName = ui->serviceName->currentText();
    if(currServiceName.size() <= 0)
    {
        return;
    }
    if(isPreModel)
    {
        int index = -1;
        for(int i = 0;i<PreExeServices.size();i++)
        {
            if(PreExeServices.at(i)->serviceName == currServiceName)
            {
                index = i;
                break;
            }
        }
        if(index >= 0 && index < PreExeServices.size())
        {
            delete PreExeServices.at(index);
            PreExeServices.removeAt(index);
            QListWidgetItem * item = ui->serviceList->takeItem(index);
            delete item;
        }
    }
    else
    {
        int index = -1;
        for(int i = 0;i<ResetExeServices.size();i++)
        {
            if(ResetExeServices.at(i)->serviceName == currServiceName)
            {
                index = i;
                break;
            }
        }
        if(index >= 0 && index < ResetExeServices.size())
        {
            delete ResetExeServices.at(index);
            ResetExeServices.removeAt(index);
            QListWidgetItem * item = ui->serviceList->takeItem(index);
            delete item;
        }
    }

    setButtState(true);

    if(ui->serviceList->count()<=0)
    {
        ui->saveToDisk->setEnabled(false);
    }
}

//Ӧ�ø���
void InitService::applyUpdateService()
{
    if(currServiceIndex <0 || currServiceIndex > ui->serviceList->count())
    {
        return;
    }
    ServiceProperty * prop = NULL;
    if(isPreModel)
    {
        prop = PreExeServices.at(currServiceIndex);
    }
    else
    {
        prop = ResetExeServices.at(currServiceIndex);
    }

    foreach(Parameter * para,prop->inputParas)
    {
        delete para;
    }
    prop->inputParas.clear();

    foreach(Parameter * para,inputTableView->model()->getPara())
    {
        Parameter * pp = new Parameter;
        pp->pId = para->pId;
        pp->pName = para->pName;
        pp->pValue = para->pValue;
        pp->pType = para->pType;
        pp->pRemark = para->pRemark;
        prop->inputParas.append(pp);
    }
}

//�����ݿ��������,ֻ�ڵ�һ�μ��غ�ˢ�������б�
void InitService::initServiceData()
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

//����л�������Ϣ
void InitService::respSwitchService(QListWidgetItem* item)
{
    ServiceProperty * prop = NULL;
    if(isPreModel)
    {
        for(int i = 0;i<ui->serviceList->count();i++)
        {
            if(item->text() == ui->serviceList->item(i)->text())
            {
                currServiceIndex = i;
                prop = PreExeServices.at(i);
                break;
            }
        }
    }
    else
    {
        for(int i = 0;i<ui->serviceList->count();i++)
        {
            if(item->text() == ui->serviceList->item(i)->text())
            {
                currServiceIndex = i;
                prop = ResetExeServices.at(i);
                break;
            }
        }
    }

    if(currServiceIndex != -1)
    {
        QString serviceName = item->text();
        for(int i = 0;i<GlobalServiceProperties.size(); i++)
        {
            if(GlobalServiceProperties.at(i)->serviceName == serviceName)
            {
                showSelectedItemInfo(prop);
                setButtState(false);
                break;
            }
        }
    }
}

//���������б���ʾ��Ӧ��Ϣ
void InitService::showSelectedItemInfo(ServiceProperty * prop)
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
    isComboxAutoChanged = false;
}

//���´��������б�
void InitService::updateDelegateList(QList<MyItem *> pItems)
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
}

//�л����з��������б�
void InitService::switchServiceInfo(int currIndex)
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

        if(findHasExistedService(ui->serviceName->currentText()))
        {
            setButtState(false);
        }
        else
        {
            setButtState(true);
        }
    }
}

//�����Ƿ��Ѵ��ڷ���
bool InitService::findHasExistedService(QString sname)
{
    if(isPreModel)
    {
        foreach(ServiceProperty * prop,PreExeServices)
        {
            if(prop->serviceName == sname)
            {
                return true;
            }
        }
    }
    else
    {
        foreach(ServiceProperty * prop,ResetExeServices)
        {
            if(prop->serviceName == sname)
            {
                return true;
            }
        }
    }

    return false;
}

//���ð�ť��״̬
void InitService::setButtState(bool isAddEnabled)
{
    ui->addService->setEnabled(isAddEnabled);
    ui->removeService->setEnabled(!isAddEnabled);
    ui->applyUpdate->setEnabled(!isAddEnabled);
}

InitService::~InitService()
{
    delete chooseBar;
    delete inputTableView;
    delete outputTableView;
    delete ui;
}
