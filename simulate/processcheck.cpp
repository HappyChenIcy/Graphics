#include "processcheck.h"

#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../item/myitem.h"
#include "../item/myarrow.h"
#include "../item/draglinepoint.h"
#include "simulateutil.h"

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
 *[4]:
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
    QStack<PolygonDesc *> loopPolygons;   //���ݹ�����������ѭ���жϿ�
    QStack<ProcessUnit *> doWhileRects;   //���ݹ���������RECTΪ����Ľ���һ�����϶�Ϊdowhileѭ��

    ProcessUnit * frontUnit = NULL;       //ǰһ�����Ƶ�Ԫ

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

//        qDebug()<< currItem->getText();

        //��ʼ��
        if(currItem == startItem)
        {
            if(arrows.size() == 0 || arrows.size() > 1)
            {
                return FLOW_ERROR;
            }

            currItem = SimulateUtil::instance()->getMyItem(arrows.at(0));

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

            if(polygons.size() > 0 && frontUnit)
            {
                if(frontUnit->ptype == PRO_JUDGE)
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
                else if(frontUnit->ptype == PRO_LOOP)
                {
                    if(loopPolygons.size() > 0)
                    {
                        PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);
                        if(currDesc->isProcLeft)
                        {
                            frontUnit->yesChild = endUnit;
                        }
                        else if(currDesc->isProcRight)
                        {
                            frontUnit->noChild = endUnit;
                        }
                    }
                }
                else
                {
                    frontUnit->nextChild = endUnit;
                }

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
            }
            else if(loopPolygons.size() >0 && frontUnit)
            {
                if(frontUnit->ptype == PRO_LOOP)
                {
                    PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);

                    if(currDesc->isProcLeft)
                    {
                        currDesc->isProcLeft = false;
                        frontUnit->yesChild = endUnit;
                        currDesc->isLeftOver = true;
                        currDesc->isProcRight = true;
                    }
                    else if(currDesc->isProcRight)
                    {
                        frontUnit->noChild = endUnit;
                        currDesc->isRightOver = true;
                    }
                }
                else
                {
                    frontUnit->nextChild = endUnit;
                }

                //���δ������ɣ������ദ����ɣ�Ȼ�󽫴���Ȩ�����ҿؼ�
                while(loopPolygons.size() > 0 )
                {
                    PolygonDesc * topDesc = loopPolygons.at(loopPolygons.size() -1);
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
                        loopPolygons.pop();             //��������
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
            }
            else
            {
                frontUnit->nextChild = endUnit;
            }

            if(polygons.size() == 0 && loopPolygons.size() == 0)
            {
                isAtEnd = true;
                procUnits.push_back(endUnit);
            }
        }
        else
        {
            GraphicsType gtype = currItem->getType();

            int inNum = 0,outNum = 0;               //��ǰ�ؼ��ֱ���Ϊ�����յ�����ֱ�ߵĸ���
            SimulateUtil::instance()->getItemInOutNum(currItem,arrows,inNum,outNum);

            //���������/�����[��ע��ͷָ���]
            if(gtype == GRA_PARALLELOGRAM || gtype == GRA_RECT)
            {
                if(inNum !=1 || outNum < 1)
                {
                    return FLOW_ERROR;
                }

                //��ǰ�ؼ�����һ���ڵ�
                QList<MyItem *> outItems = SimulateUtil::instance()->getInOutItems(currItem,arrows,true);

                if(outItems.size() == 1)
                {
                    ProcessType  ptype;

                    switch (gtype)
                    {
                        case GRA_RECT:
                                        ptype = PRO_PROCESS;
                                        break;
                        case GRA_PARALLELOGRAM:
                                       ptype = PRO_INPUT;
                                       break;
                                       break;
                        case GAR_PARALLE:
                                       ptype = PRO_PARALLEL;
                                       break;
                        default:
                            break;
                    }

                    ProcessUnit  * unit = new ProcessUnit;
                    unit->gtype = gtype;
                    unit->item = currItem;
                    unit->ptype = ptype;

                    bool isDoWhile = false;

                    //��������򣬲����ж�����룬����ʱ�϶�Ϊѭ����������
                    if(ptype == PRO_PROCESS && outNum > 1)
                    {
                        bool isExisted = false;
                        foreach (ProcessUnit * punit, doWhileRects)
                        {
                            if(punit->item == currItem)
                            {
                                isExisted = true;
                                unit = punit;
                                break;
                            }
                        }

                        if(!isExisted)
                        {
                            doWhileRects.append(unit);
                        }

                        //�жϿ���yes��֧
                        if(frontUnit->ptype == PRO_LOOP && isExisted &&loopPolygons.size()>0)
                        {
                            PolygonDesc * topLoop = loopPolygons.at(loopPolygons.size()-1);
                            if(topLoop->isProcLeft)
                            {
                                topLoop->isProcLeft = false;
                                topLoop->isLeftOver = true;
                                topLoop->isProcRight = true;

                                topLoop->processUnit->yesChild = unit;
                                currItem = topLoop->rightItem;
                                isDoWhile = true;
                            }
                        }
                    }

                    if(!isDoWhile)
                    {
                        //�������жϿ���Ҫ�����˿ؼ����õ��жϿ�����ҷ�֧��
                        if(frontUnit->gtype == GRA_POLYGON)
                        {
                            PolygonDesc * currDesc = NULL;
                            if(frontUnit->ptype == PRO_JUDGE && polygons.size() > 0)
                            {
                                currDesc = polygons.at(polygons.size() - 1);
                            }
                            else if(frontUnit->ptype == PRO_LOOP && loopPolygons.size() >0)
                            {
                                currDesc = loopPolygons.at(loopPolygons.size() - 1);
                            }

                            if(currDesc)
                            {
                                if(currDesc->isProcLeft)
                                {
                                    frontUnit->yesChild = unit;
                                }
                                else if(currDesc->isProcRight)
                                {
                                    frontUnit->noChild = unit;
                                }
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
            }
            //[1������(�ж�)/2������(ѭ��)��2�����]
            //����Ϊdowhileѭ��ʱ�����ܻ������if���������غ��������Ҫ��һ���жϡ��ǡ��ķ�֧�Ƿ�ָ��ǰ�������doWhileRects
            else if(gtype == GRA_POLYGON)
            {
                if(inNum !=2 || outNum < 1)
                {
                    return FLOW_ERROR;
                }

                //�ֱ��ȡ���������Ͳ�������item������������item��ֵleft��������item��ֵright
                MyItem * yesItem = SimulateUtil::instance()->getConditionItem(currItem,arrows,true);
                MyItem * noItem = SimulateUtil::instance()->getConditionItem(currItem,arrows,false);

                if(yesItem == NULL || noItem == NULL)
                {
                    return FLOW_ERROR;
                }

                bool isYesItemProcced = false;
                ProcessUnit * yesProcessUnit = NULL;

                //���yes��ָ֧���rect��֮ǰ��doWhileRects���ڣ���������ѭ��
                foreach(ProcessUnit * unit,doWhileRects)
                {
                    if(unit->item == yesItem)
                    {
                        isYesItemProcced = true;
                        yesProcessUnit = unit;
                        break;
                    }
                }

                //�жϴ���
                if(!isYesItemProcced && outNum == 1)
                {
                    PolygonDesc * desc = new PolygonDesc;
                    desc->currItem = currItem;

                    ProcessUnit  * unit = new ProcessUnit;
                    unit->gtype = gtype;
                    unit->item = currItem;
                    unit->ptype = PRO_JUDGE;

                    if(frontUnit->gtype == GRA_POLYGON)
                    {
                        PolygonDesc * currDesc = NULL;
                        if(frontUnit->ptype == PRO_JUDGE && polygons.size() > 0)
                        {
                            currDesc = polygons.at(polygons.size() - 1);
                        }
                        else if(frontUnit->ptype == PRO_LOOP && loopPolygons.size() >0)
                        {
                            currDesc = loopPolygons.at(loopPolygons.size() - 1);
                        }

                        if(currDesc)
                        {
                            if(currDesc->isProcLeft)
                            {
                                frontUnit->yesChild = unit;
                            }
                            else if(currDesc->isProcRight)
                            {
                                frontUnit->noChild = unit;
                            }
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

                    desc->isProcLeft = true;
                    desc->leftItem = yesItem;
                    desc->rightItem = noItem;

                    currItem = yesItem;
                }
                //ѭ������
                else if(isYesItemProcced || outNum > 1)
                {
                    //�ж�ѭ��ջ�Ƿ�����˵�ǰѭ���ؼ���
                    bool hasExisted = false;
                    PolygonDesc * existedDesc = NULL;
                    foreach(PolygonDesc * tmpDesc,loopPolygons)
                    {
                        if(tmpDesc->currItem == currItem)
                        {
                            hasExisted = true;
                            existedDesc = tmpDesc;

                            //ǰһ����Ԫ��ѭ��[˫��ѭ��ʱ�����ѭ�����Ƚ��������ѭ��δ��������ʱҪ�������ĵ���������Ӱ�����]
                            if(frontUnit->ptype == PRO_LOOP)
                            {
                                PolygonDesc * frontDesc = NULL;
                                for(int i = 0; i<loopPolygons.size();i++)
                                {
                                    if(loopPolygons.at(i)->currItem == frontUnit->item)
                                    {
                                        frontDesc = loopPolygons.at(i);
                                        break;
                                    }
                                }

                                if(frontDesc && frontDesc->isProcLeft)
                                {
                                    frontUnit->yesChild = tmpDesc->processUnit;
                                    frontDesc->isLeftOver = true;
                                    frontDesc->isProcLeft = false;

                                    frontDesc->isProcRight = true;

                                    currItem = frontDesc->rightItem;
                                }
                                else if(frontDesc && frontDesc->isProcRight)
                                {
                                    frontUnit->noChild = tmpDesc->processUnit;

                                    frontDesc->isRightOver = true;
                                    frontDesc->isProcRight = false;
                                }
                            }
                            else if(frontUnit->ptype == PRO_JUDGE )
                            {
                                if(polygons.size() >=1)
                                {
                                    PolygonDesc * topPolygon = polygons.at(polygons.size() -1);

                                    if(topPolygon->isProcLeft)
                                    {
                                        frontUnit->yesChild = tmpDesc->processUnit;
                                    }
                                    else if(topPolygon->isProcRight)
                                    {
                                        frontUnit->noChild = tmpDesc->processUnit;
                                    }
                                }
                            }
                            else
                            {
                                frontUnit->nextChild = tmpDesc->processUnit;
                            }
                            break;
                        }
                    }

                    //Ƕ��ѭ��ʱ���ڲ�ѭ���Ƚ��������δ��������Ҫ�����ڲ�ѭ�����Ѿ���ɵĵ���
                    for(int i = loopPolygons.size() - 1; i>=0;i--)
                    {
                        if(loopPolygons.at(i)->isLeftOver && loopPolygons.at(i)->isRightOver)
                        {
                            delete loopPolygons.at(i);
                            loopPolygons.remove(i);
                        }
                    }

                    //���Ѵ��ڵ�ѭ��״̬�����޶�
                    if(hasExisted)
                    {
                        //��ջ����ѭ��״̬��������
                        if(existedDesc->isProcLeft)
                        {
                            //˫��ѭ��+�ж�
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
                                    while(loopPolygons.size() > 0)
                                    {
                                        PolygonDesc * topLoop = loopPolygons.at(loopPolygons.size() -1 );
                                        if(topLoop->isProcLeft)
                                        {
                                            topLoop->isProcLeft = false;
                                            topLoop->isLeftOver = true;
                                            topLoop->isProcRight = true;

                                            currItem = topLoop->rightItem;
                                            frontUnit = topLoop->processUnit;
                                            break;
                                        }
                                        else if(topLoop->isProcRight)
                                        {
                                            topLoop->isProcRight = false;
                                            topLoop->isRightOver = true;
                                        }

                                        PolygonDesc * parent = NULL;

                                        if(topLoop->isLeftOver && topLoop->isLeftOver)
                                        {
                                            parent = topLoop->parent;
                                            delete topLoop;
                                            loopPolygons.pop();             //��������
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
                                }
                            }
                            else
                            {
                                existedDesc->isProcLeft = false;
                                existedDesc->isLeftOver =  true;
                                existedDesc->isProcRight = true;

                                frontUnit = existedDesc->processUnit;
                                currItem = existedDesc->rightItem;
                            }
                        }
                        else if(existedDesc->isProcRight)
                        {
                            existedDesc->isProcRight = false;
                            existedDesc->isRightOver = true;
                        }
                    }

                    //�����µ�ѭ����֧
                    if(!hasExisted)
                    {
                        PolygonDesc * desc = new PolygonDesc;
                        desc->currItem = currItem;

                        ProcessUnit  * unit = new ProcessUnit;
                        unit->gtype = gtype;
                        unit->item = currItem;
                        unit->ptype = PRO_LOOP;

                        if(loopPolygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_LOOP)
                        {
                            PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);

                            if(currDesc->isProcLeft)
                            {
                                frontUnit->yesChild = unit;
                            }
                            else if(currDesc->isProcRight)
                            {
                                frontUnit->noChild = unit;
                            }
                        }
                        else if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
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
                        if(loopPolygons.size() > 0)
                        {
                            PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);
                            desc->parent = currDesc;
                        }

                        loopPolygons.push(desc);

                        desc->isProcLeft = true;
                        desc->leftItem = yesItem;
                        desc->rightItem = noItem;

                        //���ѭ��״̬�£�yes��ָ֧����doWhileRects�����е�һԱ����ô�Զ��϶���֧�������
                        if(isYesItemProcced && yesProcessUnit)
                        {
                            desc->isProcLeft = false;
                            desc->isLeftOver = true;
                            desc->isProcRight = true;

                            currItem = desc->rightItem;
                            desc->processUnit->yesChild = yesProcessUnit;
                        }
                        else
                        {
                            currItem = yesItem;
                        }
                    }
                }
            }
        }
    }
    //[2]��֤�߼��Ƿ���ȷ
    return RETURN_SUCCESS;
}
