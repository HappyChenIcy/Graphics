#include "simulatecontrolpanel.h"
#include "ui_simulatecontrolpanel.h"

#include <QDebug>
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

    QList<QGraphicsItem *> existedItems;
    //��1����2��
    ReturnType returnType = SceneItemPickup::instance()->pickUpItems(existedItems);

    if(returnType != RETURN_SUCCESS)
    {
        QMessageBox::information(0,"��ʾ",QString("%1").arg(returnType));
        return;
    }
    else
    {
        QMessageBox::information(0,"��ʾ","��ʼ��֤��ȷ");
    }

    //��3����4��
    QList<MyItem *> resortedItems;
    QList<ProcessUnit *> procUnits;
    returnType = ProcessCheck::instance()->checkProcess(existedItems,resortedItems,procUnits);

    if(returnType != RETURN_SUCCESS)
    {
        QMessageBox::information(0,"��ʾ",QString("%1").arg(returnType));
        return;
    }
    else
    {
        QMessageBox::information(0,"��ʾ","������֤��ת����ȷ");
    }

    //��5���Դ���Ԫ���д���
    ProcessUnit * currUnit = procUnits.first();
    while(currUnit && currUnit->ptype != PRO_END)
    {
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
        }
    }
}

SimulateControlPanel::~SimulateControlPanel()
{
    delete ui;
}
