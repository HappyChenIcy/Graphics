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
 *��4��:��ͷ��ʼ��ȡ�ؼ�������ת���ɴ���Ԫ��ѹ�봦����С���Ҫ��
 *����:
 *[1]:��ͷ���з���ģ������ϳ���ͷ����㣬�ɿ������ͷ���յ㣬�������ͷ���Ÿı���ı䡣
 *[2]:�˰汾��ֻ�漰˳��ͼ򵥵ķ�֧�ṹ(20161011��)
 *[3]:�ڴ����жϿ�ʱ��Ĭ���ȴ���yes������no��֧��
 *!*/
ReturnType ProcessCheck::checkProcess(QList<QGraphicsItem *> &existedItems,QList<MyItem *> &resortedItems,QList<ProcessUnit *> & procUnits)
{
    //[1]�ж������Ƿ�������ͬʱ������������
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

    ProcessUnit * frontUnit = NULL;

    ProcessUnit * startUnit = NULL;
    ProcessUnit * endUnit = NULL;

    while(!isAtEnd)
    {
        if(!currItem)
        {
            return RETURN_ERROR;
        }
        resortedItems.append(currItem);

        QList<MyArrow *> arrows = currItem->getArrows();

        //��ʼ��
        if(currItem == startItem)
        {
            if(arrows.size() == 0 || arrows.size() > 1)
            {
                return FLOW_ERROR;
            }

            currItem = getMyItem(arrows.at(0));

            ProcessUnit  * unit = new ProcessUnit;
            unit->gtype = startItem->getType();
            unit->ptype = PRO_START;
            unit->item =  startItem;

            startUnit = unit;
            frontUnit = unit;
            procUnits.push_back(unit);
        }
        //������
        else if(currItem == endItem)
        {
            if(arrows.size() == 0)
            {
                return FLOW_ERROR;
            }

            if(!endUnit)
            {
                ProcessUnit  * unit = new ProcessUnit;
                unit->gtype = endItem->getType();
                unit->item =  endItem;
                unit->ptype = PRO_END;

                endUnit = unit;
            }

            if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
            {
                PolygonDesc * currDesc = polygons.at(polygons.size() - 1);

                if(currDesc->isProcLeft)
                {
                    frontUnit->yesChild = endUnit;
                }
                else if(currDesc->isProcRight)
                {
                    frontUnit->noChild = endUnit;
                }
            }
            else
            {
                frontUnit->nextChild = endUnit;
            }

            if(polygons.size() > 0)
            {
                //���δ������ɣ������ദ����ɣ�Ȼ�󽫴���Ȩ�����ҿؼ�
                while(polygons.size() > 0 )
                {
                    PolygonDesc * topDesc = polygons.at(polygons.size() -1);
                    PolygonDesc * parent = NULL;
                    //���������״̬
                    if(topDesc->isProcLeft && !topDesc->isLeftOver)
                    {
                        topDesc->isLeftOver = true;
                        topDesc->isProcLeft = false;
                        topDesc->isProcRight = true;

                        currItem = topDesc->rightItem;
                        frontUnit = topDesc->processUnit;

                        break;
                    }
                    //�Ҳ�δ�����꣬����ҿؼ��������
                    else if(topDesc->isProcRight && !topDesc->isRightOver)
                    {
                        topDesc->isRightOver = true;
                        topDesc->isProcRight = false;
                    }

                    if(topDesc->isLeftOver && topDesc->isLeftOver)
                    {
                        parent = topDesc->parent;
                        delete topDesc;
                        polygons.pop();             //��������
                    }

                    //���ø��ؼ���״̬
                    if(parent && !parent->isLeftOver)
                    {
                        parent->isProcLeft = false;
                        parent->isLeftOver = true;
                        parent->isProcRight = true;

                        currItem = parent->rightItem;

                        frontUnit = parent->processUnit;
                        break;
                    }
                    else if(parent && !parent->isRightOver)
                    {
                        parent->isProcRight = false;
                        parent->isRightOver = true;
                    }
                }

                if(polygons.size() == 0)
                {
                    isAtEnd = true;
                    procUnits.push_back(endUnit);
                }
            }
            else
            {
                isAtEnd = true;
                procUnits.push_back(endUnit);
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
                    return FLOW_ERROR;
                }

                //��ǰ�ؼ�����һ���ڵ�
                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                if(outItems.size() == 1)
                {
                    ProcessUnit  * unit = new ProcessUnit;
                    unit->gtype = gtype;
                    unit->item = currItem;

                    ProcessType  ptype;

                    switch (gtype)
                    {
                        case GRA_RECT:
                                       ptype = PRO_PROCESS;
                                       break;
                        case GRA_PARALLELOGRAM:
                                       ptype = PRO_INPUT;
                                       break;
                        case GRA_LOOP_UP:
                                       ptype = PRO_LOOP_UP;
                                       break;
                        case GRA_LOOP_DOWN:
                                       ptype = PRO_LOOP_DOWN;
                                       break;
                        case GAR_PARALLE:
                                       ptype = PRO_PARALLEL;
                                       break;
                        default:
                            break;
                    }

                    unit->ptype = ptype;

                    //�������жϿ���Ҫ�����˿ؼ����õ��жϿ�����ҷ�֧��
                    if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
                    {
                        PolygonDesc * currDesc = polygons.at(polygons.size() - 1);

                        if(currDesc->isProcLeft)
                        {
                            frontUnit->yesChild = unit;
                        }
                        else if(currDesc->isProcRight)
                        {
                            frontUnit->noChild = unit;
                        }
                    }
                    else
                    {
                        frontUnit->nextChild = unit;
                    }

                    frontUnit = unit;

                    procUnits.push_back(unit);

                    currItem = outItems.at(0);
                }
            }
            //�ж�[1�����룬2�����]
            else if(gtype == GRA_POLYGON)
            {
                if(inNum !=2 || outNum != 1)
                {
                    return FLOW_ERROR;
                }

                PolygonDesc * desc = new PolygonDesc;
                desc->currItem = currItem;

                ProcessUnit  * unit = new ProcessUnit;
                unit->gtype = gtype;
                unit->item = currItem;
                unit->ptype = PRO_JUDGE;

                if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
                {
                    PolygonDesc * currDesc = polygons.at(polygons.size() - 1);

                    if(currDesc->isProcLeft)
                    {
                        frontUnit->yesChild = unit;
                    }
                    else if(currDesc->isProcRight)
                    {
                        frontUnit->noChild = unit;
                    }
                }
                else
                {
                    frontUnit->nextChild = unit;
                }

                frontUnit = unit;

                procUnits.push_back(unit);

                desc->processUnit = unit;

                //���������жϿ�ʱ����Ҫ�������жϿ��ջ�����жϿ�Ĺ�ϵ
                if(polygons.size() > 0)
                {
                    PolygonDesc * currDesc = polygons.at(polygons.size() - 1);
                    desc->parent = currDesc;
                }

                polygons.push(desc);

                //��ǰ�ؼ�����һ���ڵ�
//                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                //�ֱ��ȡ���������Ͳ�������item������������item��ֵleft��������item��ֵright
                MyItem * yesItem = getConditionItem(currItem,arrows,true);
                MyItem * noItem = getConditionItem(currItem,arrows,false);

                desc->isProcLeft = true;
                desc->leftItem = yesItem;
                desc->rightItem = noItem;
                currItem = yesItem;
            }
        }
    }
    //[2]��֤�߼��Ƿ���ȷ
    return RETURN_SUCCESS;
}

//�ֱ��ȡ���������Ͳ�������item
MyItem * ProcessCheck::getConditionItem(MyItem * currItem,QList<MyArrow *> &arrows, bool isYes)
{
    foreach(MyArrow * arrow,arrows)
    {
        MyItem * item = getMyItem(arrow,true);
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

