/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: simulateutil.h
**��  ��: wey       Version: 1.0       Date: 2016.10.31
**��  ��:���ݵĹ�����
**Others:1.����ĳ���ؼ�����ȡ�����еĸ��ؼ�
**
**�޸���ʷ:
**20161101:wey:����GRA_RECT��������[!!!]
**20161107:wey:�޸���dowhile������rect����ѭ������(��δ���жϿ��ܵ�����(�ж�/ѭ��))
**20161117:wey:���Ӷ�ѭ�����жϿؼ�����������
*************************************************/
#ifndef SIMULATEUTIL_H
#define SIMULATEUTIL_H

#include "../Header.h"

class MyItem;
class MyArrow;

class SimulateUtil
{
public:
    static SimulateUtil * instance();

    QList<MyItem *> getCurrParentItem(MyItem *);

    MyItem * getMyItem(MyArrow * arrow,bool isEnd = true);
    QList<MyItem *> getInOutItems(MyItem *currItem, QList<MyArrow *>& arrows, bool isEnd = true);
    void getItemInOutNum(MyItem * currItem,QList<MyArrow *>& arrows,int &inputNum,int &outputNum);
    MyItem * getConditionItem(MyItem *currItem, QList<MyArrow *>& arrows, bool isYes = true);

    bool getIsItemStart(MyItem * item);
    bool getIsItemEnd(MyItem * item);

    bool parseText(QString text,QStringList & result);

private:
    SimulateUtil();
    static SimulateUtil * util;

    bool isRelationOperation(QString text);
    bool isLogicalOperation(QString text);

    //���ڴʷ������жϿ���ȡ�ַ�
    enum CharacterType
    {
        CHAR_START,         //δ��ʼ����
        CHAR_NORMAL,        //��ͨ�ַ�(����)
        CHAR_RELATION,      //��ϵ�����
        CHAR_LOGICAL,       //�߼������
        CHAR_LEFT_AGGRE,    //������
        CHAR_RIGHT_AGGRE,   //������
        CHAR_NOT_EXIST,     //��ϵ��ַ�δ����
        CHAR_ILLEGAL        //�Ƿ��ַ�
    };

    //����·����״̬
    struct SignalPathDesc
    {
        SignalPathDesc()
        {
            hasSearched = false;
            isSearching = false;
            hasAddItem = 0;
            startItem = NULL;
        }
        bool isSearching;           //�Ƿ�������Ѱ�˽ڵ�
        bool hasSearched;           //�Ƿ��Ѿ���������
        int hasAddItem;             //�ڵ�ǰ·���£��Ѿ���ӵ���������е�item������������ѭ��״̬�·��򵯳�
        MyItem * startItem;         //��ʼ����item
    };

    //���ڼ�¼һ��GRA_RECT���ڶ�������ͷʱ�����ÿ����ͷ��·����������¼ÿ��·����״̬
    struct MultiPathDesc
    {
        MultiPathDesc()
        {
            totalPathNum = 0;
            currPathNum = 0;
            currItem = NULL;
        }
        int totalPathNum;       //��·������
        int currPathNum;        //��ǰ��Ѱ������,currPathNum = totalPathNum -1;
        MyItem * currItem;      //������֧��GRA_RECT

        QList<SignalPathDesc*> pathes;      //�ܹ����ڵ�·��
    };

    QStringList relationList;       //��ϵ�������
    QStringList logicalList;        //�߼��������
};

#endif // SIMULATEUTIL_H
