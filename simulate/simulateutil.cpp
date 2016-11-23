#include "simulateutil.h"

#include "../item/myitem.h"
#include "../item/myarrow.h"
#include "../item/draglinepoint.h"

#include <QStack>

#include <QDebug>

SimulateUtil * SimulateUtil::util = NULL;

SimulateUtil::SimulateUtil()
{
    relationList<<">"<<">="<<"=="<<"!="<<"<"<<"<=";
    logicalList<<"&&"<<"||"<<"!";
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
 *��ȡ��ǰ�ؼ��ĸ��ؼ���ֻ���Ǹ��ؼ�Ϊ�����(GRA_RECT).
 *��1��20161101���Ӷ�ѭ����������������
 *��2��20161107�޸���dowhile������rect����ѭ������(��δ���жϿ��ܵ�����(�ж�/ѭ��))
 *��3��20161117���Ӷ��ж�ѭ�������������
 *!*/
QList<MyItem *> SimulateUtil::getCurrParentItem(MyItem * item)
{
    QList<MyItem *> list;

    MyItem * pItem = item;
    bool isFirst = true;                  //�Ƿ��һ��ѭ����������pItem==Item�����жϳ�������

    QStack<MultiPathDesc * > descs;       //��¼���ݹ�����ĳ���ؼ�������������Ķ�·��
    QStack<MyItem * > polygons;           //��¼�Ƶ������������Ķ����(�������жϣ�Ҳ������ѭ��)
    MultiPathDesc * currMPath = NULL;

    bool isSearchOver = false;

    if(item->getType())
    {
        while(pItem&&!isSearchOver)
        {
            QList<MyArrow *> arrows = pItem->getArrows();
            QList<MyItem *> pItems = getInOutItems(pItem,arrows,false);

//            qDebug()<<pItem->getText();

            bool hasAdded = false;
            bool skipCurrLoop = false;

            if(pItem->getType() == GRA_POLYGON)
            {
                bool hasAddPolygon = false;

                if(!isFirst)
                {
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

//                        qDebug()<<"===add:"<<pItem->getText();
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
//                                    qDebug()<<"===remve:"<<list.last()->getText();
                                    list.removeLast();
                                }

                                spathDesc->isSearching = false;
                                spathDesc->hasSearched = true;

                                lastDesc->currPathNum += 1;
                                //��ǰ�Ѿ���������,
                                if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                                {
                                    foreach(SignalPathDesc* tmpSignalPath,lastDesc->pathes)
                                    {
                                        delete tmpSignalPath;
                                    }
                                    lastDesc->pathes.clear();
                                    descs.pop();
                                }
                                else
                                {
                                    SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                                    pItem = spathDesc->startItem;
                                    skipCurrLoop = true;
//                                    qDebug()<<"+++"<<pItem->getText();
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
                            foreach(SignalPathDesc* tmpSignalPath,lastDesc->pathes)
                            {
                                delete tmpSignalPath;
                            }
                            lastDesc->pathes.clear();
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
//                                qDebug()<<"===remve:"<<list.last()->getText();
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

//�Ƿ�Ϊ��ϵ�����
bool SimulateUtil::isRelationOperation(QString text)
{
    return relationList.contains(text);
}

//�Ƿ�Ϊ�߼������
bool SimulateUtil::isLogicalOperation(QString text)
{
    return logicalList.contains(text);
}

/**
 *����״̬���ʷ������ж�����
 *��1���滻���ڵĿո�
 *��2��������ַ������з���
 */
bool SimulateUtil::parseText(QString text,QStringList & result)
{
    //��1��
    QRegExp exp("\\s+");
    QString nonWhileSpace = text.replace(exp,"");

    //��2��
    CharacterType ctype = CHAR_START;        //��һ�ε��ַ�����״̬
    QString hasChar;
    QChar ch;
    bool isRight = true;
    for(int index = 0;  index < nonWhileSpace.length() && isRight; index ++)
    {
        ch = nonWhileSpace.at(index);

        switch(ctype)
        {
            case CHAR_START:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' || ch == '&' || ch == '|' || ch == ')')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else if(ch == '!' )
                                {
                                    hasChar.append(ch);
                                    if(index + 1 < nonWhileSpace.length())
                                    {
                                        QChar nextChar = nonWhileSpace.at(index+1);
                                        if(nextChar == '>' || nextChar == '=' || nextChar == '<' || nextChar == '&' || nextChar == '|' || nextChar == ')')
                                        {
                                            ctype = CHAR_ILLEGAL;
                                        }
                                        else
                                        {
                                            ctype = CHAR_OPPOSE;
                                        }
                                    }
                                }
                                else if(ch == '(')
                                {
                                    ctype = CHAR_LEFT_AGGRE;
                                }
                                else
                                {
                                    hasChar.append(ch);
                                    ctype = CHAR_NORMAL;
                                }
                            }
                            break;
            case CHAR_NORMAL:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' )
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_RELATION;
                                }
                                else if( ch == '!' || ch == '&' || ch == '|')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_LOGICAL;
                                }
                                else if(ch == '(')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else if(ch == ')')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_RIGHT_AGGRE;
                                }
                                else
                                {
                                    hasChar.append(ch);
                                    ctype = CHAR_NORMAL;
                                }
                            }
                            break;
            case CHAR_RELATION:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' || ch == '!' )
                                {
                                    hasChar.append(ch);
                                    if(!isRelationOperation(hasChar))
                                    {
                                        if(ch == '!')
                                        {
                                            hasChar.remove(hasChar.size() - 1);
                                            result.append(hasChar);
                                            hasChar.clear();
                                            hasChar.append(ch);
                                            ctype = CHAR_OPPOSE;
                                            break;
                                        }
                                        else
                                        {
                                            ctype = CHAR_ILLEGAL;
                                            break;
                                        }
                                    }
                                    ctype = CHAR_RELATION;
                                }
                                else if(ch == '(')
                                {
                                    if(isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_LEFT_AGGRE;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else if(ch == ')' || ch == '&' || ch == '|' || ch =='!')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else
                                {
                                    if(isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_NORMAL;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                            }
                            break;
            case CHAR_LOGICAL:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' )
                                {
                                    if(ch == '=' && hasChar == "!")
                                    {
                                        hasChar.append(ch);
                                        ctype = CHAR_LOGICAL;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else if(ch == '(')
                                {
                                    if(isLogicalOperation(hasChar)||isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_LEFT_AGGRE;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else if(ch == '&' || ch == '|' || ch =='!')
                                {
                                    hasChar.append(ch);
                                    if(isLogicalOperation(hasChar))
                                    {
                                        ctype = CHAR_LOGICAL;
                                    }
                                    else
                                    {
                                        //�޸�&&!2�����
                                        QString beforeString = hasChar.remove(hasChar.length()-1,1);
                                        if(isLogicalOperation(beforeString) && ch == '!')
                                        {
                                            result.append(hasChar);
                                            hasChar.clear();
                                            hasChar.append(ch);
                                            ctype = CHAR_OPPOSE;
                                            break;
                                        }
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else if(ch == ')' )
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else
                                {
                                    if(isLogicalOperation(hasChar)||isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_NORMAL;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                            }
                            break;
            case CHAR_LEFT_AGGRE:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' ||ch == ')' || ch == '&' || ch == '|')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else if(ch == '!')
                                {
                                    if(index + 1 < nonWhileSpace.length())
                                    {
                                        QChar nextChar = nonWhileSpace.at(index+1);
                                        if(nextChar == '>' || nextChar == '=' || nextChar == '<' || nextChar == '&' || nextChar == '|' || nextChar == ')')
                                        {
                                            ctype = CHAR_ILLEGAL;
                                        }
                                        else
                                        {
                                            result.append(hasChar);
                                            hasChar.clear();
                                            hasChar.append(ch);
                                            ctype = CHAR_OPPOSE;
                                        }
                                    }
                                }
                                else if(ch == '(')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_LEFT_AGGRE;
                                }
                                else
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_NORMAL;
                                }
                            }
                            break;
            case CHAR_RIGHT_AGGRE:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' )
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_RELATION;
                                }
                                else if(ch == '&' || ch == '|')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_LOGICAL;
                                }
                                else if(ch == ')')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_RIGHT_AGGRE;
                                }
                                else
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                            }
                            break;
            case CHAR_OPPOSE:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' || ch == '&' || ch== '|' || ch==')')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else if(ch == '(')
                                {
                                    if(isLogicalOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_LEFT_AGGRE;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else
                                {
                                    if(isLogicalOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_NORMAL;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                            }
                break;
            case CHAR_ILLEGAL:
                            {
                                isRight = false;
                            }
                            break;
        }
    }

    if(ctype == CHAR_RIGHT_AGGRE)
    {
        result.append(")");
    }
    else if(ctype == CHAR_NORMAL)
    {
        result.append(hasChar);
    }
    else
    {
        return false;
    }
    return true;
}

//����ʷ������������ж�����
bool SimulateUtil::getResult(QStringList &wordList)
{
    bool correct = true;
    int  index = 0;
    QVariant finalResult = calcLogical(wordList,index,correct);

    return finalResult.toBool();
}

//�߼�����
QVariant SimulateUtil::calcLogical(QStringList & result,int &index,bool & correct)
{
    QVariant tmpResult1 = calcRelation(result,index,correct);
    QString textA;
    while(index < result.size())
    {
        textA = result.at(index);
        index++;

        if(textA != "&&" && textA != "||" && textA != "!")
        {
            index--;
            return tmpResult1;
        }
        QVariant tmpResult2 = calcRelation(result,index,correct);
        if(correct)
        {
            bool tmpA = tmpResult1.toBool();
            bool tmpB = tmpResult2.toBool();
            if(textA == "&&")
            {
                tmpResult1 = (tmpA && tmpB);
            }
            else if(textA == "||")
            {
                tmpResult1 = (tmpA || tmpB);
            }
            else if(textA == "!")
            {
                tmpResult1 = !tmpA;
            }
        }
    }

    return tmpResult1;
}

//���й�ϵ����
QVariant SimulateUtil::calcRelation(QStringList & result, int &index, bool & correct)
{
    QVariant tmpResult1 = getNext(result,index,correct);
    QString textA;
    while(index < result.length())
    {
        textA = result.at(index);
        index++;
        if(!isRelationOperation(textA))
        {
            index--;
            return tmpResult1;
        }

        QVariant tmpResult2 = getNext(result,index,correct);
        if(correct)
        {
            double result1 = tmpResult1.toDouble();
            double result2 = tmpResult2.toDouble();
            if(textA == ">")
            {
                tmpResult1 = (result1 > result2);
            }
            else if(textA == ">=")
            {
                tmpResult1 = (result1 >= result2);
            }
            else if(textA == "==")
            {
                tmpResult1 = (result1 == result2);
            }
            else if(textA == "!=")
            {
                tmpResult1 = (result1 != result2);
            }
            else if(textA == "<")
            {
                tmpResult1 = (result1 < result2);
            }
            else if(textA == "<=")
            {
                tmpResult1 = (result1 <= result2);
            }
        }
        else
        {
            return QVariant(0);
        }
    }

    return tmpResult1;
}

//����
QVariant SimulateUtil::getNext(QStringList & result,int &index,bool & correct)
{
    QVariant tmpResult = false;
    QString text;
    if(index < result.size())
    {
        text = result.at(index);
        index++;

        if( text == "(")
        {
            tmpResult = calcRelation(result,index,correct);
            text = result.at(index);
            index++;
            if(text == ")")
            {
                correct = false;
                return QVariant(false);
            }
            else
            {
                return tmpResult;
            }
        }
    }

    return QVariant(text);
}
