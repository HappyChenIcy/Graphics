#include "myarrow.h"

#include <QPainter>
#include <QRectF>
#include <QLineF>
#include <QCursor>
#include <QDebug>

#include "myitem.h"
#include "mynodeport.h"
#include "mytextitem.h"
#include "../global.h"

#include "math.h"

const qreal Pi = 3.14;
#define ARROW_SIZE 15

QDataStream & operator <<(QDataStream &stream,MyArrow * item)
{
    int type = item->type;

    stream<<type<<item->property;

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyArrow * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->type = (GraphicsType)type;

    item->setProperty(prop);

    return stream;
}

MyArrow::MyArrow(QGraphicsItem *parent):
    QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    type = GRA_LINE;
    property.itemBrush = QBrush(Qt::black);
    property.itemPen = QPen(Qt::black,2);

    property.endLineType = LINE_SOLID_TRIANGLE;

    createTextItem();
}

//�����������˵�����
void MyArrow::setLineType(LineType lineType)
{
    property.lineType = lineType;
}

//�����߶ε����
void MyArrow::setStartItem(MyNodeLine *startItem)
{
    this->startItem = startItem;
}

//������ʼ�����ݱ��
void MyArrow::setStartItemID(const QString id)
{
    property.startItemID = id;
}

//�ڡ�LINE_MYITEM��ģʽ�±�����������
void MyArrow::setStartPointType(PointType type)
{
    property.startPointType = type;
}

//�����߶ε��յ�
void MyArrow::setEndItem(MyNodeLine *endItem)
{
    this->endItem = endItem;
}

//�����յ����ݱ��
void MyArrow::setEndItemID(const QString id)
{
    property.endItemID = id;
}

//�ڡ�LINE_MYITEM��ģʽ�±����յ������
void MyArrow::setEndPointType(PointType type)
{
    property.endPointType = type;
}

//��������
void MyArrow::createTextItem()
{
    myTextItem = new MyTextItem(GRA_TEXT,NULL,this);
    myTextItem->setTextExistType(TEXT_CHILD);
    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    myTextItem->cleartText();

    setAcceptHoverEvents(true);
}

QRectF MyArrow::boundingRect()const
{
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                           line().p2().y() - line().p1().y()));
}

QPainterPath MyArrow::shape()const
{
    QPainterPath path = QGraphicsLineItem::shape();
    if(property.lineType == LINE_MYITEM)
    {
        path.addPolygon(arrowHead);
    }
    return path;
}

//������ʽ����
void MyArrow::setProperty(ItemProperty property)
{
    this->property = property;
    myTextItem->setProperty(property);
    if(property.content.size() > 0)
    {
        setText(property.content);
    }
    update();
}

//��������������
void MyArrow::setStartLineType(int type)
{
    this->property.startLineType = (AddLineType)type;
    emit updateSceneDraw();
}

//�����յ��������
void MyArrow::setEndLineType(int type)
{
    this->property.endLineType = (AddLineType)type;
    emit updateSceneDraw();
}

void MyArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    if(property.lineType == LINE_MYITEM && startItem && endItem)
    {
        //�����ؼ������ĵ�ֱ���������ֱ�������������ؼ�����
//        QLineF centerLine(startItem->pos(), endItem->pos());

        QPointF startCrossPoint = startItem->getParentItem()->mapToScene(startItem->pos());

//        countItemCrossPoint(true,startItem->polygon(),centerLine,startCrossPoint);

        QPointF endCrossPoint = endItem->getParentItem()->mapToScene(endItem->pos());
//        countItemCrossPoint(false,endItem->polygon(),centerLine,endCrossPoint);

        setLine(QLineF(startCrossPoint,endCrossPoint));
    }
    else if(property.lineType == LINE_NODEPORT && startItem && endItem)
    {
        //��ȡ�˿��������scene������ֵ
        QPointF startPoint = startItem->getParentItem()->mapToScene(startItem->pos());
        QPointF endPoint = endItem->getParentItem()->mapToScene(endItem->pos());

        QLineF centerLine(startPoint, endPoint);

        MyNodePort * startNodePort = dynamic_cast<MyNodePort *>(startItem);
        MyNodePort * endNodePort = dynamic_cast<MyNodePort *>(endItem);

        QPointF endCrossPoint;
        countNodeCrossPoint(endNodePort->getScenePolygon(),centerLine,endCrossPoint);

        QPointF startCrossPoint;
        countNodeCrossPoint(startNodePort->getScenePolygon(),centerLine,startCrossPoint);

        setLine(QLineF(startCrossPoint,endCrossPoint));
    }

    painter->setPen(property.itemPen);
    painter->drawLine(line());

    //���
    if(property.startLineType == LINE_HORIZONTAL)
    {

    }
    else if(property.startLineType == LINE_ARROW)
    {
        QPointF arrowP1,arrowP2;
        countArrowPoint(false,line().p1(),arrowP1,arrowP2);
        painter->drawLine(line().p1(),arrowP1);
        painter->drawLine(line().p1(),arrowP2);
    }
    else if(property.startLineType == LINE_SOLID_TRIANGLE)
    {
        QPolygonF polygon = countArrowPolygon(line().p1(),false);

        painter->setBrush(property.itemBrush);
        painter->drawPolygon(polygon);
    }

    //�յ�
    if(property.endLineType == LINE_HORIZONTAL)
    {

    }
    else if(property.endLineType == LINE_ARROW)
    {
        QPointF arrowP1,arrowP2;
        countArrowPoint(true,line().p2(),arrowP1,arrowP2);
        painter->drawLine(line().p2(),arrowP1);
        painter->drawLine(line().p2(),arrowP2);
    }
    else if(property.endLineType == LINE_SOLID_TRIANGLE)
    {
        QPolygonF polygon = countArrowPolygon(line().p2(),true);
        painter->setBrush(property.itemBrush);
        painter->drawPolygon(polygon);
    }

    if (isSelected())
    {
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }

    myTextItem->setCentralPos((line().p1()+line().p2())/2);

    painter->restore();
}

//�����ͷ
QPolygonF MyArrow::countArrowPolygon(QPointF startPoint,bool isStart)
{
    QPointF arrowP1,arrowP2;
    countArrowPoint(isStart,startPoint,arrowP1,arrowP2);

    QPolygonF  polygon;
    polygon << startPoint << arrowP1 << arrowP2;

    return polygon;
}

void MyArrow::countArrowPoint(bool isStart,QPointF & startPoint,QPointF & point1, QPointF & point2)
{
    double angle = ::acos(line().dx() / line().length());

    if (line().dy() >= 0)
    {
        angle = (Pi * 2) - angle;
    }

    if(isStart)
    {
        point1 = startPoint - QPointF(sin(angle + Pi / 3) * ARROW_SIZE,cos(angle + Pi / 3) * ARROW_SIZE);
        point2 = startPoint - QPointF(sin(angle + Pi - Pi / 3) * ARROW_SIZE,cos(angle + Pi - Pi / 3) * ARROW_SIZE);
    }
    else
    {
        point1 = startPoint + QPointF(sin(angle + Pi / 3) * ARROW_SIZE,cos(angle + Pi / 3) * ARROW_SIZE);
        point2 = startPoint + QPointF(sin(angle + Pi - Pi / 3) * ARROW_SIZE,cos(angle + Pi - Pi / 3) * ARROW_SIZE);
    }
}

//����˿ڽ���������
void MyArrow::countNodeCrossPoint(QPolygonF polygon,QLineF centerLine,QPointF &endIntersectPoint)
{
    QPointF p1 = polygon.first();
    QPointF p2;
    QLineF polyLine;
    //����������ε��е���յ����εĽ���
    for (int i = 1; i <= polygon.count(); ++i)
    {
        //���һ����Ҫ�͵�һ����������
        if(i == polygon.count())
        {
            p1 = polygon.first();
        }
        else
        {
            p2 = polygon.at(i);
        }

        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &endIntersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
        {
            break;
        }
        p1 = p2;
    }
}

//�����������㽻�������꡾������߶���ʼ�����ؼ�һ�ߵ��е㣬��������������ؼ��ߵĽ������꡿
void MyArrow::countItemCrossPoint(bool isStart , QPolygonF polygon,QLineF centerLine,QPointF &intersectPoint)
{
    //��polygons��item������ϵת����scene������ϵ��֧�ּ�ʹ�ؼ���ת����Ȼ���Ա��ּ�ͷָ��ĳһ��
//    MyItem * tmpItem;
//    if(isStart)
//    {
//        tmpItem = startItem;
//    }
//    else
//    {
//        tmpItem = endItem;
//    }

//    QPointF p1 = tmpItem->mapToScene(polygon.first());
//    QPointF p2;
//    QLineF polyLine;
//    //����������ε��е���յ����εĽ���
//    for (int i = 1; i <= polygon.count(); ++i)
//    {
//        //���һ����Ҫ�͵�һ����������
//        if(i == polygon.count())
//        {
//            p1 = tmpItem->mapToScene(polygon.first());
//        }
//        else
//        {
//            p2 = tmpItem->mapToScene(polygon.at(i));
//        }

//        polyLine = QLineF(p1, p2);
//        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
//        if (intersectType == QLineF::BoundedIntersection)
//        {
//            break;
//        }
//        p1 = p2;
//    }
}

//�����߶ε�λ��
void MyArrow::updatePosition()
{
    QLineF line(mapFromItem(startItem, 0, 0), mapFromItem(endItem, 0, 0));
    setLine(line);
}

//˫���༭������Ϣ
void MyArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(GlobalWindowState == WINDOW_BUILD_MODEL)
    {
        emit editMe();
        QGraphicsLineItem::mouseDoubleClickEvent(event);
    }
}

//������
void MyArrow::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    QGraphicsLineItem::hoverEnterEvent(event);
}

void MyArrow::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsLineItem::hoverLeaveEvent(event);
}

QString MyArrow::getText()
{
    return myTextItem->toPlainText();
}

//����������Ϣ��ͬʱ����textItem��item�е�λ��
void MyArrow::setText(QString text)
{
    property.content = text;
    myTextItem->setPlainText(text);

    QRectF rectF = myTextItem->getBoundRect();

    myTextItem->setPos(-rectF.width()/2,-rectF.height()/2);
}

MyArrow::~MyArrow()
{
    startItem = NULL;
    endItem = NULL;

}
