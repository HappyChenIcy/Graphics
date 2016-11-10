#include "simulatecontrolpanel.h"
#include "ui_simulatecontrolpanel.h"

#include <QDebug>
#include <QApplication>
#include <QMessageBox>

#include "SimulateHeader.h"
#include "sceneitempickup.h"
#include "processcheck.h"
#include "simulateutil.h"

#include "../item/myitem.h"
#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../util.h"
#include "../global.h"
#include "../webservice/mywebservice.h"

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

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
    connect(ui->simProcedure,SIGNAL(currentItemChanged(QListWidgetItem * , QListWidgetItem *)),this,SLOT(respItemChanged(QListWidgetItem * , QListWidgetItem *)));
    connect(ui->simProcedure,SIGNAL(itemClicked (QListWidgetItem*)),this,SLOT(respItemActivated(QListWidgetItem*)));
    connect(ui->simProcedure,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(respItemDoubleClicked(QListWidgetItem *)));
    connect(this,SIGNAL(sendSingleSimulate(ProcessUnit*)),this,SLOT(showSimulateOperate(ProcessUnit*)));

    connect(MyWebService::instance(),SIGNAL(lastUnitProcessOver(bool,QString)),this,SLOT(procLastUnitResult(bool,QString)));
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
        if(tmpUnit->gtype == GRA_POLYGON)
        {
            qDebug()<<tmpUnit->item->getText()<<"__"<<tmpUnit->yesChild->item->getText()<<"_"<<tmpUnit->noChild->item->getText();
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


    //��5���Դ���Ԫ���д���
    currProcUnit = procUnits.first();
    startProcUnit();

    setSimulateState(false);
}

//��Ե�ǰ����Ԫ���صĽ�����д���
void SimulateControlPanel::procLastUnitResult(bool hasFault,QString context)
{
    if(hasFault)
    {
        currProcUnit->item->hightLightItem(LEVEL_MIDDLE,true);
        Util::showWarn("������ʳ�������������ֹ!");
        return;
    }

    currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
    if(currProcUnit->ptype == PRO_PROCESS)
    {
        ServiceProperty * prop = currProcUnit->item->getServiceProp();
        Parameter * para = prop->outputParas.at(0);

        para->pValue = context;

        currProcUnit = currProcUnit->nextChild;
    }
    //�жϿ�
    else if(currProcUnit->ptype == PRO_JUDGE)
    {

    }
    //ѭ����
    else if(currProcUnit->ptype == PRO_LOOP)
    {

    }
    //����
    else if(currProcUnit->ptype == PRO_END)
    {

    }

    startProcUnit();
}

//��ʼ����Ԫ
void SimulateControlPanel::startProcUnit()
{
    while(true)
    {
        if(currProcUnit->ptype == PRO_START)
        {
            currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
            emit sendSingleSimulate(currProcUnit);
            currProcUnit = currProcUnit->nextChild;
        }
        //�����
        else if(currProcUnit->ptype == PRO_PROCESS)
        {
            ServiceProperty * prop = currProcUnit->item->getServiceProp();
            submitUrl(currProcUnit->item,prop);
            emit sendSingleSimulate(currProcUnit);
            break;
        }
        //�жϿ�
        else if(currProcUnit->ptype == PRO_JUDGE)
        {
break;
        }
        //ѭ����
        else if(currProcUnit->ptype == PRO_LOOP)
        {
break;
        }
        //����
        else if(currProcUnit->ptype == PRO_END)
        {
            emit sendSingleSimulate(currProcUnit);
            currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
            break;
        }
    }
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
            if(paraValue.startsWith(QString(COMBOX_START_FLAG)))
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

//    qDebug()<<fullUrl;

    MyWebService::instance()->submit(fullUrl);
}

//�������õĲ���������������õ����ֵ
QString SimulateControlPanel::getQuoteOutValue(MyItem * item,QString value)
{
   QList<MyItem *> parentItems =  SimulateUtil::instance()->getCurrParentItem(item);
   int pos = value.indexOf("]");
   int startLen = QString(COMBOX_START_FLAG).size();
   int parentIndex = value.mid(startLen,pos - startLen).toInt();

   if(parentIndex >= 0 && parentIndex < parentItems.size())
   {
       ServiceProperty * sp = parentItems.at(parentIndex)->getServiceProp();
       ParaList out = sp->outputParas;

       if(out.size() > 0 )
       {
           return out.at(0)->pValue;
       }
   }

   return "0";
}

//�����Ƿ�Ϊ����״̬���ÿؼ���״̬
void SimulateControlPanel::setSimulateState(bool isSim)
{
    GlobalIsSimulateState = isSim;
    ui->startSimulate->setEnabled(!isSim);
}

//��ʾÿһ�������ݵĲ�����Ϣ
void SimulateControlPanel::showSimulateOperate(ProcessUnit *unit)
{
    MY_ASSERT(unit)
    QPixmap pixmap;

    Util::loadPixmapByGType(unit->gtype,pixmap);

    MyListWidgetItem * item = new MyListWidgetItem;
    item->setIcon(QIcon(pixmap));
    item->setText(unit->item->getText());
    item->bindProscessUnit(unit);

    ui->simProcedure->addItem(item);
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
        pitem->getUnit()->item->hightLightItem(LEVEL_MIDDLE,true);
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
        MyGraphicsView::instance()->showSelectedItemPropEdit(citem->getUnit()->item);
//        QEvent * event(QEvent::GraphicsSceneContextMenu);
//        MyGraphicsView::instance()->scene()->sendEvent(citem->getUnit()->item,event);
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

SimulateControlPanel::~SimulateControlPanel()
{
    delete ui;
}
