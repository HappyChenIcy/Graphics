#include "mypathitem.h"

#include <QPainter>
#include <QRectF>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>

#include "myitem.h"
#include "mynodeline.h"
#include "mytextitem.h"
#include "../global.h"
#include "../manager/menumanager.h"

QDataStream & operator <<(QDataStream &stream,MyPathItem * item)
{
    int type = item->type;

    stream<<type<<item->property;

    stream<<item->getPathPoints().size();

    foreach(QPointF point,item->getPathPoints())
    {
        stream<<point;
    }

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyPathItem * item)
{
    int type;
    ItemProperty prop;
    int pointSize = 0;

    stream>>type>>prop;
    stream>>pointSize;

    QList<QPointF> points;
    for(int i = 0; i < pointSize; i++)
    {
        QPointF point;
        stream>>point;
        points.push_back(point);
    }

    item->type = (GraphicsType)type;
    item->setProperty(prop);
    item->setPathPoints(points);

    return stream;
}

MyPathItem::MyPathItem(QObject * parent, QGraphicsItem * parent1):
    QObject(parent),
    QGraphicsPathItem(parent1)
{
    setFlags(ItemIsSelectable);

    type = GRA_VECTOR_LINE;
    property.itemBrush = QBrush(Qt::black);
    property.itemPen = QPen(Qt::black,3);
    property.endLineType = LINE_SOLID_TRIANGLE;

    startItem = NULL;
    endItem = NULL;

    createTextItem();
}

//��������
void MyPathItem::createTextItem()
{
    myTextItem = new MyTextItem(GRA_TEXT,this);
    myTextItem->setTextExistType(TEXT_CHILD);
    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    myTextItem->cleartText();

    setAcceptHoverEvents(true);
}

void MyPathItem::setPath(const QPainterPath &path)
{
    painterPath = path;
    QGraphicsPathItem::setPath(path);
}

QRectF MyPathItem::boundingRect()const
{
    return boundRect;
}

//˫���༭������Ϣ
void MyPathItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    MY_BUILD_MODEL_ONLY

    emit editMe();
    QGraphicsPathItem::mouseDoubleClickEvent(event);
}

void MyPathItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    MenuManager::instance()->menu(Constants::MENU_ITEM_RIGHT_MENU)->exec(event->screenPos());
}

QPainterPath MyPathItem::shape()const
{
    QPainterPath path;
    for(int i = 0;i < points.size() - 1;i++)
    {
        QPainterPath  tmpPath;
        tmpPath.moveTo(points.at(i));
        tmpPath.lineTo(points.at(i+1));
        path.addPath(tmpPath);
    }

    QPainterPathStroker stroker;
    stroker.setWidth(5);
    return stroker.createStroke(path);
}

void MyPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->save();

    if(property.isNeedBorder)
    {
        QPen itemPen;
        itemPen.setWidth(5);
        itemPen.setColor(Qt::red);
        painter->setPen(property.itemPen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }

    painter->setPen(property.itemPen);
    painter->drawPath(painterPath);

    //���
    if(property.startLineType == LINE_HORIZONTAL)
    {

    }
    else if(property.startLineType == LINE_SOLID_TRIANGLE)
    {
        QPolygonF polygon;
        if(points.size() > 0)
        {
            QPointF lastPoint = points.first();
            if(property.startPointType == MIDDLE_LEFT)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() - ARROW_SIZE,lastPoint.y() - ARROW_SIZE/2)<<QPointF(lastPoint.x() - ARROW_SIZE,lastPoint.y()+ARROW_SIZE/2);
            }
            else if(property.startPointType == TOP_MIDDLE)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() - ARROW_SIZE/2,lastPoint.y() - ARROW_SIZE)<<QPointF(lastPoint.x() + ARROW_SIZE/2,lastPoint.y()-ARROW_SIZE);
            }
            else if(property.startPointType == MIDDLE_RIGHT)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() + ARROW_SIZE,lastPoint.y() - ARROW_SIZE/2)<<QPointF(lastPoint.x() + ARROW_SIZE,lastPoint.y()+ARROW_SIZE/2);
            }
            else if(property.startPointType == BOTTOM_MIDDLE)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() - ARROW_SIZE/2,lastPoint.y() + ARROW_SIZE)<<QPointF(lastPoint.x() + ARROW_SIZE/2,lastPoint.y()+ARROW_SIZE);
            }
        }

        painter->setBrush(property.itemBrush);
        painter->drawPolygon(polygon);
    }

    //�յ�
    if(property.endLineType == LINE_HORIZONTAL)
    {

    }
    else if(property.endLineType == LINE_SOLID_TRIANGLE)
    {
        QPolygonF polygon;
        if(points.size() > 0)
        {
            QPointF lastPoint = points.last();
            if(property.endPointType == MIDDLE_LEFT)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() - ARROW_SIZE,lastPoint.y() - ARROW_SIZE/2)<<QPointF(lastPoint.x() - ARROW_SIZE,lastPoint.y()+ARROW_SIZE/2);
            }
            else if(property.endPointType == TOP_MIDDLE)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() - ARROW_SIZE/2,lastPoint.y() - ARROW_SIZE)<<QPointF(lastPoint.x() + ARROW_SIZE/2,lastPoint.y()-ARROW_SIZE);
            }
            else if(property.endPointType == MIDDLE_RIGHT)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() + ARROW_SIZE,lastPoint.y() - ARROW_SIZE/2)<<QPointF(lastPoint.x() + ARROW_SIZE,lastPoint.y()+ARROW_SIZE/2);
            }
            else if(property.endPointType == BOTTOM_MIDDLE)
            {
                polygon<<lastPoint<<QPointF(lastPoint.x() - ARROW_SIZE/2,lastPoint.y() + ARROW_SIZE)<<QPointF(lastPoint.x() + ARROW_SIZE/2,lastPoint.y()+ARROW_SIZE);
            }
        }

        painter->setBrush(property.itemBrush);
        painter->drawPolygon(polygon);
    }

    painter->restore();

//    QPen itemPen1;
//    itemPen1.setWidth(1);
//    itemPen1.setColor(Qt::blue);
//    itemPen1.setStyle(Qt::DashLine);
//    painter->setPen(itemPen1);
//    painter->drawRect(boundRect);
}

void MyPathItem::setProperty(ItemProperty property)
{
    this->property = property;
    myTextItem->setProperty(property);
    if(property.content.size() > 0)
    {
        setText(property.content);
    }
    update();
}

//�����߶ε����
void MyPathItem::setStartItem(MyNodeLine *startItem)
{
    this->startItem = startItem;
    countPathItemPoints();
}

//������ʼ�����ݱ��
void MyPathItem::setStartItemID(const QString id)
{
    property.startItemID = id;
}

//�����߶ε��յ�
void MyPathItem::setEndItem(MyNodeLine *endItem)
{
    this->endItem = endItem;
    countPathItemPoints();
}

//�����յ����ݱ��
void MyPathItem::setEndItemID(const QString id)
{
    property.endItemID = id;
}

//�ڡ�LINE_MYITEM��ģʽ�±�����������
void MyPathItem::setStartPointType(PointType type)
{
    property.startPointType = type;
}

//�����߶ε��յ�
void MyPathItem::setEndPointType(PointType type)
{
    property.endPointType = type;
}

//�ڡ�LINE_NODEPORT��ģʽ�±����������ͣ���DragDirectӳ�䵽PointType
void MyPathItem::setStartPointType(DragDirect type)
{
    if(type == DRAG_LEFT)
    {
        property.startPointType = MIDDLE_LEFT;
    }
    else if(type == DRAG_TOP)
    {
        property.startPointType = TOP_MIDDLE;
    }
    else if(type == DRAG_RIGHT)
    {
        property.startPointType = MIDDLE_RIGHT;
    }
    else if(type == DRAG_BOTTOM)
    {
        property.startPointType = BOTTOM_MIDDLE;
    }
}

void MyPathItem::setEndPointType(DragDirect type)
{
    if(type == DRAG_LEFT)
    {
        property.endPointType = MIDDLE_LEFT;
    }
    else if(type == DRAG_TOP)
    {
        property.endPointType = TOP_MIDDLE;
    }
    else if(type == DRAG_RIGHT)
    {
        property.endPointType = MIDDLE_RIGHT;
    }
    else if(type == DRAG_BOTTOM)
    {
        property.endPointType = BOTTOM_MIDDLE;
    }
}

//��������������
void MyPathItem::setStartLineType(int type)
{
    this->property.startLineType = (AddLineType)type;
    emit updateSceneDraw();
}

//�����յ��������
void MyPathItem::setEndLineType(int type)
{
    this->property.endLineType = (AddLineType)type;
    emit updateSceneDraw();
}

//���ݴ���������յ�item�������һ�����ߣ�Ȼ�������ϵ��۵㱣������
void MyPathItem::countPathItemPoints()
{
    MY_ASSERT(startItem)
    MY_ASSERT(endItem)

    startPosPoint = startItem->getParentItem()->mapToScene(startItem->pos());
    endPosPoint = endItem->getParentItem()->mapToScene(endItem->pos());

    getNewLine();
}

void MyPathItem::setStartPoint(QPointF startPoint)
{
    startPosPoint = startPoint;
    getNewLine();
}

void MyPathItem::setEndPoint(QPointF endPoint)
{
    endPosPoint = endPoint;
    getNewLine();
}

//���ݵ�ǰ���õ������յ㣬ʵʱ�����������ı���λ��
void MyPathItem::getNewLine()
{
    QPointF startPoint,endPoint;       //�۵�������յ�
    startPoint = QPointF(0,0);

    endPoint = endPosPoint - startPosPoint;

    detectItem(startPoint,endPoint);

    prepareGeometryChange();
    painterPath = QPainterPath();
    for(int i = 0; i < points.size(); i++)
    {
        if(i == 0)
        {
            painterPath.moveTo(points.at(i));
        }
        else
        {
            painterPath.lineTo(points.at(i));
        }
    }

    QRectF rect(startPosPoint,endPosPoint);
    setPos(rect.topLeft());

    //�ҳ���������������ҵĵ�
    if(points.size() > 0)
    {
        qreal leftMinX = 0, leftMinY = 0;
        qreal rightMaxX = 0, rightMaxY = 0;
        foreach(QPointF point,points)
        {
            if(point.x() <= leftMinX)
            {
                leftMinX = point.x();
            }

            if(point.y() <= leftMinY)
            {
                leftMinY = point.y();
            }

            if(point.x() >= rightMaxX)
            {
                rightMaxX = point.x();
            }

            if(point.y() >= rightMaxY)
            {
                rightMaxY = point.y();
            }
        }

        leftMinX -= 2;
        leftMinY -= 2;
        rightMaxX += 2;
        rightMaxY += 2;

        //��boundRect��setPos�й�
        boundRect = QRectF(QPointF(leftMinX,leftMinY),QPointF(rightMaxX,rightMaxY));
    }

    updateTextPos();

    update();
}

//���������ӵ���Item������·�����ɡ�
void MyPathItem::detectItem(QPointF startPoint,QPointF endPoint)
{
    points.clear();

    QLineF line(startPosPoint,endPosPoint);
    qreal pointXDistance =qAbs(line.dx());
    qreal pointYDistance = qAbs(line.dy());

    qreal startItemWidth , endItemWidth;
    qreal startItemHeight , endItemHeight;

    if(startItem)
    {
        MyItem * sitem = dynamic_cast<MyItem *>(startItem->getParentItem());
        if(sitem)
        {
            startItemWidth = sitem->getProperty().itemRect.width / 2;
            startItemHeight = sitem->getProperty().itemRect.height / 2;
        }
    }

    if(endItem)
    {
        MyItem * eitem = dynamic_cast<MyItem *>(endItem->getParentItem());
        if(eitem)
        {
            endItemWidth = eitem->getProperty().itemRect.width / 2;
            endItemHeight = eitem->getProperty().itemRect.height / 2;
        }
    }

    //����
    if(startPoint.x() <= endPoint.x() && startPoint.y() <= endPoint.y())
    {
        if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x() + startItemWidth,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + startItemWidth,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(endPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + pointXDistance /2,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + pointXDistance /2,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            if(pointYDistance < (startItemHeight + endItemHeight))
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() - startItemHeight - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() - startItemHeight - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() + pointYDistance/2));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() + pointYDistance/2));
            }
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == BOTTOM_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x() ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - startItemWidth ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - startItemWidth ,endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_LEFT))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),endPoint.y()));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            points.append(QPointF(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPointF(startPoint.x(),startPoint.y() + pointYDistance /2));
            points.append(QPoint(endPoint.x() ,startPoint.y() + pointYDistance /2));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x(),startPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(endPoint.x() + startItemWidth *2 +PATH_ITEM_MIN_DIS ,startPoint.y()));
                points.append(QPoint(endPoint.x() + startItemWidth *2 +PATH_ITEM_MIN_DIS ,endPoint.y() + startItemHeight+PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS ,endPoint.y() + startItemHeight+PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x() + pointXDistance/2,startPoint.y()));
                points.append(QPoint(startPoint.x() + pointXDistance/2,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(endPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(endPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
    }
    //����
    else if(startPoint.x() <= endPoint.x() && startPoint.y() > endPoint.y())
    {
        if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
        {
            if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
            {
                points.append(startPoint);
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
                {
                    points.append(QPoint(startPoint.x() - startItemWidth,startPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(startPoint.x() - startItemWidth,endPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                }
                else
                {
                    points.append(QPoint(startPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                }
                points.append(endPoint);
            }
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - pointYDistance /2));
            points.append(QPoint(endPoint.x(),startPoint.y() - pointYDistance /2));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            if(pointYDistance < (startItemHeight + endItemHeight))
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() + startItemHeight + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() + startItemHeight + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() - pointYDistance/2));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() - pointYDistance/2));
            }
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == BOTTOM_MIDDLE))
        {
            points.append(startPoint);
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(endPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(endPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemWidth ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemWidth ,endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x() - startItemWidth,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - startItemWidth,endPoint.y()));
            }
            else
            {
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            points.append(QPointF(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(startPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() + pointXDistance / 2,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + pointXDistance / 2,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(endPoint.x() + startItemWidth,startPoint.y()));
                points.append(QPoint(endPoint.x() + startItemWidth,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(endPoint.x() + startItemWidth *2 +PATH_ITEM_MIN_DIS ,startPoint.y()));
                points.append(QPoint(endPoint.x() + startItemWidth *2 +PATH_ITEM_MIN_DIS ,endPoint.y() - startItemHeight-PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS ,endPoint.y() - startItemHeight-PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x() + pointXDistance/2,startPoint.y()));
                points.append(QPoint(startPoint.x() + pointXDistance/2,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() ,startPoint.y()));
            points.append(endPoint);
        }
    }
    //����
    else if(startPoint.x() >= endPoint.x() && startPoint.y() <= endPoint.y())
    {
        if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
        {
            if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
            {
                points.append(startPoint);
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
                {
                    points.append(QPoint(startPoint.x() - startItemWidth,startPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(startPoint.x() - startItemWidth,endPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                }
                else
                {
                    points.append(QPoint(endPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                }
                points.append(endPoint);
            }
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - pointXDistance /2,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - pointXDistance /2,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x(),startPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()+pointYDistance/2));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()+pointYDistance/2));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x() - pointXDistance/2,startPoint.y()));
                points.append(QPoint(startPoint.x() - pointXDistance/2,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == BOTTOM_MIDDLE))
        {
            points.append(startPoint);
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(endPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(endPoint.x() - startItemWidth - PATH_ITEM_MIN_DIS,endPoint.y()+PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemWidth ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemWidth ,endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS ,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x(),endPoint.y()));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() + pointYDistance / 2));
            points.append(QPoint(endPoint.x(),startPoint.y() + pointYDistance / 2));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            if(pointYDistance < (startItemHeight + endItemHeight))
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS ,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y() + startItemHeight + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS ,endPoint.y() + startItemHeight + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() + pointYDistance /2 ));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() + pointYDistance /2 ));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance <= startItemWidth || pointXDistance <= endItemWidth)
            {
                points.append(QPoint(endPoint.x() + endItemWidth + PATH_ITEM_MIN_DIS ,startPoint.y()));
                points.append(QPoint(endPoint.x() + endItemWidth + PATH_ITEM_MIN_DIS ,endPoint.y()+PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y()+PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()+PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y()+PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
    }
    //����
    else if(startPoint.x() >= endPoint.x() && startPoint.y() > endPoint.y())
    {
        if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
        {
            if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
            {
                points.append(startPoint);
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
                {
                    points.append(QPoint(startPoint.x() + startItemWidth,startPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(startPoint.x() + startItemWidth,endPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                }
                else
                {
                    points.append(QPoint(startPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                }
                points.append(endPoint);
            }
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x(),endPoint.y()));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == TOP_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - pointYDistance/2));
            points.append(QPoint(endPoint.x(),startPoint.y() - pointYDistance/2));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(endPoint.x() - startItemWidth,startPoint.y()));
                points.append(QPoint(endPoint.x() - startItemWidth,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()-pointYDistance/2));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()-pointYDistance/2));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x() - pointXDistance/2,startPoint.y()));
                points.append(QPoint(startPoint.x() - pointXDistance/2,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_LEFT && property.endPointType == BOTTOM_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() ,startPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - startItemWidth ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - startItemWidth ,endPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() + PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == MIDDLE_RIGHT))
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + startItemWidth ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + startItemWidth ,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            if(pointXDistance < startItemWidth || pointXDistance < endItemWidth)
            {
                points.append(QPoint(endPoint.x() + startItemWidth,startPoint.y()));
                points.append(QPoint(endPoint.x() + startItemWidth,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            if(pointYDistance < (startItemHeight + endItemHeight))
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS ,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y() - startItemHeight - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS ,endPoint.y() - startItemHeight - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            else
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y() - pointYDistance /2 ));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() - pointYDistance /2 ));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == BOTTOM_MIDDLE && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
            if(pointXDistance < (startItemWidth + endItemWidth))
            {
                points.append(QPoint(startPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + startItemWidth + PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() - pointXDistance / 2,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - pointXDistance / 2,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointYDistance <= startItemHeight || pointXDistance <= endItemHeight)
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS ,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS ,startPoint.y()+startItemHeight+PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),startPoint.y()+startItemHeight+PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS ,startPoint.y()));
                points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()+PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y()+PATH_ITEM_MIN_DIS));
            }
            points.append(endPoint);
        }
    }
}

//��itemλ�øı�󣬸��µ�ǰ���ߣ�������paint��ֱ�ӵ���(��Ӱ��ˢ���ٶ�)
void MyPathItem::updateCurrItemPos()
{
    countPathItemPoints();
}

//��ȡ·���Ľڵ�����
QList<QPointF> MyPathItem::getPathPoints()
{
    return this->points;
}

//����·���Ľڵ�����
void MyPathItem::setPathPoints(QList<QPointF> &points)
{
    this->points.clear();
    foreach(QPointF point,points)
    {
        this->points.push_back(point);
    }
}

//�����������˵�����
void MyPathItem::setLineType(LineType lineType)
{
    property.lineType = lineType;
}

QString MyPathItem::getText()
{
    return myTextItem->toPlainText();
}

//����������Ϣ��ͬʱ����textItem��item�е�λ��
void MyPathItem::setText(QString text)
{
    property.content = text;
    myTextItem->setPlainText(text);

    updateTextPos();
}

//ʵʱ����Text��λ��
void MyPathItem::updateTextPos()
{
    //�����ı���λ��,�ҳ�������ߡ�
    qreal maxPos = 0;
    qreal maxLen = 0;
    for(int i = 0; i < points.size() - 1; i++)
    {
        qreal tmpLen = 0;
        if(points.at(i).x() == points.at(i+1).x())
        {
            if(points.at(i).y() != points.at(i+1).y())
            {
                tmpLen = qAbs(points.at(i).y() - points.at(i+1).y());
            }
        }
        else
        {
            tmpLen = qAbs(points.at(i).x() - points.at(i+1).x());
        }

        if(tmpLen > maxLen)
        {
            maxLen = tmpLen;
            maxPos = i;
        }
    }

    if(maxPos >=0 && maxPos <= points.size() -1)
    {
        QPointF spoint = points.at(maxPos);
        QPointF epoint = points.at(maxPos+1);

        myTextItem->setPos((spoint.x() + epoint.x())/2 - myTextItem->getBoundRect().width()/2,(spoint.y() + epoint.y())/2);
    }
    else
    {
        myTextItem->setPos(boundingRect().center());
    }
}

MyPathItem::~MyPathItem()
{

}
