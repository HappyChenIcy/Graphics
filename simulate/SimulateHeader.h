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
class MyItem;

//��Ԫ����
enum ProcessType
{
    PRO_START,              //��ʼ:Բ�Ǿ���
    PRO_INPUT,              //�������:ƽ���ı���
    PRO_PROCESS,            //����:����
    PRO_JUDGE,              //�ж�:����
    PRO_PARALLEL,           //����:ƽ����
    PRO_LOOP,               //ѭ��
    PRO_END                 //����:Բ�Ǿ���
};

//����Ԫ
struct ProcessUnit
{
    ProcessUnit()
    {
        item = NULL;
        nextChild = NULL;
        yesChild = NULL;
        noChild = NULL;
    }
    ProcessType ptype;               //����Ԫ����
    GraphicsType gtype;              //�ؼ�������

    MyItem * item;                   //����Ԫ��Ӧ�Ŀؼ�Item
    ProcessUnit * nextChild;         //���жϿ��£�ָ���������
    ProcessUnit * yesChild;          //�жϿ�״̬�£�yes��no child����
    ProcessUnit * noChild;           //
};

#endif // SIMULATEHEADER_H
