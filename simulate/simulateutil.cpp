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
 *��2��20161107�޸���dowhile������rect����ѭ������(��δ���жϿ��ܵ�����(�ж�/ѭ��))
 *!*/
QList<MyItem *> SimulateUtil::getCurrParentItem(MyItem * item)
{
    QList<MyItem *> list;

    MyItem * pItem = item;
    bool isFirst = true;

    QStack<MultiPathDesc * > descs;       //��¼���ݹ�����ĳ���ؼ�������������Ķ�·��
    QStack<MyItem * > polygons;           //��¼�Ƶ������������Ķ����(�������жϣ�Ҳ������ѭ��)
    MultiPathDesc * currMPath = NULL;

    bool isSearchOver = false;

    if(item->getType() == GRA_RECT)
    {
        while(pItem&&!isSearchOver)
        {
            QList<MyItem *> pItems = getInOutItems(pItem,pItem->getArrows(),false);

//            qDebug()<<pItem->getText();

            bool hasAdded = false;
            bool skipCurrLoop = false;

            if(pItem->getType() == GRA_POLYGON)
            {
                bool hasAddPolygon = false;

                if(pItem == item)
                {
                    hasAddPolygon = true;
                }

                foreach(MyItem * tmpItem,polygons)
                {
                    if(tmpItem == pItem)
                    {
                        hasAddPolygon = true;
                        break;
                    }
                }

                if(!hasAddPolygon)
                {
                    polygons.append(pItem);
                }
                //�Ѿ����룬��˵��������ѭ��
                else
                {
                    hasAdded = true;
                }
            }
            //Ϊ�˷�ֹ֧���е�һ��itemΪrect������©������
            else if (pItem->getType() == GRA_RECT)
            {
                if(!isFirst)
                {
                    bool hasAddRect = false;

                    if(pItem == item)
                    {
                        hasAddRect = true;
                    }

                    foreach(MyItem * tmpItem ,list)
                    {
                        if(tmpItem == item)
                        {
                            hasAddRect = true;
                            break;
                        }
                    }

                    if(!hasAddRect)
                    {
                        if(descs.size() > 0)
                        {
                            MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                            SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                            spathDesc->hasAddItem++;
                        }

                        qDebug()<<"===add:"<<pItem->getText();
                        list.append(pItem);
                    }
                    else
                    {
                        while(descs.size() > 0 )
                        {
                            MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                            SignalPathDesc * spathDesc = NULL;

                            if(lastDesc->currPathNum < lastDesc->totalPathNum)
                            {
                                spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                            }

                            if(spathDesc)
                            {
                                for(int i=0;i<spathDesc->hasAddItem;i++)
                                {
                                    qDebug()<<"===remve:"<<list.last()->getText();
                                    list.removeLast();
                                }

                                spathDesc->isSearching = false;
                                spathDesc->hasSearched = true;

                                lastDesc->currPathNum += 1;
                                //��ǰ�Ѿ���������,
                                if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                                {
                                    delete lastDesc;
                                    descs.pop();
                                }
                                else
                                {
                                    SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                                    pItem = spathDesc->startItem;
                                    skipCurrLoop = true;
                                    qDebug()<<"+++"<<pItem->getText();
                                    break;
                                }
                            }

                            if(descs.size() > 0)
                            {
                                MultiPathDesc  *  parentLastDesc = descs.at(descs.size()-1);
                                parentLastDesc->currPathNum++;
                                if(parentLastDesc->currPathNum >= parentLastDesc->totalPathNum)
                                {
                                    delete parentLastDesc;
                                    descs.pop();
                                }
                                else
                                {
                                    SignalPathDesc * spathDesc = parentLastDesc->pathes.at(parentLastDesc->currPathNum);
                                    pItem = spathDesc->startItem;
                                    break;
                                }
                            }

                            if(descs.size() == 0)
                            {
                                isSearchOver = true;
                                break;
                            }
                       }
                    }
                }
            }

            isFirst = false;

            if(!skipCurrLoop)
            {
                if(!hasAdded)
                {
                    if(pItems.size() > 1)
                    {
                        for(int i = 0;i < descs.size() ; i++)
                        {
                            if(descs.at(i)->currItem == pItem)
                            {
                                hasAdded = true;
                            }
                            break;
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
                    //����ж�·�����ڣ���Ҫ�Ե�ǰ·�����и��£������Ѿ�����ķ��򵯳�
                    while(descs.size() > 0 )
                    {
                        MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                        SignalPathDesc * spathDesc = NULL;

                        if(lastDesc->currPathNum < lastDesc->totalPathNum)
                        {
                            spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                        }

                        if(spathDesc)
                        {
                            for(int i=0;i<spathDesc->hasAddItem;i++)
                            {
                                qDebug()<<"===remve:"<<list.last()->getText();
                                list.removeLast();
                            }

                            spathDesc->isSearching = false;
                            spathDesc->hasSearched = true;

                            lastDesc->currPathNum += 1;
                            //��ǰ�Ѿ���������,
                            if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                            {
                                delete lastDesc;
                                descs.pop();
                            }
                            else
                            {
                                SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                                pItem = spathDesc->startItem;
                                break;
                            }
                        }

                        if(descs.size() > 0)
                        {
                            MultiPathDesc  *  parentLastDesc = descs.at(descs.size()-1);
                            parentLastDesc->currPathNum++;
                            if(parentLastDesc->currPathNum >= parentLastDesc->totalPathNum)
                            {
                                delete parentLastDesc;
                                descs.pop();
                            }
                            else
                            {
                                SignalPathDesc * spathDesc = parentLastDesc->pathes.at(parentLastDesc->currPathNum);
                                pItem = spathDesc->startItem;
                                break;
                            }
                        }

                        if(descs.size() == 0)
                        {
                            isSearchOver = true;
                            break;
                        }
                    }
                 }
            }
        }
    }

//    foreach(MyItem * tmpItem,list)
//    {
//        qDebug()<<"result:"<<tmpItem->getText();
//    }

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
