#include "myscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QKeyEvent>
#include <QDebug>

#include "global.h"
#include "myitem.h"
#include "myarrow.h"
#include "mytextitem.h"
#include "mypathitem.h"
#include "mynodeport.h"
#include "myiteminfo.h"
#include "draglinepoint.h"
#include "../util.h"

#include "typeinfo.h"

MyScene::MyScene(QMenu *menu, QObject * parent):
    rightMenu(menu),
    QGraphicsScene(parent)
{
    CurrAddGraType =  GRA_NONE;
    insertTmpLine = NULL;
    insertTmpPath = NULL;
    isLocalFileOpened = false;
    isDragLine = false;
    isClear = false;
    myItemInfo = NULL;
    createItemInfo();

    connect(this,SIGNAL(selectionChanged()),this,SLOT(itemSelectedChanged()));

    setBackgroundBrush(QPixmap(":/images/backgroundRole.png"));
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SceneLastClickPoint = event->scenePos();

    if(event->button () == Qt::LeftButton)
    {
        if(itemAt(event->scenePos()) && TYPE_ID(*itemAt(event->scenePos())) == TYPE_ID(DragLinePoint))
        {
            insertTmpLine = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
            insertTmpLine->setPen(QPen(Qt::red, 2));
            insertTmpLine->setZValue(1000);
            addItem(insertTmpLine);
            isDragLine = true;
        }

        if(!isDragLine && CurrAddGraType == GRA_LINE)
        {
            insertTmpLine = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
            insertTmpLine->setPen(QPen(Qt::red, 2));
            addItem(insertTmpLine);
        }
        else if(CurrAddGraType == GRA_TEXT)
        {
            MyTextItem  * item = new MyTextItem(CurrAddGraType,rightMenu);
            connect(item,SIGNAL(textLostFocus(MyTextItem *)),this,SLOT(respTextLostFocus(MyTextItem *)));
//            connect(item,SIGNAL(posHasChanged(MyRect)),this,SIGNAL(selectedItemPosChanged(MyRect)));

            item->setPos(event->scenePos());
            addItem(item);
        }
        else if(CurrAddGraType == GRA_VECTOR_LINE)
        {
//            insertTmpPath = new MyPathItem;
//            insertTmpPath->setPen(QPen(Qt::blue,2));
//            insertTmpPath->setPos(event->screenPos());
//            addItem(insertTmpPath);
        }
        else if(CurrAddGraType != GRA_NONE)
        {
            addItem(CurrAddGraType,event->scenePos());
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        CurrAddGraType = GRA_NONE;
        emit resetItemAction();
    }

    if(CurrAddGraType != GRA_NONE && CurrAddGraType != GRA_LINE && CurrAddGraType != GRA_VECTOR_LINE)
    {
        CurrAddGraType = GRA_NONE;
        emit resetItemAction();
    }

    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    if(itemAt(event->scenePos()))
//    {
//        qDebug()<<TYPE_ID(*itemAt(event->scenePos()));
//    }

    if((isDragLine||CurrAddGraType == GRA_LINE)&& insertTmpLine)
    {
        QLineF newLine(insertTmpLine->line().p1(), event->scenePos() /*- QPointF(2,2)*/);
        insertTmpLine->setLine(newLine);
    }
    else if(CurrAddGraType == GRA_VECTOR_LINE && insertTmpPath)
    {
        QPainterPath path;
        path.moveTo(0,0);
        path.lineTo(event->scenePos());
        insertTmpPath->setPath(path);
    }
    else
    {
        if(selectedItems().size() == 0)
        {

        }

        QGraphicsScene::mouseMoveEvent(event);
    }    
}

//����ק����ֱ��ʱ��������갴�ڿؼ���Text�ϣ���ô��Ҫ��һ�����ж�
void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(CurrAddGraType == GRA_LINE && insertTmpLine)
    {
        QList<QGraphicsItem *> startItems = items(insertTmpLine->line().p1());
        if (startItems.count() && startItems.first() == insertTmpLine)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items(insertTmpLine->line().p2());
        if (endItems.count() && endItems.first() == insertTmpLine)
        {
            endItems.removeFirst();
        }
        removeItem(insertTmpLine);
        delete insertTmpLine;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first())
        {
            QString firstItemId = TYPE_ID(* startItems.first());
            QString secondItemId = TYPE_ID(* endItems.first());

            MyItem *startItem = qgraphicsitem_cast<MyItem *>(startItems.first());
            MyItem *endItem = qgraphicsitem_cast<MyItem *>(endItems.first());

            if(firstItemId == TYPE_ID(MyTextItem) && startItems.size() > 1)
            {
                firstItemId = TYPE_ID(* startItems.at(1));
                startItem = qgraphicsitem_cast<MyItem *>(startItems.at(1));
            }

            if(secondItemId == TYPE_ID(MyTextItem) && endItems.size() > 1)
            {
                secondItemId = TYPE_ID(* endItems.at(1));
                endItem = qgraphicsitem_cast<MyItem *>(endItems.at(1));
            }

            if(firstItemId == TYPE_ID(MyItem) && secondItemId == TYPE_ID(MyItem) &&
                    startItem && endItem)
            {
                MyArrow *arrow = new MyArrow(startItem, endItem);
                connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                addItem(arrow);
                arrow->updatePosition();
            }
            else if(firstItemId == TYPE_ID(MyNodePort) && secondItemId == TYPE_ID(MyNodePort))
            {
                MyNodePort *startItem = qgraphicsitem_cast<MyNodePort *>(startItems.first());
                MyNodePort *endItem = qgraphicsitem_cast<MyNodePort *>(endItems.first());

                if(startItem && endItem)
                {
                    MyArrow *arrow = new MyArrow(startItem, endItem);
                    connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                    connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

                    startItem->addArrow(arrow);
                    endItem->addArrow(arrow);
                    arrow->setZValue(-1000.0);
                    addItem(arrow);
                    arrow->updatePosition();
                }
            }
        }
    }
    else if(CurrAddGraType == GRA_VECTOR_LINE && insertTmpPath)
    {

    }

    insertTmpLine = NULL;
    insertTmpPath = NULL;
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        emit deleteKeyPress();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_L)
    {
        emit ctrlLockKeyPress();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_L)
    {
        emit ctrlUnLockKeyPress();
    }
    QGraphicsScene::keyPressEvent(event);
}

//����ӿؼ�
void MyScene::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);

    emit itemSizeChanged(items().size());
}

///*****************************************************
///**Function:
///**Description:1:���ڸ��ơ�����ʱ����.2:���ڱ��ش�ʱ����
///**Input:
///**Output:
///**Return:
///**Others:
///****************************************************/
void MyScene::addItem(CutInfo cutInfo, bool isCopy)
{
    if(cutInfo.graphicsType == GRA_TEXT)
    {
        MyTextItem  * item = new MyTextItem(cutInfo.graphicsType,rightMenu);
        addMyTextConnect(item);
//        connect(item,SIGNAL(posHasChanged(MyRect)),this,SIGNAL(selectedItemPosChanged(MyRect)));

        item->setTextInteractionFlags(Qt::TextEditorInteraction);

        //���Ǳ��ش򿪣���Ҫ�ֶ���Ϊÿ��ͼ��ָ��һ���������ֵ��Ϊ���´δ�ʱ����԰��ձ�����ʽ�ָ�
        if(!isLocalFileOpened)
        {
            cutInfo.itemProperty.zValue = Util::getGlobalZValue();
        }

        item->setProperty(cutInfo.itemProperty);
        if(isCopy)
        {
            item->setPos(SceneLastClickPoint);
        }
        else
        {
            item->setPos(cutInfo.itemProperty.itemRect.x,cutInfo.itemProperty.itemRect.y);
        }

        item->setPlainText(cutInfo.itemProperty.content);

        addItem(item);
    }
    else if(cutInfo.graphicsType != GRA_NONE && cutInfo.graphicsType != GRA_LINE)
    {
        MyItem * item = new MyItem(cutInfo.graphicsType,rightMenu,this);
        addMyItemConnect(item);

        if(!isLocalFileOpened)
        {
            cutInfo.itemProperty.zValue = Util::getGlobalZValue();
        }

        item->setText(cutInfo.content);
        item->setProperty(cutInfo.itemProperty);
        if(isCopy)
        {
            item->setPos(SceneLastClickPoint);
            item->setSelected(true);
            item->setDragPointVisible(true);
            item->resetItemUUID();         //����ʱ��Ҫ���¿ؼ���ID�Ž��и��¡�!!!!��
        }
        else
        {
            item->resetPolygon();
            item->setPos(QPointF(cutInfo.itemProperty.itemRect.x,cutInfo.itemProperty.itemRect.y));
        }

        item->setMoveable(cutInfo.itemProperty.isMoveable);

        //���ش�ʱ�����ڴ���ʱ������µ�ID������Ҫ�ֶ��ĸ���
        foreach(NodePortProperty prop,cutInfo.nodeProperties)
        {
            MyNodePort * tmp = item->addNodePort(prop);
            if(!isCopy)
            {
                tmp->updatePortID(prop.startItemID);
                localNodeports.push_back(tmp);
            }
        }

        //���ش�ʱ��Ҫ����
        if(!isCopy)
        {
            localItems.push_back(item);
        }

        addItem(item);
    }
    else if(cutInfo.graphicsType == GRA_LINE)
    {

        if(cutInfo.itemProperty.lineType == LINE_MYITEM)
        {
            int startIndex = findItemById(localItems,cutInfo.itemProperty.startItemID);
            int endIndex = findItemById(localItems,cutInfo.itemProperty.endItemID);

            if(startIndex>=0&&startIndex<localItems.size() &&
                    endIndex>=0&&endIndex<localItems.size())
            {
                MyItem * startItem = localItems.at(startIndex);
                MyItem * endItem = localItems.at(endIndex);

                MyArrow *arrow = new MyArrow(startItem, endItem);
                connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                arrow->setProperty(cutInfo.itemProperty);

                arrow->updatePosition();
                addItem(arrow);
            }
        }
        else if(cutInfo.itemProperty.lineType == LINE_NODEPORT)
        {
            int startIndex = findItemById(localNodeports,cutInfo.itemProperty.startItemID);
            int endIndex = findItemById(localNodeports,cutInfo.itemProperty.endItemID);
            if(startIndex>=0&&startIndex<localNodeports.size() &&
                    endIndex>=0&&endIndex<localNodeports.size())
            {

                MyNodePort * startNode = localNodeports.at(startIndex);
                MyNodePort * endNode = localNodeports.at(endIndex);

                MyArrow *arrow = new MyArrow(startNode, endNode);
                connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

                startNode->addArrow(arrow);
                endNode->addArrow(arrow);
                arrow->setZValue(-1000.0);
                arrow->setProperty(cutInfo.itemProperty);

                arrow->updatePosition();
                addItem(arrow);
            }
        }
    }
}

///*****************************************************
///**Function:
///**Description:���ش򿪱����ļ��󣬶Խ��������ݽ��н�����ʾ
///**Input:
///**Output:
///**Return:
///**Others:20160929:wey:������ֱ�ߵĽ�������ֱ�߷��������ؼ�֮�����
///****************************************************/
void MyScene::addItem(QList<CutInfo *> &cutInfos)
{
    isLocalFileOpened = true;

    QList<CutInfo *> lines;

    foreach (CutInfo * cutInfo, cutInfos)
    {
        if(cutInfo->graphicsType == GRA_LINE)
        {
            lines.push_back(cutInfo);
        }
        else
        {
            addItem(*cutInfo);
        }
    }

    foreach (CutInfo * cutInfo, lines)
    {
        addItem(*cutInfo);
    }

    isLocalFileOpened = false;
    localItems.clear();
    localNodeports.clear();
}

void MyScene::addItem(GraphicsType type, QPointF pos)
{
    if(type == GRA_TEXT)
    {
        MyTextItem  * item = new MyTextItem(type,rightMenu);
        if(!isLocalFileOpened)
        {
            item->setZValue(Util::getGlobalZValue());
        }
        addMyTextConnect(item);
        item->setPos(pos);
        item->setSelected(true);
        addItem(item);
    }
    else
    {
        MyItem * myItem = new MyItem(type,rightMenu,this);
        if(!isLocalFileOpened)
        {
            myItem->setZValue(Util::getGlobalZValue());
        }
        myItem->setPos(pos);
        addMyItemConnect(myItem);
        addItem(myItem);
    }
}

//����MyItem���źŲ۹�ϵ
void MyScene::addMyItemConnect(MyItem * item)
{
    connect(item,SIGNAL(updateSceneDraw()),this,SLOT(update()));
    connect(item,SIGNAL(propHasChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));
    connect(item,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
    connect(item,SIGNAL(itemPosChanged(MyItem *)),this,SLOT(showItemPosInfo(MyItem *)));
    connect(item,SIGNAL(itemRotationChanged(MyItem *)),this,SLOT(showItemRotationInfo(MyItem *)));
}

//����MyText���źŲ۹�ϵ
void MyScene::addMyTextConnect(MyTextItem  * item)
{
    connect(item,SIGNAL(textLostFocus(MyTextItem *)),this,SLOT(respTextLostFocus(MyTextItem *)));
}

//��Ӽ�ͷʱ���ҳ���ͷ�ĸ��ڵ�
int MyScene::findItemById(QList<MyItem *> &localItem,QString Id)
{
    int index = -1;

    for (int i=0;i<localItem.size();i++)
    {
        if(localItem.at(i)->getProperty().startItemID == Id)
        {
            index = i;
            break;
        }
    }
    return index;
}

//��Ӽ�ͷʱ���ҳ���ͷ�ĸ��˿�
int MyScene::findItemById(QList<MyNodePort *> &localNode, QString Id)
{
    int index = -1;

    for (int i=0;i<localNode.size();i++)
    {
        if(localNode.at(i)->getNodeProperty().startItemID == Id)
        {
            index = i;
            break;
        }
    }
    return index;
}

//ɾ���ӿؼ�
void MyScene::removeItem(QGraphicsItem *item)
{
    QGraphicsScene::removeItem(item);

    emit itemSizeChanged(items().size());
}

//��ʾѡ��item�ı��λ����Ϣ
void MyScene::showItemPosInfo(MyItem * item)
{
    if(!myItemInfo)
    {
        createItemInfo();
    }
    QRectF minRect = getHorizonalRoundedRect(item);
    myItemInfo->setVisible(true);
    qreal itemWidth = minRect.width();
    qreal itemHeight = minRect.height();

    myItemInfo->setPosInfo(minRect.x(),minRect.y());
    myItemInfo->setPos(minRect.x() + itemWidth/2,minRect.y() + itemHeight + 30);
}

//��ʾѡ��item�ı����ת��Ϣ
void MyScene::showItemRotationInfo(MyItem *item)
{
    if(!myItemInfo)
    {
        createItemInfo();
    }
    QRectF minRect = getHorizonalRoundedRect(item);
    myItemInfo->setVisible(true);
    qreal itemWidth = minRect.width();
    qreal itemHeight = minRect.height();

    myItemInfo->setRotationInfo(selectedItems().first()->rotation());
    myItemInfo->setPos(minRect.x() + itemWidth/2,minRect.y() + itemHeight + 30);
}

//��ʾ��ת��λ����Ϣ
void MyScene::createItemInfo()
{
    myItemInfo = new MyItemInfo(GRA_ITEM_INFO);
    myItemInfo->setVisible(false);

    addItem(myItemInfo);
}

//��ȡ��С��Χ�ؼ��ľ��Σ�����ת��boundRect�޷���ʾrect��scene�е�λ��
QRectF MyScene::getHorizonalRoundedRect(MyItem *item)
{
    QRectF rect;

    if(!item)
    {
        return rect;
    }

    QPointF p1 = item->mapToScene(item->boundingRect().topLeft().x(),item->boundingRect().topLeft().y());
    QPointF p2 = item->mapToScene(item->boundingRect().topRight().x(),item->boundingRect().topRight().y());
    QPointF p3 = item->mapToScene(item->boundingRect().bottomLeft().x(),item->boundingRect().bottomLeft().y());
    QPointF p4 = item->mapToScene(item->boundingRect().bottomRight().x(),item->boundingRect().bottomRight().y());

    qreal minX = p1.x();
    minX = qMin(minX,p2.x());
    minX = qMin(minX,p3.x());
    minX = qMin(minX,p4.x());

    qreal minY = p1.y();
    minY = qMin(minY,p2.y());
    minY = qMin(minY,p3.y());
    minY = qMin(minY,p4.y());

    qreal maxX = p1.x();
    maxX = qMax(maxX,p2.x());
    maxX = qMax(maxX,p3.x());
    maxX = qMax(maxX,p4.x());

    qreal maxY = p1.y();
    maxY = qMax(maxY,p2.y());
    maxY = qMax(maxY,p3.y());
    maxY = qMax(maxY,p4.y());

    rect.setX(minX);
    rect.setY(minY);
    rect.setWidth(maxX - minX);
    rect.setHeight(maxY - minY);
    return rect;
}

//itemѡ��״̬�ı䡾���ʱ��˿ؼ���һ���ֱ���գ������޷����ʡ�
void MyScene::itemSelectedChanged()
{
    if( !isClear && myItemInfo)
    {
        myItemInfo->setVisible(false);
    }
}

//������ֺ󣬹���Ƴ����ж������Ƿ�Ϊ�գ�Ϊ����ɾ��
void MyScene::respTextLostFocus(MyTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty())
    {
        removeItem(item);
        item->deleteLater();
    }
}

//���item�����������һ��Item(MyItemInfo)��!!!��
void MyScene::clear()
{
    isClear = true;
    QGraphicsScene::clear();
    isClear = false;

    myItemInfo = NULL;
}

MyScene::~MyScene()
{

}
