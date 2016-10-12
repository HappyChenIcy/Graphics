#include "processcheck.h"

#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../item/myitem.h"
#include "../item/myarrow.h"
#include "../item/draglinepoint.h"

#include <QDebug>
#include <QStack>

ProcessCheck * ProcessCheck::pcheck = NULL;

ProcessCheck::ProcessCheck()
{

}

ProcessCheck * ProcessCheck::instance()
{
    if(!pcheck)
    {
        pcheck = new ProcessCheck();
    }
    return pcheck;
}

/*!
 *��3��:�ж�������·�Ƿ�������ͬʱ�ж��߼��Ƿ���ȷ�����ҽ��ؼ������߼�˳���������С���Ҫ��
 *����:
 *[1]:��ͷ���з���ģ������ϳ���ͷ����㣬�ɿ������ͷ���յ㣬�������ͷ���Ÿı���ı䡣
 *[2]:�˰汾��ֻ�漰˳��ͼ򵥵ķ�֧�ṹ(20161011��)
 *!*/
ReturnType ProcessCheck::checkProcess(QList<QGraphicsItem *> &existedItems)
{
    //[1]�ж������Ƿ�������ͬʱ������������
    QList<MyItem * > resortItems;

    MyItem * startItem;
    MyItem * endItem;

    foreach(QGraphicsItem * item,existedItems)
    {
        if(TYPE_ID(*item) == TYPE_ID(MyItem))
        {
            MyItem * tmpItem = dynamic_cast<MyItem *>(item);
            if(tmpItem && tmpItem->getType() == GRA_ROUND_RECT)
            {
                QString text = tmpItem->getText();
                if(text == "��ʼ" || text.toLower().contains("start"))
                {
                    startItem = tmpItem;
                }
                else if(text == "����" || text.toLower().contains("end"))
                {
                    endItem = tmpItem;
                }
            }
        }
    }
    bool isAtEnd = false;

    MyItem * currItem = startItem;        //��ǰ����ָ��ָ��ؼ�
    QStack<PolygonDesc *> polygons;       //���ݹ����м�¼�����������жϿ�

    while(!isAtEnd)
    {
        resortItems.append(currItem);

//        qDebug()<<currItem->getText()<<"______";

        QList<MyArrow *> arrows = currItem->getArrows();

        //��ʼ��
        if(currItem == startItem)
        {
            if(arrows.size() == 0 || arrows.size() > 1)
            {
                return FLOW_SUCCESS;
            }

            currItem = getMyItem(arrows.at(0));
        }
        //������
        else if(currItem == endItem)
        {
            if(arrows.size() == 0)
            {
                return FLOW_SUCCESS;
            }

            if(polygons.size() > 0)
            {
                PolygonDesc * topDesc = polygons.at(polygons.size() -1);
                //���δ������ɣ������ദ����ɣ�Ȼ�󽫴���Ȩ�����ҿؼ�
                if(topDesc->isProcLeft && !topDesc->isLeftOver)
                {
                    topDesc->isLeftOver = true;
                    topDesc->isProcLeft = false;
                    topDesc->isProcRight = true;

//                    resortItems.removeLast();
                    currItem = topDesc->rightItem;
                }
                //�Ҳ�δ�����꣬����ҿؼ��������
                else if(topDesc->isProcRight && !topDesc->isRightOver)
                {
                    topDesc->isRightOver = true;
                    topDesc->isProcRight = false;
//                    resortItems.removeLast();
                }

                //���Ҿ�������,����и��࣬�򽫴���Ȩ�������࣬���ҽ��Լ���ջ���Ƴ�
                if(topDesc->isLeftOver && topDesc->isRightOver)
                {
                    PolygonDesc * parent = topDesc->parent;
                    if(parent && !parent->isLeftOver)
                    {
                        parent->isProcLeft = false;
                        parent->isLeftOver = true;
                        parent->isProcRight = true;

//                        resortItems.removeLast();
                        currItem = parent->rightItem;
                    }
                    else if(parent && !parent->isRightOver)
                    {
                        parent->isProcRight = false;
                        parent->isRightOver = true;
//                        resortItems.removeLast();
                    }
                    polygons.pop();

                    //����������Ҿ���������������̽���
                    if(parent && parent->isLeftOver && parent->isRightOver)
                    {
                        isAtEnd = true;
                        polygons.pop();
                    }
                }
            }
            else
            {
                isAtEnd = true;
            }
        }
        else
        {
            GraphicsType gtype = currItem->getType();

            int inNum = 0,outNum = 0;               //��ǰ�ؼ��ֱ���Ϊ�����յ�����ֱ�ߵĸ���
            getItemInOutNum(currItem,arrows,inNum,outNum);

            //���������/�����[��ע��ͷָ���]
            if(gtype == GRA_PARALLELOGRAM || gtype == GRA_RECT)
            {
                if(inNum !=1 || outNum != 1)
                {
                    return FLOW_SUCCESS;
                }

                //��ǰ�ؼ�����һ���ڵ�
                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                if(outItems.size() == 1)
                {
                    currItem = outItems.at(0);
                }
            }
            //�ж�[1�����룬2�����]
            else if(gtype == GRA_POLYGON)
            {
                if(inNum !=2 || outNum != 1)
                {
                    return FLOW_SUCCESS;
                }

                //��ǰ�ؼ�����һ���ڵ�
                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                PolygonDesc * desc = new PolygonDesc;
                desc->currItem = currItem;

                //���������жϿ�ʱ����Ҫ�������жϿ��ջ�����жϿ�Ĺ�ϵ
                if(polygons.size() > 0)
                {
                    PolygonDesc * currDesc = polygons.at(polygons.size() - 1);
                    desc->parent = currDesc;
                }

                polygons.push(desc);

                //[�Զ������һ���ڵ�]
                desc->isProcLeft = true;
                desc->leftItem = outItems.at(0);
                desc->rightItem = outItems.at(1);
                currItem = outItems.at(0);
            }
        }
    }

    foreach(MyItem * item,resortItems)
    {
        qDebug()<<item->getText();
    }

    //[2]��֤�߼��Ƿ���ȷ
    return RETURN_SUCCESS;
}

//��ȡĳ���ؼ���Ϊ��ͷ�����յ����
void ProcessCheck::getItemInOutNum(MyItem *currItem, QList<MyArrow *> &arrows, int &inputNum, int &outputNum)
{
    QString currItemId = currItem->getProperty().startItemID;

    foreach(MyArrow * arrow,arrows)
    {
        MyItem * startItem = getMyItem(arrow,false);
        MyItem * endItem = getMyItem(arrow,true);
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

//��ȡ�ؼ�����Ϊ��ͷ�����յ�ؼ�����
QList<MyItem *> ProcessCheck::getInOutItems(MyItem *currItem,QList<MyArrow *> &arrows, bool isEnd)
{
    QList<MyItem *> resultList;

    foreach (MyArrow * arrow, arrows)
    {
       MyItem * item = getMyItem(arrow,isEnd);
       if(item != currItem)
       {
           resultList.append(item);
       }
    }
    return resultList;
}

//��ȡ��ͷ��ʼ��/�յ�Item
MyItem * ProcessCheck::getMyItem(MyArrow * arrow, bool isEnd)
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

