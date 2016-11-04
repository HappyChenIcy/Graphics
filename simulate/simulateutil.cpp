#include "simulateutil.h"

#include "../item/myitem.h"
#include "../item/myarrow.h"
#include "../item/draglinepoint.h"

#include <QStack>

#include <QDebug>

SimulateUtil * SimulateUtil::util = NULL;

SimulateUtil::SimulateUtil()
{
}

SimulateUtil * SimulateUtil::instance()
{
    if(!util)
    {
        util = new SimulateUtil;
    }
    return util;
}

/*!
 *��ȡ��ǰ�ؼ��ĸ��ؼ���ֻ�����жϿ�(GRA_RECT).
 *��1��20161101���Ӷ�ѭ����������������
 *!*/
QList<MyItem *> SimulateUtil::getCurrParentItem(MyItem * item)
{
    QList<MyItem *> list;

    MyItem * pItem = item;

    QStack<MultiPathDesc * > descs;
    MultiPathDesc * currMPath = NULL;

    bool isSearchOver = false;

    if(item->getType() == GRA_RECT)
    {
        while(pItem&&!isSearchOver)
        {
            QList<MyItem *> pItems = getInOutItems(pItem,pItem->getArrows(),false);

            bool hasAdded = false;
            if(pItems.size() > 1)
            {

                for(int i = 0;i < descs.size() ; i++)
                {
                    if(descs.at(i)->currItem == pItem)
                    {
                        hasAdded = true;
                    }
                }

                if(!hasAdded)
                {
                    MultiPathDesc * mpath = new MultiPathDesc;
                    mpath->totalPathNum = pItems.size();
                    mpath->currItem = pItem;
                    foreach(MyItem * tmp,pItems)
                    {
                        SignalPathDesc * path = new SignalPathDesc;
                        path->startItem = tmp;
                        mpath->pathes.append(path);
                    }

                    descs.append(mpath);
                    currMPath = mpath;

                    pItem = pItems.at(0);
                }
            }
            else if(pItems.size() == 1)
            {
                pItem = pItems.at(0);
            }
            else
            {
                isSearchOver = true;
            }

            //�жϵ�ǰ�Ƿ�Ϊ��ʼ
            if(getIsItemStart(pItem))
            {
                if(descs.size() > 0)
                {
                    MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                    SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                    spathDesc->isSearching = false;
                    spathDesc->hasSearched = true;

                    lastDesc->currPathNum += 1;

                    //��ǰ�Ѿ���������
                    if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                    {
                        delete lastDesc;
                        descs.pop();
                    }
                    else
                    {
                        SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                        pItem = spathDesc->startItem;
                    }
                }

                if(descs.size() == 0)
                {
                    isSearchOver = true;
                }
            }

            //������ѭ��ʱ��ѭ����ָ�����Լ�
            if(hasAdded)
            {
                //���Ѿ�����ķ��򵯳�
                if(descs.size() > 0 )
                {
                    MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                    SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);

                    for(int i=0;i<spathDesc->hasAddItem;i++)
                    {
                        list.removeLast();
                    }

                    spathDesc->isSearching = false;
                    spathDesc->hasSearched = true;

                    lastDesc->currPathNum += 1;
                    //��ǰ�Ѿ���������
                    if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                    {
                        delete lastDesc;
                        descs.pop();
                    }
                    else
                    {
                        SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                        pItem = spathDesc->startItem;
                    }

                    if(descs.size() == 0)
                    {
                        isSearchOver = true;
                    }
                }
            }
            else
            {
                if(pItem->getType() == GRA_RECT)
                {
                    if(descs.size() > 0)
                    {
                        MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                        SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                        spathDesc->hasAddItem++;
                    }
                    list.append(pItem);
                }
            }
        }
    }
    return list;
}

//��ȡ��ͷ��ʼ��/�յ�Item
MyItem * SimulateUtil::getMyItem(MyArrow * arrow, bool isEnd)
{
    if(arrow->getLineType() == LINE_MYITEM)
    {
        DragLinePoint * dragLine = NULL;
        if(isEnd)
        {
            dragLine = dynamic_cast<DragLinePoint *>(arrow->getEndItem());
        }
        else
        {
            dragLine = dynamic_cast<DragLinePoint *>(arrow->getStartItem());
        }

        if(dragLine)
        {
            MyItem * item = dynamic_cast<MyItem *>(dragLine->getParentItem());
            return item;
        }
    }
    return NULL;
}

//��ȡ�ؼ�����Ϊ��ͷ�����յ�ؼ�����
QList<MyItem *> SimulateUtil::getInOutItems(MyItem *currItem,QList<MyArrow *> &arrows, bool isEnd)
{
    QList<MyItem *> resultList;

    foreach (MyArrow * arrow, arrows)
    {
       MyItem * item = SimulateUtil::instance()->getMyItem(arrow,isEnd);
       if(item != currItem)
       {
           resultList.append(item);
       }
    }
    return resultList;
}

//��ȡĳ���ؼ���Ϊ��ͷ�����յ����
void SimulateUtil::getItemInOutNum(MyItem *currItem, QList<MyArrow *> &arrows, int &inputNum, int &outputNum)
{
    QString currItemId = currItem->getProperty().startItemID;

    foreach(MyArrow * arrow,arrows)
    {
        MyItem * startItem = SimulateUtil::instance()->getMyItem(arrow,false);
        MyItem * endItem = SimulateUtil::instance()->getMyItem(arrow,true);
        QString startItemId = startItem->getProperty().startItemID;
        QString endItemId = endItem->getProperty().startItemID;

        if(startItemId == currItemId)
        {
            inputNum++;
        }
        else if(endItemId == currItemId)
        {
            outputNum++;
        }
    }
}

//�ֱ��ȡ���������Ͳ�������item
MyItem * SimulateUtil::getConditionItem(MyItem * currItem,QList<MyArrow *> &arrows, bool isYes)
{
    foreach(MyArrow * arrow,arrows)
    {
        MyItem * item = SimulateUtil::instance()->getMyItem(arrow,true);
        if(item != currItem && isYes && (arrow->getText().toLower()== "yes" || arrow->getText() == "��"))
        {
            return item;
        }
        else if(item != currItem && !isYes && (arrow->getText().toLower()== "no" || arrow->getText() == "��"))
        {
            return item;
        }
    }
    return NULL;
}

//�жϵ�ǰ�Ƿ�Ϊ��ʼ��
bool SimulateUtil::getIsItemStart(MyItem *item)
{
    QString text = item->getText();
    if(text == "��ʼ" || text.toLower().contains("start"))
    {
        return true;
    }

    return false;
}

//�жϵ�ǰ�Ƿ�Ϊ������
bool SimulateUtil::getIsItemEnd(MyItem *item)
{
    QString text = item->getText();
    if(text == "����" || text.toLower().contains("end"))
    {
        return true;
    }

    return false;
}
