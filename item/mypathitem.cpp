#include "mypathitem.h"

#include <QPainter>
#include <QRectF>
#include <QDebug>

#include "myitem.h"
#include "mynodeline.h"

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
    property.itemPen = QPen(Qt::red,3);

    startItem = NULL;
    endItem = NULL;
}

void MyPathItem::setPath(const QPainterPath &path)
{
    painterPath = path;
    QGraphicsPathItem::setPath(path);
}

QRectF MyPathItem::boundingRect()const
{
//    qDebug()<<boundRect.x()<<"__"<<boundRect.y()<<"__"<<boundRect.width()<<"__"<<boundRect.height();

    return boundRect;
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

    painter->setBrush(Qt::NoBrush);

    painter->setPen(QPen(Qt::red,3));
    painter->drawPath(painterPath);

    painter->restore();

    QPen itemPen1;
    itemPen1.setWidth(1);
    itemPen1.setColor(Qt::blue);
    itemPen1.setStyle(Qt::DashLine);
    painter->setPen(itemPen1);
    painter->drawRect(boundRect);
}

void MyPathItem::setProperty(ItemProperty property)
{
    this->property = property;

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

//���ݵ�ǰ���õ������յ㣬ʵʱ��������
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

    if(points.size() > 0)
    {
        QPointF firstPoint = points.at(0);
        QPointF lastPoint = points.last();

        boundRect = QRectF(firstPoint,lastPoint);
    }

    update();
}

//���������ӵ���Item������·�����ɡ�
void MyPathItem::detectItem(QPointF startPoint,QPointF endPoint)
{
    points.clear();

    QLineF line(startPosPoint,endPosPoint);
    qreal  pointDistance =qAbs(line.dx());

    int startItemRadius , endItemRadius;

    if(startItem)
    {
        MyItem * sitem = dynamic_cast<MyItem *>(startItem->getParentItem());
        if(sitem)
        {
            startItemRadius = sitem->getProperty().itemRect.width / 2;
        }
    }

    if(endItem)
    {
        MyItem * eitem = dynamic_cast<MyItem *>(endItem->getParentItem());
        if(eitem)
        {
            endItemRadius = eitem->getProperty().itemRect.width / 2;
        }
    }

    qDebug()<<startPoint<<"+++++++++"<<endPoint;

    //����
    if(startPoint.x() <= endPoint.x() && startPoint.y() <= endPoint.y())
    {
        if(property.startPointType == TOP_MIDDLE && property.endPointType == TOP_MIDDLE)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(startPoint.x() + startItemRadius,startPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() + startItemRadius,endPoint.y() - PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
            }
            else
            {
                points.append(QPoint(endPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
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
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(startPoint.x() ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - startItemRadius ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - startItemRadius ,endPoint.y() + PATH_ITEM_MIN_DIS));
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
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x(),startPoint.y()));
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
                if(pointDistance < startItemRadius || pointDistance < endItemRadius)
                {
                    points.append(QPoint(startPoint.x() - startItemRadius,startPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(startPoint.x() - startItemRadius,endPoint.y() - PATH_ITEM_MIN_DIS));
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
        else if(property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemRadius ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemRadius ,endPoint.y() + PATH_ITEM_MIN_DIS));
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
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(startPoint.x() - startItemRadius,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - startItemRadius,endPoint.y()));
            }
            else
            {
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            }
            points.append(endPoint);
        }
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(endPoint.x() + startItemRadius,startPoint.y()));
                points.append(QPoint(endPoint.x() + startItemRadius,endPoint.y() - PATH_ITEM_MIN_DIS));
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
                if(pointDistance < startItemRadius || pointDistance < endItemRadius)
                {
                    points.append(QPoint(startPoint.x() - startItemRadius,startPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(startPoint.x() - startItemRadius,endPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(endPoint.x(),endPoint.y() - PATH_ITEM_MIN_DIS));
                }
                else
                {
                    points.append(QPoint(endPoint.x(),startPoint.y() - PATH_ITEM_MIN_DIS));
                }
                points.append(endPoint);
            }
        }
        else if(property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemRadius ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(endPoint.x() + startItemRadius ,endPoint.y() + PATH_ITEM_MIN_DIS));
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
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y() - PATH_ITEM_MIN_DIS));
            points.append(QPoint(endPoint.x() ,endPoint.y() - PATH_ITEM_MIN_DIS));
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
                if(pointDistance < startItemRadius || pointDistance < endItemRadius)
                {
                    points.append(QPoint(startPoint.x() + startItemRadius,startPoint.y() - PATH_ITEM_MIN_DIS));
                    points.append(QPoint(startPoint.x() + startItemRadius,endPoint.y() - PATH_ITEM_MIN_DIS));
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
        else if(property.startPointType == MIDDLE_LEFT && property.endPointType == MIDDLE_LEFT)
        {
            points.append(startPoint);
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(endPoint.x() - PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == MIDDLE_RIGHT && property.endPointType == MIDDLE_RIGHT)
        {
            points.append(startPoint);
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,startPoint.y()));
            points.append(QPoint(startPoint.x() + PATH_ITEM_MIN_DIS,endPoint.y()));
            points.append(endPoint);
        }
        else if(property.startPointType == BOTTOM_MIDDLE && property.endPointType == BOTTOM_MIDDLE)
        {
            points.append(startPoint);
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(startPoint.x(),startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - startItemRadius ,startPoint.y() + PATH_ITEM_MIN_DIS));
                points.append(QPoint(startPoint.x() - startItemRadius ,endPoint.y() + PATH_ITEM_MIN_DIS));
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
        else if((property.startPointType == MIDDLE_RIGHT && property.endPointType == TOP_MIDDLE))
        {
            points.append(startPoint);
            if(pointDistance < startItemRadius || pointDistance < endItemRadius)
            {
                points.append(QPoint(endPoint.x() + startItemRadius,startPoint.y()));
                points.append(QPoint(endPoint.x() + startItemRadius,endPoint.y() - PATH_ITEM_MIN_DIS));
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

MyPathItem::~MyPathItem()
{

}
