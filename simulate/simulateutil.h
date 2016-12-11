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
**20161122:wey:���״̬���ʷ������жϿ����
**             ��Ӷ�!��֧��
**20161123:wey:��Ӷ��ж�����ִ�У�ʵ���жϿ�[!!!]
**20161211:wey:�ڽ���ģ��ʱ����ԭ��ֱ�߻������������
*************************************************/
#ifndef SIMULATEUTIL_H
#define SIMULATEUTIL_H

#include "../Header.h"

class MyItem;
class MyArrow;
class MyPathItem;

class SimulateUtil
{
public:
    static SimulateUtil * instance();

    QList<MyItem *> getCurrParentItem(MyItem *);

    MyItem * getMyItem(MyArrow * arrow,bool isEnd = true);
    MyItem * getMyItem(MyPathItem * pathItem,bool isEnd = true);
    QList<MyItem *> getInOutItems(MyItem *currItem, QList<MyArrow *>& arrows, QList<MyPathItem *> &pathItems, bool isEnd = true);
    void getItemInOutNum(MyItem * currItem, QList<MyArrow *>& arrows, QList<MyPathItem *> &pathItems, int &inputNum, int &outputNum);
    MyItem * getConditionItem(MyItem *currItem, QList<MyArrow *>& arrows, QList<MyPathItem *> &pathItems, bool isYes = true);

    bool getIsItemStart(MyItem * item);
    bool getIsItemEnd(MyItem * item);

    bool parseText(QString text,QStringList & result);
    bool getResult(QStringList & wordList);

private:
    SimulateUtil();
    static SimulateUtil * util;

    bool isRelationOperation(QString text);
    bool isLogicalOperation(QString text);

    //���������
    QVariant calcRelation(QStringList & result,int &index,bool & correct);
    QVariant calcLogical(QStringList & result,int &index,bool & correct);
    QVariant getNext(QStringList & result,int &index,bool & correct);

    //���ڴʷ������жϿ���ȡ�ַ�
    enum CharacterType
    {
        CHAR_START,         //δ��ʼ����
        CHAR_NORMAL,        //��ͨ�ַ�(����)
        CHAR_RELATION,      //��ϵ�����>��>=��==��!=��<��<=
        CHAR_LOGICAL,       //�߼������&&��||����
        CHAR_LEFT_AGGRE,    //������'('
        CHAR_RIGHT_AGGRE,   //������')'
        CHAR_OPPOSE,        //�߼���'!'
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
