#include "simulatecontrolpanel.h"
#include "ui_simulatecontrolpanel.h"

#include <QDebug>
#include <QMessageBox>

#include "SimulateHeader.h"
#include "sceneitempickup.h"
#include "processcheck.h"

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

    //��3��
    returnType = ProcessCheck::instance()->checkProcess(existedItems);

    if(returnType != RETURN_SUCCESS)
    {
        QMessageBox::information(0,"��ʾ",QString("%1").arg(returnType));
        return;
    }
    else
    {
        QMessageBox::information(0,"��ʾ","������֤��ȷ");
    }


}

SimulateControlPanel::~SimulateControlPanel()
{
    delete ui;
}
