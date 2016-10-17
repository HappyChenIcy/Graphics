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
*************************************************/
#ifndef PROCESSCHECK_H
#define PROCESSCHECK_H

#include "SimulateHeader.h"

class MyItem;
class MyArrow;

class ProcessCheck
{
public:
    //���ڼ�¼���ݹ������������жϿ�
    struct PolygonDesc
    {
        PolygonDesc()
        {
            isProcLeft = false;
            isProcRight = false;
            isLeftOver = false;
            isRightOver = false;
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
        PolygonDesc * leftChild;             //������ֱ���жϿ�
        PolygonDesc * rightChild;            //������ֱ���жϿ�
        PolygonDesc * parent;                //����
        ProcessUnit * processUnit;           //����Ԫ
    };

    static ProcessCheck * instance();

    ReturnType checkProcess(QList<QGraphicsItem *> &existedItems, QList<MyItem *> &resortedItems, QList<ProcessUnit *> &procUnits);

private:
    MyItem * getMyItem(MyArrow * arrow,bool isEnd = true);
    QList<MyItem *> getInOutItems(MyItem *currItem, QList<MyArrow *>& arrows, bool isEnd = true);
    void getItemInOutNum(MyItem * currItem,QList<MyArrow *>& arrows,int &inputNum,int &outputNum);
    MyItem * getConditionItem(MyItem *currItem, QList<MyArrow *>& arrows, bool isYes = true);

    ProcessCheck();
    static ProcessCheck * pcheck;

};

#endif // PROCESSCHECK_H
