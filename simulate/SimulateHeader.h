/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: SimulateHeader.h
**��  ��: wey       Version: 1.0       Date: 2016.10.11
**��  ��:
**Others: ����ģ��ṹ�嶨���ļ�
**
**�޸���ʷ:
**
*************************************************/
#ifndef SIMULATEHEADER_H
#define SIMULATEHEADER_H

#include "../Header.h"

class QGraphicsItem;

//��Ԫ����
enum ProcessType
{
    PRO_START,              //��ʼ:Բ�Ǿ���
    PRO_INPUT,              //����:ƽ���ı���
    PRO_OUTPUT,             //���:ƽ���ı���
    PRO_PROCESS,            //����:����
    PRO_JUDGE,              //�ж�:����
    PRO_PARALLEL,           //����:ƽ����
    PRO_LOOP,               //ѭ��:
    PRO_END                 //����:Բ�Ǿ���
};

//����Ԫ
struct ProcessUnit
{
    ProcessType ptype;               //����Ԫ����
    GraphicsType gtype;              //�ؼ�������

    QGraphicsItem * item;            //ָ���Item

    QString id;


    ProcessUnit * next;
};

#endif // SIMULATEHEADER_H
