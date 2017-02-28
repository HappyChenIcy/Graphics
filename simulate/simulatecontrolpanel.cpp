#include "simulatecontrolpanel.h"
#include "ui_simulatecontrolpanel.h"

#include <QDebug>
#include <QApplication>
#include <QMessageBox>

#include "SimulateHeader.h"
#include "sceneitempickup.h"
#include "processcheck.h"
#include "simulateutil.h"
#include "./mainwindow.h"

#include "../item/myitem.h"
#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../util.h"
#include "../webservice/mywebservice.h"
#include "../SelfWidget/beforesimulateserviceconfig.h"
#include "../sql/serviceinfoprocess.h"

MyListWidgetItem::MyListWidgetItem(QListWidget *parent, int type)
    :QListWidgetItem(parent,type)
{
    punit = NULL;
}

MyListWidgetItem::MyListWidgetItem(const QString &text, QListWidget *parent, int type)
    :QListWidgetItem(text,parent,type)
{
    punit = NULL;
}

//�󶨴���Ԫ�����ڷ���Ŀ���
void MyListWidgetItem::bindProscessUnit(ProcessUnit *unit)
{
    this->punit = unit;
}

SimulateControlPanel::SimulateControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulateControlPanel)
{
    ui->setupUi(this);

    setFixedWidth(300);
    currProcUnit = NULL;
    beforeUnit = NULL;
    isSimulateState = false;
    isAutoRun = true;
    sflow = PRE_FLOW;
    preOrResetIndex = 0;

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
    connect(ui->simProcedure,SIGNAL(currentItemChanged(QListWidgetItem * , QListWidgetItem *)),this,SLOT(respItemChanged(QListWidgetItem * , QListWidgetItem *)));
    connect(ui->simProcedure,SIGNAL(itemClicked (QListWidgetItem*)),this,SLOT(respItemActivated(QListWidgetItem*)));
    connect(ui->simProcedure,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(respItemDoubleClicked(QListWidgetItem *)));
    connect(ui->terminalSimulate,SIGNAL(clicked()),this,SLOT(stopCurrSimulate()));

    connect(ui->autoRun,SIGNAL(toggled(bool)),this,SLOT(chooseRunMethod(bool)));
    connect(ui->signalRun,SIGNAL(toggled(bool)),this,SLOT(chooseRunMethod(bool)));
    connect(ui->nextStep,SIGNAL(clicked()),this,SLOT(stepByStep()));

    ui->autoRun->setChecked(true);
    ui->terminalSimulate->setEnabled(false);

    connect(MyWebService::instance(),SIGNAL(lastUnitProcessOver(bool,QMap<QString,QString>)),this,SLOT(procLastUnitResult(bool,QMap<QString,QString>)));

    /*************��ɾ***************/
//    intSimulateData();
}

//��ʼ��ģ��ȫ�����ݡ���ɾ��
void SimulateControlPanel::intSimulateData()
{
    ServiceProperty * prop1 = new ServiceProperty;
    prop1->hasSettInfo = true;
    prop1->serviceName = "�µ���ģ���ʼ��";

    QString path1;
//    if(ServiceInfoProcess::instance()->getUrlByServiceName(prop1->serviceName,path1))
//    {
//        prop1->servicePath = path1;
//    }
//    else
//    {
//        prop1->servicePath = "http://localhost:8080/axis2/services/BluePlane";
//    }

    prop1->servicePath = "http://127.0.0.1:8080/axis2/services/missile";

    prop1->method = "initial";

    Parameter * para1 = new Parameter;
    para1->pName = "startplaceJ";
    para1->pValue = "108.54";
    para1->pType = "double";

    Parameter * para2 = new Parameter;
    para2->pName = "startplaceW";
    para2->pValue = "34.16";
    para2->pType = "double";

    Parameter * para3 = new Parameter;
    para3->pName = "destinationJ";
    para3->pValue = "115.25";
    para3->pType = "double";

    Parameter * para4 = new Parameter;
    para4->pName = "destinationW";
    para4->pValue = "39.26";
    para4->pType = "double";

    Parameter * para5 = new Parameter;
    para5->pName = "num";
    para5->pValue = "5";
    para5->pType = "int";

    prop1->inputParas.append(para1);
    prop1->inputParas.append(para2);
    prop1->inputParas.append(para3);
    prop1->inputParas.append(para4);
    prop1->inputParas.append(para5);

    PreExeServices.append(prop1);

    ServiceProperty * prop2 = new ServiceProperty;
    prop2->hasSettInfo = true;
    prop2->serviceName = "���״��ʼ��";
    QString path2;

//    if(ServiceInfoProcess::instance()->getUrlByServiceName(prop2->serviceName,path2))
//    {
//        prop2->servicePath = path2;
//    }
//    else
//    {
//        prop2->servicePath = "http://localhost:8080/axis2/services/DetectModel";
//    }

    prop2->servicePath = "http://127.0.0.1:8080/axis2/services/radar";

    prop2->method = "initial";

    Parameter * para11 = new Parameter;
    para11->pName = "placeJ";
    para11->pValue = "115.25";
    para11->pType = "double";

    Parameter * para12 = new Parameter;
    para12->pName = "placeW";
    para12->pValue = "39.26";
    para12->pType = "double";

    Parameter * para13 = new Parameter;
    para13->pName = "distance";
    para13->pValue = "500";
    para13->pType = "double";

    prop2->inputParas.append(para11);
    prop2->inputParas.append(para12);
    prop2->inputParas.append(para13);

    PreExeServices.append(prop2);

    ServiceProperty * prop3 = new ServiceProperty;
    prop3->hasSettInfo = true;
    prop3->serviceName = "������ģ���ʼ��";

    prop3->servicePath = "http://127.0.0.1:8080/axis2/services/defendMissile";

    prop3->method = "initial";

    Parameter * para21 = new Parameter;
    para21->pName = "startplaceJ";
    para21->pValue = "115.25";
    para21->pType = "double";

    Parameter * para22 = new Parameter;
    para22->pName = "startplaceW";
    para22->pValue = "39.26";
    para22->pType = "double";

    Parameter * para23 = new Parameter;
    para23->pName = "ratio";
    para23->pValue = "0.9";
    para23->pType = "double";

    prop3->inputParas.append(para21);
    prop3->inputParas.append(para22);
    prop3->inputParas.append(para23);

    PreExeServices.append(prop3);
}

//�Զ�����
void SimulateControlPanel::chooseRunMethod(bool flag)
{
    QString objName = QObject::sender()->objectName();

    if(objName == "autoRun")
    {
        if(flag)
        {
            isAutoRun = true;
            ui->nextStep->setEnabled(false);
        }
    }
    else if(objName == "signalRun")
    {
        if(flag)
        {
            isAutoRun = false;
            ui->nextStep->setEnabled(true);
        }
    }
}

//��һ��
void SimulateControlPanel::stepByStep()
{
    if(isSimulateState)
    {
        startProcUnit();
    }
}

/*!
 *��ʼ��������
 *��1��:��ȡ��ǰ�����пؼ�������
 *��2��:�ж��Ƿ������ʼ�ͽ���
 *��3��:�ж�������·�Ƿ�������ͬʱ�ж��߼��Ƿ���ȷ�����ҽ��ؼ������߼�˳���������С���Ҫ��
 *��4��:��ͷ��ʼ��ȡ�ؼ�������ת���ɴ���Ԫ��ѹ�봦����С���Ҫ��
 *��5��:�Զ��н��д���ͬʱ�Ե�ǰ����Ŀؼ����и�����ʶ����Ҫ��
 *!*/
void SimulateControlPanel::respStartSimulate()
{
    setSimulateState(true);

    emit resetSimluate();
    resetSimluateFlag();

    QList<QGraphicsItem *> existedItems;
    //��1����2��
    ReturnType returnType = SceneItemPickup::instance()->pickUpItems(existedItems);

    if(returnType != RETURN_SUCCESS)
    {
        setFlagState(ui->step1,false);
        setSimulateState(false);
        return;
    }
    else
    {
        setFlagState(ui->step1,true);
    }

    //��3����4��
    QList<MyItem *> resortedItems;

    returnType = ProcessCheck::instance()->checkProcess(existedItems,resortedItems,procUnits);

    if(returnType != RETURN_SUCCESS)
    {
        setFlagState(ui->step2,false);
        setSimulateState(false);
        return;
    }
    else
    {
        setFlagState(ui->step2,true);
        setFlagState(ui->step3,true);
    }

    qDebug()<<"============================";

    foreach(ProcessUnit * tmpUnit,procUnits)
    {
        tmpUnit->item->setProcessType(tmpUnit->ptype);

        if(tmpUnit->gtype == GRA_POLYGON)
        {
            qDebug()<<tmpUnit->ptype<<"_"<<tmpUnit->item->getText()<<"__"<<tmpUnit->yesChild->item->getText()<<"_"<<tmpUnit->noChild->item->getText();
        }
        else
        {
            if(tmpUnit->nextChild)
            {
                qDebug()<<tmpUnit->item->getText()<<"__"<<tmpUnit->nextChild->item->getText();
            }
            else
            {
                qDebug()<<tmpUnit->item->getText();
            }
        }
    }

    clearLastSimluteRecord();

    //������Ϣ�������
    BeforeSimulateServiceConfig bsconfig(GlobalMainWindow);
    bsconfig.setProcedureData(procUnits);
    bsconfig.exec();

    isSimulateState = true;

    sflow = PRE_FLOW;
    preOrResetIndex = 0;
    preExecuteServices();
}

//�����һ�����ݵļ�¼
void SimulateControlPanel::clearLastSimluteRecord()
{
    foreach(ProcessUnit * unit,procUnits)
    {
        if(unit->ptype == PRO_LOOP)
        {
            LoopProperty * lprop = unit->item->getLoopProp();
            foreach(SignalVari  * tmp ,lprop->signalList)
            {
                tmp->isAssignedValue = false;
                tmp->middlValue = tmp->initialValue;
                tmp->middleResults.clear();
            }
        }
    }
}

//��Ե�ǰ����Ԫ���صĽ�����д���
void SimulateControlPanel::procLastUnitResult(bool hasFault,QMap<QString,QString> result)
{
    if(sflow == PRE_FLOW)
    {
        preExecuteServices();
    }
    else if(sflow == RESET_FLOW)
    {
        resetExecuteServices();
    }
    else
    {
        if(currProcUnit && currProcUnit->item->getServiceProp()->id.size() > 0)
        {
            ServiceInfoProcess::instance()->setServiceStatus(currProcUnit->item->getServiceProp()->id,true);
        }

        if(hasFault && currProcUnit)
        {
            currProcUnit->item->hightLightItem(LEVEL_MIDDLE,true);
            Util::showWarn("������ʳ�������������ֹ!");
            setSimulateState(false);
            return;
        }

        if(currProcUnit && currProcUnit->ptype == PRO_PROCESS)
        {
            ServiceProperty * prop = currProcUnit->item->getServiceProp();

            foreach(Parameter * para,prop->outputParas)
            {
                para->pValue = result.value(para->pName);
            }

            if(!isAutoRun)
            {
                showCurrProcessResultPanel(false);
            }

            currProcUnit = currProcUnit->nextChild;
        }

        if(isAutoRun)
        {
            startProcUnit();
        }
    }
}

//��ʼ����Ԫ
void SimulateControlPanel::startProcUnit()
{
    while(isSimulateState)
    {
        //���Ҳ�ģ������м��뵱ǰ���ݿؼ�
        MyListWidgetItem * currListItem = showSimulateOperate(currProcUnit);

        if(currProcUnit->ptype == PRO_START)
        {
            currProcUnit = currProcUnit->nextChild;
        }
        //�����
        else if(currProcUnit->ptype == PRO_PROCESS)
        {
            ServiceProperty * prop = currProcUnit->item->getServiceProp();
            ServiceInfoProcess::instance()->setServiceStatus(prop->id,false);
            submitUrl(currProcUnit->item,prop);
            break;
        }
        //�жϿ�
        else if(currProcUnit->ptype == PRO_JUDGE)
        {
            JudgeProperty * jprop = currProcUnit->item->getJudegeProp();

            bool judgeResult = countJudgeValue(currProcUnit->item,jprop->express);
            if(judgeResult)
            {
                currProcUnit = currProcUnit->yesChild;
            }
            else
            {
                currProcUnit = currProcUnit->noChild;
            }
        }
        //ѭ����
        else if(currProcUnit->ptype == PRO_LOOP)
        {
            int loopRecord = 0;
            bool isExisted = false;
            foreach(LoopRecord  * lr,loopRecords)
            {
                if(lr->item == currProcUnit->item)
                {
                    lr->record+=1;
                    loopRecord = lr->record;
                    isExisted = true;
                    break;
                }
            }

            if(!isExisted)
            {
                LoopRecord  * lr = new LoopRecord;
                lr->item = currProcUnit->item;
                lr->record = 0;
                loopRecord = lr->record;
                loopRecords.push_back(lr);
            }
            if(currListItem)
            {
                currListItem->setData(Qt::UserRole,loopRecord);
            }

            LoopProperty * loopProp = currProcUnit->item->getLoopProp();
            SignalVariList slist = loopProp->signalList;

            bool loopResult = countLoopValue(currProcUnit->item,slist);

            if(loopResult)
            {
                currProcUnit = currProcUnit->yesChild;
            }
            else
            {
                //��ִ�з�ʱ����Ҫ���ϴ�ѭ�����м�ֵ��Ϊ0(Ƕ��ѭ��)
                foreach(SignalVari * tmpVari,slist)
                {
                    tmpVari->middlValue = tmpVari->initialValue;
                }
                int index = 0;
                for(int i = 0; i < loopRecords.size(); i++)
                {
                    if(loopRecords.at(i)->item == currProcUnit->item)
                    {
                        index = i;
                        delete loopRecords.at(i);
                        break;
                    }
                }
                loopRecords.removeAt(index);

                currProcUnit = currProcUnit->noChild;
            }
        }
        //ƽ���ı������������
        else if(currProcUnit->ptype == PRO_INPUT)
        {
            JudgeProperty * jprop = currProcUnit->item->getJudegeProp();
            QString switchResult = switchQuoteParameter(currProcUnit->item,jprop->express);
            currProcUnit->item->getJudegeProp()->switchExpress = switchResult;

            if(!isAutoRun)
            {
                showCurrProcessResultPanel(false);
            }

            currProcUnit = currProcUnit->nextChild;
        }
        //����
        else if(currProcUnit->ptype == PRO_END)
        {
            sflow = RESET_FLOW;
            resetExecuteServices();
            break;
        }

        if(!isAutoRun)
        {
            break;
        }
    }
}

/*!����ѭ�������趨��ֵ�����㵱ǰ�����Ƿ����㣻�����㣬ִ��ѭ����yes��֧������ִ��no��֧
 *20161117:���ѭ��������ǰ����������Ľ��ֵ
 *         ��Ӷ�ѭ�������ļ�¼�����䱣������ע��Ϣ��
 **/
bool SimulateControlPanel::countLoopValue(MyItem * item, SignalVariList &loopList)
{
    if(loopList.size() == 0)
    {
        return false;
    }

    bool finalResult = false;

    //�ж�ѭ���������Ƿ�������ã������������Ҫ�����õ�ֵ���л�ȡ
    foreach(SignalVari * sv,loopList)
    {
//        if(sv->isQuoted && !sv->isAssignedValue)
//        {
            QList<MyItem *> pItems = SimulateUtil::instance()->getCurrParentItem(item);
            int index = 1;
            foreach(MyItem * tmpItem,pItems)
            {
                ServiceProperty * prop = tmpItem->getServiceProp();

                foreach(Parameter * outPara,prop->outputParas)
                {
                    QString newItem = QString(COMBOX_START_FLAG)+QString::number(index)+"]"+tmpItem->getText()+":"+outPara->pName;
                    index++;
                    if(newItem == sv->variName)
                    {
                        sv->initialValue = outPara->pValue.toInt();
                        sv->middlValue = sv->initialValue;
                        sv->isAssignedValue = true;
                        break;
                    }
                }
//            }
        }
    }

    //�ж������Ƿ����
    foreach(SignalVari * sv,loopList)
    {
        if(sv->operateSymbol == "<")
        {
            if(sv->middlValue < sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == "<=")
        {
            if(sv->middlValue <= sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == ">")
        {
            if(sv->middlValue > sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == ">=")
        {
            if(sv->middlValue >= sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == "==")
        {
            if(sv->middlValue == sv->finalValue)
            {

                finalResult = true;
            }
        }
    }

    //��¼ÿ��ѭ�������в����������м�ֵ
    foreach(SignalVari * sv,loopList)
    {
        sv->middleResults.append(sv->middlValue);
    }

    //�����������Ա������иı����
    if(finalResult)
    {
        foreach(SignalVari * sv,loopList)
        {
            int middval = sv->middlValue;

            if(sv->selfOperateSymbol == "++")
            {
                sv->middlValue = middval+1;
            }
            else if(sv->selfOperateSymbol == "--")
            {
                sv->middlValue = middval-1;
            }
            else if(sv->selfOperateSymbol == "+=")
            {
                sv->middlValue = middval + sv->selfOperateValue;
            }
            else if(sv->selfOperateSymbol == "-=")
            {
                sv->middlValue = middval - sv->selfOperateValue;
            }
            else if(sv->selfOperateSymbol == QString(LOOP_UNTIL_EXPRESS_ILLEGAL))
            {
            }
        }
    }

    return finalResult;
}

//��װURLֵ
void SimulateControlPanel::submitUrl(MyItem * item, ServiceProperty * prop)
{
    QString fullUrl = prop->servicePath+"/"+prop->method;

    if(prop->inputParas.size() > 0)
    {
        fullUrl += "?";
        for(int i = 0;i<prop->inputParas.size();i++)
        {
            QString paraValue = prop->inputParas.at(i)->pValue;
            QString realParaValue;
            if(item && paraValue.startsWith(QString(COMBOX_START_FLAG)))
            {
                realParaValue = getQuoteOutValue(item,paraValue);
            }
            else
            {
                realParaValue = prop->inputParas.at(i)->pValue;
            }

            fullUrl += (prop->inputParas.at(i)->pName+"="+ realParaValue);

            if(i != prop->inputParas.size() - 1)
            {
                fullUrl += "&";
            }
        }
    }

//    qDebug() <<__FUNCTION__
//            <<fullUrl
//            <<"\n";

    MyWebService::instance()->submit(fullUrl);
}

//�������õĲ���������������õ����ֵ
QString SimulateControlPanel::getQuoteOutValue(MyItem * item,QString value)
{
   QList<MyItem *> parentItems =  SimulateUtil::instance()->getCurrParentItem(item);
   int pos = value.indexOf("]");
   int colonPos = value.indexOf(":");

   QString quoteParentText = value.mid(pos + 1,colonPos - pos - 1);
   QString quoteParaText = value.right(value.size() - colonPos -1);

   for(int i = 0; i < parentItems.size() ; i++)
   {
       if(parentItems.at(i)->getText() == quoteParentText)
       {
           ServiceProperty * prop = parentItems.at(i)->getServiceProp();
           for(int j = 0; j < prop->outputParas.size(); j++)
           {
               if(prop->outputParas.at(j)->pName == quoteParaText)
               {
                   return prop->outputParas.at(j)->pValue;
               }
           }
       }
   }

   return "0";
}

//��ֹ��ǰ������
void SimulateControlPanel::stopCurrSimulate()
{
    isSimulateState = false;
    setSimulateState(false);
}

//�����Ƿ�Ϊ����״̬���ÿؼ���״̬
void SimulateControlPanel::setSimulateState(bool isSim)
{
    isSimulateState = isSim;
    GlobalIsSimulateState = isSim;
    ui->startSimulate->setEnabled(!isSim);
    ui->terminalSimulate->setEnabled(isSim);
    beforeUnit = NULL;

    foreach(LoopRecord * lr,loopRecords)
    {
        delete lr;
    }
    loopRecords.clear();

    if(isSim)
    {
        ui->autoRun->setEnabled(false);
        ui->signalRun->setEnabled(false);
    }
    else
    {
        if(isAutoRun)
        {
            ui->autoRun->setEnabled(true);
            ui->signalRun->setEnabled(true);
            ui->nextStep->setEnabled(false);
        }
        else
        {
            ui->autoRun->setEnabled(true);
            ui->signalRun->setEnabled(true);
            ui->nextStep->setEnabled(true);
        }
    }
}

//��ʾÿһ�������ݵĲ�����Ϣ
MyListWidgetItem * SimulateControlPanel::showSimulateOperate(ProcessUnit *unit)
{
    MY_ASSERT_RETURN_NULL(unit)
    QPixmap pixmap;

    if(beforeUnit)
    {
       beforeUnit->item->hightLightItem(LEVEL_NORMAL,true);
    }
    unit->item->hightLightItem(LEVEL_HIGH,true);

    beforeUnit = unit;

    Util::loadPixmapByGType(unit->gtype,pixmap);

    MyListWidgetItem * item = new MyListWidgetItem;
    item->setIcon(QIcon(pixmap));
    item->setText(unit->item->getText());
    item->bindProscessUnit(unit);

    ui->simProcedure->addItem(item);
    ui->simProcedure->scrollToBottom();

    return item;
}

//����ؼ�ʱ��������ʾ
void SimulateControlPanel::respItemActivated(QListWidgetItem * current)
{
    MY_ASSERT(current)

    MyListWidgetItem * citem = dynamic_cast<MyListWidgetItem *>(current);
    if(citem)
    {
        citem->getUnit()->item->hightLightItem(LEVEL_HIGH,true);

        SimulateUtil::instance()->getCurrParentItem(citem->getUnit()->item);
    }
}

//����ѡ���item��������ƿؼ�
void SimulateControlPanel::respItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(current)
    MY_ASSERT(previous)

    MyListWidgetItem * pitem = dynamic_cast<MyListWidgetItem *>(previous);

    if(pitem)
    {
        pitem->getUnit()->item->hightLightItem(LEVEL_NORMAL,true);
    }

    MyGraphicsView::instance()->update();
}

//˫���Ҳ���б����Բ鿴��ǰ������
void SimulateControlPanel::respItemDoubleClicked(QListWidgetItem *current)
{
    MY_ASSERT(current)
    MyListWidgetItem * citem = dynamic_cast<MyListWidgetItem *>(current);

    if(citem)
    {
        MyGraphicsView::instance()->showSelectedItemPropEdit(citem->getUnit()->item,citem->data(Qt::UserRole),true);
    }
}

//��������������֤��ʾ
void SimulateControlPanel::resetSimluateFlag()
{
    ui->step1->setStyleSheet("");
    ui->step2->setStyleSheet("");
    ui->step3->setStyleSheet("");

    ui->simProcedure->clear();

    foreach(ProcessUnit * unit,procUnits)
    {
        delete unit;
    }
    procUnits.clear();
}

//���ݼ��Ľ��������������ʽ
void SimulateControlPanel::setFlagState(QLabel * label,bool isSuccess)
{
    if(isSuccess)
    {
        label->setStyleSheet("background-color:green;color:white;border-radius:4px;");
    }
    else
    {
        label->setStyleSheet("background-color:red;color:white;border-radius:4px;");
    }
}

/*!����߼����ʽ��ֵ,Ŀǰ��֧�ֱ��ʽ����÷���
  ��1���Ա��ʽ�г��ֵı����������ý����滻
  ��2���Ա��ʽ��ȷ����֤(��δ����)
  ��3����������ַ����дʷ���������һ���ַ���������������ȡ
  ��4���Ա��ʽ��������߼��������ؼ���Ľ��
!*/
bool SimulateControlPanel::countJudgeValue(MyItem * item, QString express)
{
    //��1��
    QString switchResult = switchQuoteParameter(item,express);
    item->getJudegeProp()->switchExpress = switchResult;

    //��2������

    //��3��
    QStringList wordList ;
    bool flag = SimulateUtil::instance()->parseText(switchResult,wordList);
    if(flag)
    {
        //��4��
        return SimulateUtil::instance()->getResult(wordList);
    }
    else
    {
        Util::showWarn("�ж������������������ж�����!");
        return false;
    }
    return false;
}

//�Ա��ʽ�е����ò��������滻��������Ҫʹ��"\\["
QString SimulateControlPanel::switchQuoteParameter(MyItem * item,QString & express)
{
    QString finalResult = express;
    QRegExp exp("(\\[([a-zA-Z]+):(\\w+):(\\w+)\\])");

    QList<QString> quoteResults;
    int pos = 0;
    while((pos = exp.indexIn(express,pos))>=0)
    {
        QString quoteServiceName = exp.cap(3);
        QString quoteParaName = exp.cap(4);
        QString result = findQuoteResult(item,quoteServiceName,quoteParaName);
        quoteResults.append(result);
        pos += exp.matchedLength();
    }

    foreach(QString result,quoteResults)
    {
        int firstIndex = exp.indexIn(finalResult);
        if(firstIndex >= 0)
        {
            finalResult.replace(firstIndex,exp.matchedLength(),result);
        }
    }

    return finalResult;
}

//������õ����ƣ��������Ӧ��������ֵ
QString SimulateControlPanel::findQuoteResult(MyItem * item,QString quoteServiceName,QString quoteParaName)
{
    QList<MyItem *> parentItems =  SimulateUtil::instance()->getCurrParentItem(item);
    foreach(MyItem * item,parentItems)
    {
        if(item->getText() == quoteServiceName)
        {
            ParaList out = item->getServiceProp()->outputParas;

            foreach(Parameter * para,out)
            {
                if(para->pName == quoteParaName)
                {
                    return para->pValue;
                }
            }
        }
    }
    return "0";
}

//��ʾ��ǰ����Ԫ�Ŀ������
void SimulateControlPanel::showCurrProcessResultPanel(bool isPanelEditable)
{
    if(ui->simProcedure->count() > 0)
    {
        QListWidgetItem  * lastItem = ui->simProcedure->item(ui->simProcedure->count() - 1);
        MyListWidgetItem * citem = dynamic_cast<MyListWidgetItem *>(lastItem);
        MyGraphicsView::instance()->showSelectedItemPropEdit(citem->getUnit()->item,citem->data(Qt::UserRole),isPanelEditable);
    }
}

//ִ��Ԥ�������
void SimulateControlPanel::preExecuteServices()
{
    if(preOrResetIndex >= PreExeServices.size() && sflow == PRE_FLOW)
    {
        sflow = MAIN_FLOW;
    }

    if(sflow == PRE_FLOW && preOrResetIndex >=0 && preOrResetIndex < PreExeServices.size())
    {
        ServiceProperty  * prop = PreExeServices.at(preOrResetIndex);
        submitUrl(NULL,prop);
        preOrResetIndex++;
    }
    else if(sflow == MAIN_FLOW)
    {
        //��5���Դ���Ԫ���д���
        preOrResetIndex = 0;
        currProcUnit = procUnits.first();
        startProcUnit();
    }
}

//ִ�лָ�����
void SimulateControlPanel::resetExecuteServices()
{
    if(sflow == RESET_FLOW && preOrResetIndex >= ResetExeServices.size())
    {
        setSimulateState(false);
        currProcUnit->item->hightLightItem(LEVEL_NORMAL,true);
    }

    if(sflow == RESET_FLOW && preOrResetIndex >=0 && preOrResetIndex < ResetExeServices.size())
    {
        ServiceProperty  * prop = ResetExeServices.at(preOrResetIndex);
        submitUrl(NULL,prop);
        preOrResetIndex++;
    }
}

SimulateControlPanel::~SimulateControlPanel()
{
    delete ui;
}
