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

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
    connect(ui->simProcedure,SIGNAL(currentItemChanged(QListWidgetItem * , QListWidgetItem *)),this,SLOT(respItemChanged(QListWidgetItem * , QListWidgetItem *)));
    connect(ui->simProcedure,SIGNAL(itemClicked (QListWidgetItem*)),this,SLOT(respItemActivated(QListWidgetItem*)));
    connect(ui->simProcedure,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(respItemDoubleClicked(QListWidgetItem *)));
    connect(this,SIGNAL(sendSingleSimulate(ProcessUnit*)),this,SLOT(showSimulateOperate(ProcessUnit*)));
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
    emit resetSimluate();
    resetSimluateFlag();
    ui->simProcedure->clear();

    QList<QGraphicsItem *> existedItems;
    //��1����2��
    ReturnType returnType = SceneItemPickup::instance()->pickUpItems(existedItems);

    if(returnType != RETURN_SUCCESS)
    {
        setFlagState(ui->step1,false);
        return;
    }
    else
    {
        setFlagState(ui->step1,true);
    }

    //��3����4��
    QList<MyItem *> resortedItems;
    QList<ProcessUnit *> procUnits;
    returnType = ProcessCheck::instance()->checkProcess(existedItems,resortedItems,procUnits);

    if(returnType != RETURN_SUCCESS)
    {
        setFlagState(ui->step2,false);
        return;
    }
    else
    {
        setFlagState(ui->step2,true);
        setFlagState(ui->step3,true);
    }

    //��5���Դ���Ԫ���д���
    ProcessUnit * currUnit = procUnits.first();
    while(currUnit && currUnit->ptype != PRO_END)
    {
        emit sendSingleSimulate(currUnit);
        currUnit->item->hightLightItem(LEVEL_MIDDLE,true);

        if(currUnit->ptype == PRO_JUDGE)
        {
            int result = Util::getWarnChoice(currUnit->item->getText());

            if(result == QMessageBox::Yes)
            {
                currUnit = currUnit->yesChild;
            }
            else
            {
                currUnit = currUnit->noChild;
            }
        }
        else
        {
           currUnit = currUnit->nextChild;
        }

        if(currUnit && currUnit->ptype == PRO_END)
        {
            currUnit->item->hightLightItem(LEVEL_MIDDLE,true);
            emit sendSingleSimulate(currUnit);
        }
    }
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
