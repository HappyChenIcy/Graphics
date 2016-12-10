/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: processcheck.h
**��  ��: wey       Version: 1.0       Date: 2016.10.11
**��  ��:
**Others: ������֤
**       ��3��:�ж�������·�Ƿ�������ͬʱ�ж��߼��Ƿ���ȷ�����ҽ��ؼ������߼�˳���������С���Ҫ��
**            Ŀǰֻ����˳����жϽṹ�������ǲ���
**�޸���ʷ:
**20161012:wey:������̽�������(�ݲ�֧��ѭ���Ͳ���)
**20161017:wey:��Ӷ���֤�Ķ��пؼ�����ȡ��ת���ɴ���Ԫ
**20161024:wey:�޸������̲������������ݼ��ʱ����������
**20161027:wey:�޸������ж�δ��ȫ������������������Զ��˳�
**20161104:wey:��Ӷ�for��whileѭ����֧�֡���������
**20161106:wey:��Ӷ�doWhileѭ����֧�֡�����doWhileѭ�����Ƿ��жϻ��غϡ�
**20161130:wey:������δ�޸�������dowhileѭ��ʱno��֧��Ϊѭ���嵼������ѭ��,PolygonDesc����isLoopYes�ֶ�����
**20161210:wey:�����������������ε�no��ָ֧��ǰ��Ŀؼ����������϶�Ϊ�жϿ����dowhileѭ����ֻ�����ε�yes��ָ֧����ǰ�ؼ������϶�Ϊdowhileѭ����
*************************************************/
#ifndef PROCESSCHECK_H
#define PROCESSCHECK_H

#include "SimulateHeader.h"

class MyItem;
class MyArrow;

class ProcessCheck
{
public:
    //���ڼ�¼���ݹ������������жϿ�(��ѭ������)
    struct PolygonDesc
    {
        PolygonDesc()
        {
            isProcLeft = false;
            isProcRight = false;
            isLeftOver = false;
            isRightOver = false;
            isLoopYes = true;
            currItem = NULL;
            leftItem = NULL;
            rightItem = NULL;
            leftChild = NULL;
            rightChild = NULL;
            parent = NULL;
            processUnit = NULL;
        }

        bool isProcLeft;                     //�Ƿ����ڴ���������
        bool isProcRight;                    //�Ƿ����ڴ���������
        bool isLeftOver;                     //�������Ƿ������
        bool isRightOver;                    //�������Ƿ������
        MyItem * currItem;                   //��ǰָ��Ŀؼ�
        MyItem * leftItem;                   //��ǰ��ؼ�
        MyItem * rightItem;                  //��ǰ�ҿؼ�
        bool isLoopYes;                      //����Ϊѭ����ʱ����ʾyes��֧��Ϊѭ������������no��֧��Ϊѭ����Ĭ��Ϊyes
        PolygonDesc * leftChild;             //������ֱ���жϿ�
        PolygonDesc * rightChild;            //������ֱ���жϿ�
        PolygonDesc * parent;                //����
        ProcessUnit * processUnit;           //����Ԫ
    };
    static ProcessCheck * instance();

    ReturnType checkProcess(QList<QGraphicsItem *> &existedItems, QList<MyItem *> &resortedItems, QList<ProcessUnit *> &procUnits);

private:
    ProcessCheck();
    static ProcessCheck * pcheck;

};

#endif // PROCESSCHECK_H
