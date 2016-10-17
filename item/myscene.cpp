#include "myscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QKeyEvent>
#include <QDebug>

#include <qglobal.h>

#include "global.h"
#include "myitem.h"
#include "myarrow.h"
#include "mytextitem.h"
#include "mypathitem.h"
#include "mynodeport.h"
#include "myiteminfo.h"
#include "draglinepoint.h"
#include "../util.h"

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
        //�ӿؼ��ı��е���ק����ֱ��
        if(itemAt(event->scenePos()) && TYPE_ID(*itemAt(event->scenePos())) == TYPE_ID(DragLinePoint))
        {
            DragLinePoint * tmpDrag = dynamic_cast<DragLinePoint *>(itemAt(event->scenePos()));
            QGraphicsItem * pItem = tmpDrag->getParentItem();
            startMouseItemId = dynamic_cast<MyItem *>(pItem)->getProperty().startItemID;

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
//            insertTmpPath->setPos(event->scenePos());
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
    //���߶���ק���в������߶Σ��ڲ�������ƶ��¼�ʱ�����һֱ���£��޷������Ķ������ؼ������������¼��������Ҫ���¼����зַ�
    if((isDragLine||CurrAddGraType == GRA_LINE)&& insertTmpLine)
    {
        QLineF newLine(insertTmpLine->line().p1(), event->scenePos());
        insertTmpLine->setLine(newLine);

        QList<QGraphicsItem *> currPosItems = items(event->scenePos());
        foreach (QGraphicsItem * item, currPosItems)
        {
            if(TYPE_ID(*item) == TYPE_ID(MyItem))
            {
                mouseItems.push_back(dynamic_cast<MyItem *>(item));
                //����������λ����Item���������¼�
                QEvent * eve = new QEvent(QEvent::GraphicsSceneHoverEnter);
                sendEvent(item,eve);
            }
        }

        if(currPosItems.size() == 1)
        {
            resetItemSelection();
        }
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
    if(isDragLine)
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

            DragLinePoint *startItem = qgraphicsitem_cast<DragLinePoint *>(startItems.first());
            DragLinePoint *endItem = qgraphicsitem_cast<DragLinePoint *>(endItems.first());

            if(firstItemId == TYPE_ID(DragLinePoint) && secondItemId == TYPE_ID(DragLinePoint) &&
                    startItem && endItem)
            {
                MyArrow * arrow = createArrow(LINE_MYITEM,startItem,endItem);
                arrow->setStartPointType(startItem->getPointType());
                arrow->setEndPointType(endItem->getPointType());
            }
        }
    }
    else if(!isDragLine && CurrAddGraType == GRA_LINE && insertTmpLine)
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

            if(firstItemId == TYPE_ID(MyNodePort) && secondItemId == TYPE_ID(MyNodePort))
            {
                MyNodePort *startItem = qgraphicsitem_cast<MyNodePort *>(startItems.first());
                MyNodePort *endItem = qgraphicsitem_cast<MyNodePort *>(endItems.first());

                createArrow(LINE_NODEPORT,startItem,endItem);
            }
        }
    }
    else if(CurrAddGraType == GRA_VECTOR_LINE && insertTmpPath)
    {

    }

    isDragLine = false;
    insertTmpLine = NULL;
    insertTmpPath = NULL;
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::keyPressEvent(QKeyEvent *event)
{
    if(GlobalWindowState == WINDOW_BUILD_MODEL)
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
        else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
        {
            emit ctrlPropEditKeyPress();
        }
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
                DragLinePoint * startItem = localItems.at(startIndex)->getDragLinePoint(cutInfo.itemProperty.startPointType);
                DragLinePoint * endItem = localItems.at(endIndex)->getDragLinePoint(cutInfo.itemProperty.endPointType);

                MyArrow * arrow = createArrow(LINE_MYITEM,startItem,endItem);
                arrow->setProperty(cutInfo.itemProperty);
            }
        }
        else if(cutInfo.itemProperty.lineType == LINE_NODEPORT)
        {
            int startIndex = findItemById(localNodeports,cutInfo.itemProperty.startItemID);
            int endIndex = findItemById(localNodeports,cutInfo.itemProperty.endItemID);

            if(startIndex>=0&&startIndex<localNodeports.size() &&
                    endIndex>=0&&endIndex<localNodeports.size())
            {
                MyNodePort * startItem = localNodeports.at(startIndex);
                MyNodePort * endItem = localNodeports.at(endIndex);

                MyArrow * arrow = createArrow(LINE_NODEPORT,startItem,endItem);
                arrow->setProperty(cutInfo.itemProperty);
            }
        }
    }
}

///*****************************************************
///**Function:
///**Description:ͳһ����������
///**Input:
///**Output:
///**Return:
///**Others:
///****************************************************/
MyArrow * MyScene::createArrow(LineType type, MyNodeLine *startNode, MyNodeLine *endNode)
{
    MyArrow *arrow = new MyArrow;
    connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
    connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

    startNode->addArrow(arrow);
    endNode->addArrow(arrow);

    arrow->setStartItem(startNode);
    arrow->setEndItem(endNode);

    //���ڴ���ק���������������Ҫ�����丸���ID�������ڸ����λ��
    if( type == LINE_MYITEM)
    {
        MyItem * pStart = dynamic_cast<MyItem *>(startNode->getParentItem());
        MyItem * pEnd = dynamic_cast<MyItem *>(endNode->getParentItem());
        arrow->setStartItemID(pStart->getProperty().startItemID);
        arrow->setEndItemID(pEnd->getProperty().startItemID);
    }
    else if( type == LINE_NODEPORT)
    {
        MyNodePort * pStart = dynamic_cast<MyNodePort *>(startNode);
        MyNodePort * pEnd = dynamic_cast<MyNodePort *>(endNode);

        arrow->setStartItemID(pStart->getNodeProperty().startItemID);
        arrow->setEndItemID(pEnd->getNodeProperty().startItemID);
    }

    arrow->setZValue(-1000.0);
    arrow->setLineType(type);

    arrow->updatePosition();
    addItem(arrow);

    return arrow;
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

    foreach (CutInfo * info, cutInfos)
    {
        delete info;
    }

    myItemInfo->setVisible(false);
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

    myItemInfo->setRotationInfo(item->rotation());
    myItemInfo->setPos(minRect.x() + itemWidth/2,minRect.y() + itemHeight + 30);
}

//��ʾ��ת��λ����Ϣ
void MyScene::createItemInfo()
{
    myItemInfo = new MyItemInfo(GRA_ITEM_INFO);
    myItemInfo->setVisible(false);
    myItemInfo->setZValue(1000);

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

//�����ؼ�������ʱ�������ǰ���λ����û�пؼ����������ǰѡ�е�item
void MyScene::resetItemSelection()
{
    foreach(MyItem * item,mouseItems)
    {
        if(item && item->getProperty().startItemID != startMouseItemId)
        {
            item->setDragLineVisible(false);
        }
    }
    mouseItems.clear();
    update();
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
