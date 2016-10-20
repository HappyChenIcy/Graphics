#include "simulatecontrolpanel.h"
#include "ui_simulatecontrolpanel.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMessageBox>

#include "SimulateHeader.h"
#include "sceneitempickup.h"
#include "processcheck.h"

#include "../item/myitem.h"

SimulateControlPanel::SimulateControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulateControlPanel)
{
    ui->setupUi(this);

    setFixedWidth(300);

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
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
        currUnit->item->hightLightItem(true);

        if(currUnit->ptype == PRO_JUDGE)
        {
            int result = QMessageBox::warning(0,"����",currUnit->item->getText(),QMessageBox::Yes,QMessageBox::No);

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
            currUnit->item->hightLightItem(true);
            emit sendSingleSimulate(currUnit);
        }
    }
}

//��ʾÿһ�������ݵĲ�����Ϣ
void SimulateControlPanel::showSimulateOperate(ProcessUnit *unit)
{
    MY_ASSERT(unit)
    QPixmap pixmap;
    switch(unit->gtype)
    {
        case GRA_SQUARE:
                           pixmap.load(":/images/square.png");
                           break;
        case GRA_RECT:
                           pixmap.load(":/images/rectange.png");
                           break;
        case GRA_ROUND_RECT:
                           pixmap.load(":/images/roundedrect.png");
                           break;
        case GRA_CIRCLE:
                           pixmap.load(":/images/circle.png");
                           break;
        case GRA_ELLIPSE:
                           pixmap.load(":/images/ellipse.png");
                           break;
        case GRA_POLYGON:
                           pixmap.load(":/images/diamonds.png");
                           break;
        case GRA_TEXT:
                           pixmap.load(":/images/text.png");
                           break;
        case GRA_NODE_PORT:
                           pixmap.load(":/images/nodePort.png");
                           break;
        case GRA_PARALLELOGRAM:
                           pixmap.load(":/images/parallelogram.png");
                           break;
        case GRA_LOOP_UP:
                           pixmap.load(":/images/loop_up.png");
                           break;
        case GRA_LOOP_DOWN:
                           pixmap.load(":/images/loop_down.png");
                           break;
        case GRA_ANNOTATION:
                           pixmap.load(":/images/annotation.png");
                           break;
        case GAR_PARALLE:
                           pixmap.load(":/images/parallel.png");
                           break;
        default:
                           break;
    }

    QListWidgetItem * item = new QListWidgetItem;
    item->setIcon(QIcon(pixmap));
    item->setText(unit->item->getText());

    ui->simProcedure->addItem(item);
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
