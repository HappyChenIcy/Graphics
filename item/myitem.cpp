#include "myitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QMenu>
#include <QDebug>
#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QUrl>

#include "myarrow.h"
#include "mytextitem.h"
#include "mynodeport.h"
#include "mygraphicsview.h"

#include "qmath.h"

#define QCOS(a) qCos(a*PI/180)
#define QSIN(a) qSin(a*PI/180)

//��MyRect�������������
QDataStream & operator <<(QDataStream &stream,MyRect & rect)
{
    stream<<rect.x<<rect.y<<rect.width<<rect.height;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyRect & rect)
{
    stream>>rect.x>>rect.y>>rect.width>>rect.height;
    return stream;
}

////д������
QDataStream & operator <<(QDataStream & stream,ItemProperty & prop)
{
    stream<<prop.startItemID<<prop.endItemID
           <<prop.isNeedBrush<<prop.itemBrush
            <<prop.isNeedBorder<<prop.itemPen<<prop.itemRect
             <<prop.alphaValue<<prop.rotateDegree
              <<prop.isFont<<prop.content<<prop.itemFont<<prop.fontColor
               <<prop.zValue;

    return stream;
}

////������
QDataStream & operator >>(QDataStream & stream,ItemProperty & prop)
{
    stream>>prop.startItemID>>prop.endItemID
           >>prop.isNeedBrush>>prop.itemBrush
            >>prop.isNeedBorder>>prop.itemPen>>prop.itemRect
             >>prop.alphaValue>>prop.rotateDegree
              >>prop.isFont>>prop.content>>prop.itemFont>>prop.fontColor
               >>prop.zValue;
    return stream;
}

QDataStream & operator <<(QDataStream &stream,MyItem * item)
{
    int type = item->currItemType;

    stream<<type<<item->property;

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyItem * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->currItemType = (GraphicsType)type;

    item->setProperty(prop);

    return stream;
}

MyItem::MyItem(GraphicsType itemType, QMenu *menu, QGraphicsScene *parentScene, QObject *parent1, QGraphicsItem *parent2):
    currItemType(itemType),
    rightMenu(menu),
    parentScene(parentScene),
    QObject(parent1),
    QGraphicsPolygonItem(parent2)
{
    radius = 100;

    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setAcceptDrops(true);

    prepareGeometryChange();

    float factor;

    switch(currItemType)
    {
            //������
            case GRA_SQUARE:
                               boundRect = QRectF(-radius,-radius,2*radius,2*radius);   //���÷�ΧʱͬʱҲĬ��ָ���������ĵ�����(0,0)
                               itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)<<
                                      QPointF(radius,radius)<<QPointF(-radius,radius);

                               property.itemBrush = QBrush(Qt::red);
                               break;
            //����
            case GRA_RECT:
                               factor = 0.5;
                               boundRect = QRectF(-radius,-factor*radius,2*radius,radius);
                               itemPolygon<<QPointF(-radius,-factor*radius)<<QPointF(radius,-factor*radius)<<
                                       QPointF(radius,factor*radius)<<QPointF(-radius,factor*radius);

                               property.itemBrush = QBrush(Qt::blue);
                               break;
           //Բ�Ǿ���
           case GRA_ROUND_RECT:
                              {
                                  boundRect = QRectF(-radius,-0.5*radius,2*radius,radius);
                                  QPainterPath path;
                                  path.addRoundedRect(boundRect,10,10);
                                  itemPolygon = path.toFillPolygon();

                                  property.itemBrush = QBrush(Qt::yellow);
                              }
                              break;
            //Բ��
            case GRA_CIRCLE:
                               {
                                   boundRect = QRectF(-radius,-radius,2*radius,2*radius);
                                   QPainterPath path;
                                   path.addEllipse(boundRect);
                                   itemPolygon = path.toFillPolygon();

                                   property.itemBrush = QBrush(Qt::darkCyan);
                               }
                               break;
            //��Բ
            case GRA_ELLIPSE:
                               {
                                   boundRect = QRectF(-radius,-0.5*radius,2*radius,radius);
                                   QPainterPath path;
                                   path.addEllipse(boundRect);
                                   itemPolygon = path.toFillPolygon();

                                   property.itemBrush = QBrush(Qt::darkMagenta);
                               }
                               break;
            //����
            case GRA_POLYGON:
                               factor = 0.5;
                               boundRect = QRectF(-radius,-factor*radius,2*radius,radius);
                               itemPolygon<<QPointF(-radius,-factor*radius)<<QPointF(0.5*radius,-factor*radius)<<
                                         QPointF(radius,factor*radius)<<QPointF(-0.5*radius,factor*radius);

                               property.itemBrush = QBrush(Qt::gray);
                               break;
    }

    setPolygon(itemPolygon);
    setBrush(property.itemBrush);

    property.itemRect.width = boundRect.width();
    property.itemRect.height = boundRect.height();

    property.itemFont = QFont("����",15);

    currMouseType = MOUSE_NONE;
    isNeedBorder = false;

    selectedPenWidth = 1;
    selectedPen.setColor(Qt::black);
    selectedPen.setStyle(Qt::DashLine);
    selectedPen.setWidth(selectedPenWidth);

    leftTopPoint = new DragPoint(TOP_LEFT,this);
    rightTopPoint = new DragPoint(TOP_RIGHT,this);
    leftBottomPoint = new DragPoint(BOTTOM_LEFT,this);
    rightBottomPoint = new DragPoint(BOTTOM_RIGHT,this);

    topPoint = new DragPoint(TOP_MIDDLE,this);
    leftPoint = new DragPoint(MIDDLE_LEFT,this);
    rightPoint = new DragPoint(MIDDLE_RIGHT,this);
    bottomPoint = new DragPoint(BOTTOM_MIDDLE,this);

    rotateLine = new RotateLine(this);
    connect(rotateLine,SIGNAL(rotateItem(MouseType,int)),this,SLOT(procRotate(MouseType,int)));
    updateRotateLinePos();

    myTextItem = new MyTextItem(GRA_TEXT,menu,this);

    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);

    setAcceptHoverEvents(true);
    setDragPointVisible(false);

    procResizeItem();

}

void MyItem::setPos(const QPointF &pos)
{
    property.itemRect.x = pos.x();
    property.itemRect.y = pos.y();

    QGraphicsPolygonItem::setPos(pos);
}

void MyItem::setZValue(qreal z)
{
    property.zValue = z;
    QGraphicsPolygonItem::setZValue(z);
}

QString MyItem::getText()
{
    return myTextItem->toPlainText();
}

//����������Ϣ��ͬʱ����textItem��item�е�λ��
void MyItem::setText(QString text)
{
    property.content = text;
    myTextItem->setPlainText(text);
    myTextItem->setPos(-myTextItem->getBoundRect().width()/2,-myTextItem->getBoundRect().height()/2);
}

void MyItem::updateRotateLinePos()
{
    int posX = boundingRect().center().x();
    int posY = boundingRect().topLeft().y() - 5*rotateLine->boundingRect().height();

    rotateLine->setPos(posX,posY);
}

QRectF MyItem::boundingRect()const
{
    return boundRect;
}

//������ӵļ�ͷ
void MyItem::addArrow(MyArrow *arrow)
{
    arrows.push_back(arrow);
}

//ɾ���˿ؼ�ʱ���Ƴ����еļ�ͷ
void MyItem::removeArrows()
{
    foreach (MyArrow *arrow, arrows)
    {
        arrow->getStartItem()->removeArrow(arrow);
        arrow->getEndItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void MyItem::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    if(property.isNeedBorder)
    {
        painter->setPen(property.itemPen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }

    if(property.isNeedBrush)
    {
        painter->setBrush(property.itemBrush);
    }

    painter->drawPolygon(itemPolygon);

    painter->restore();
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        setDragPointVisible(true);
    }

    QGraphicsPolygonItem::mousePressEvent(event);
}

void MyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit updateSceneDraw();

    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = this->boundingRect().width();
    property.itemRect.height = this->boundingRect().height();
    emit propHasChanged(property);

    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit updateSceneDraw();
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void MyItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void MyItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

//�Ҽ��˵��¼�
void MyItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    rightMenu->exec(event->screenPos());
}

//�����¼�
void MyItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
       event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

//����㵽�����ı���С����
qreal MyItem::getPointToRectMinDistance(QRectF rect,QPointF point)
{
    //���
    qreal minDis = 100000;

    qreal leftDis = qAbs(rect.topLeft().x() - point.x());
    minDis = minDis <= leftDis ? minDis:leftDis;

    qreal topDis = qAbs(rect.topLeft().y() - point.y());
    minDis = minDis <= topDis ? minDis:topDis;

    qreal rightDis = qAbs(rect.bottomRight().x() - point.x());
    minDis = minDis <= rightDis ? minDis:rightDis;

    qreal bottomDis = qAbs(rect.bottomRight().y() - point.y());
    minDis = minDis <= bottomDis ? minDis:bottomDis;

    return minDis;
}

//�����ƶ�
void MyItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        if(getPointToRectMinDistance(boundRect,event->pos()) <= ALLOW_DROP_RANGE)
        {
            event->acceptProposedAction();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->ignore();
    }
}

//�����뿪
void MyItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{

}

/*!
*��ק����
*�ڷ���ʱ����Ҫ���Ƿ�����һ��,
***/
void MyItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("MyItem"))
    {
        QPointF dropPoint = event->pos();
        QPointF itemPos;
        DragDirect direct;
        qreal scaleFactor;
        //�������
        if(qAbs(dropPoint.x() - boundRect.topLeft().x()) <=  ALLOW_DROP_RANGE)
        {
            itemPos.setX(boundRect.topLeft().x());
            itemPos.setY(dropPoint.y());
            direct = DRAG_LEFT;
            scaleFactor = (dropPoint.y() - boundRect.topLeft().y()) / boundRect.height();
        }
        //�����ϲ�
        else if(qAbs(dropPoint.y() - boundRect.topLeft().y()) <=  ALLOW_DROP_RANGE)
        {
            itemPos.setX(dropPoint.x());
            itemPos.setY(boundRect.topLeft().y());
            direct = DRAG_TOP;
            scaleFactor = (dropPoint.x() - boundRect.topLeft().x()) / boundRect.width();
        }
        //�����Ҳ�
        else if(qAbs(dropPoint.x() - boundRect.bottomRight().x()) <=  ALLOW_DROP_RANGE)
        {
            itemPos.setX(boundRect.bottomRight().x());
            itemPos.setY(dropPoint.y());
            direct = DRAG_RIGHT;
            scaleFactor = (dropPoint.y() - boundRect.topRight().y()) / boundRect.height();
        }
        //�����²�
        else if(qAbs(dropPoint.y() - boundRect.bottomRight().y()) <=  ALLOW_DROP_RANGE)
        {
            itemPos.setX(dropPoint.x());
            itemPos.setY(boundRect.bottomRight().y());
            direct = DRAG_BOTTOM;
            scaleFactor = (dropPoint.x() - boundRect.bottomLeft().x()) / boundRect.width();
        }

        MyNodePort * port = new MyNodePort(this);
        port->setPos(itemPos);
        port->setDragDirect(direct);
        port->setScaleFactor(scaleFactor);
        port->setBrush(property.itemBrush);

        connect(port,SIGNAL(portPosChanged(MouseType,QPointF)),this,SLOT(procPortChanged(MouseType,QPointF)));

        ports.push_back(port);
    }
}

//����˿�λ�øı�,ȷ���˿�ֻ�����ڱ����ƶ�
void MyItem::procPortChanged(MouseType type, QPointF currPoint)
{
    MyNodePort * tmpPort = qobject_cast<MyNodePort *>(QObject::sender());

    if(tmpPort)
    {
        if(type == MOUSE_MOVE)
        {
            QPointF newPoint;
            qreal tmpX,tmpY;

            switch(tmpPort->getDragDirect())
            {
                case DRAG_LEFT:
                              {
                                   tmpX = boundRect.topLeft().x();
                                   tmpY = tmpPort->pos().y() + currPoint.y();
                                   getRangeValue(boundRect.bottomLeft().y(),boundRect.topLeft().y(),tmpY);
                              }
                               break;
                case DRAG_TOP:
                              {
                                   tmpX = tmpPort->pos().x() + currPoint.x();
                                   getRangeValue(boundRect.topRight().x(),boundRect.topLeft().x(),tmpX);

                                   tmpY = boundRect.topLeft().y();
                              }
                               break;
                case DRAG_RIGHT:
                              {
                                   tmpX = boundRect.topRight().x();
                                   tmpY = tmpPort->pos().y() + currPoint.y();
                                   getRangeValue(boundRect.bottomRight().y(),boundRect.topRight().y(),tmpY);
                              }
                               break;
                case DRAG_BOTTOM:
                              {
                                   tmpX = tmpPort->pos().x() + currPoint.x();
                                   getRangeValue(boundRect.bottomRight().x(),boundRect.bottomLeft().x(),tmpX);
                                   tmpY = boundRect.bottomLeft().y();
                              }
                               break;
            }

            newPoint.setX(tmpX);
            newPoint.setY(tmpY);

            tmpPort->setPos(newPoint);
        }
        else if(type == MOUSE_RELEASE)
        {
            qreal scaleFactor;
            switch(tmpPort->getDragDirect())
            {
                case DRAG_LEFT:
                              {
                                   scaleFactor = (tmpPort->pos().y() - boundRect.topLeft().y()) / boundRect.height();
                              }
                               break;
                case DRAG_TOP:
                              {
                                   scaleFactor = (tmpPort->pos().x() - boundRect.topLeft().x()) / boundRect.width();
                              }
                               break;
                case DRAG_RIGHT:
                              {
                                   scaleFactor = (tmpPort->pos().y() - boundRect.topRight().y()) / boundRect.height();
                              }
                               break;
                case DRAG_BOTTOM:
                              {
                                   scaleFactor = (tmpPort->pos().x() - boundRect.topLeft().x()) / boundRect.width();
                              }
                               break;
            }

            tmpPort->setScaleFactor(scaleFactor);
        }
    }
}

//ɾ��ĳ���˿�
void MyItem::procDeleteNodePort(MyNodePort *nodePort)
{
    int index = -1;
    for(int i = 0;i<ports.size();i++)
    {
        if(ports.at(i) ==  nodePort)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        delete ports.at(index);
        ports.removeAt(index);
    }
}

//�༭ĳ���˿�
void MyItem::procEditNodePort(MyNodePort *nodePort)
{
    MyGraphicsView::instance()->showNodePortEdit(nodePort);
}

//����ǰֵ�������Сֵ��Ƚϣ�ֻ���ڴ˷�Χ��
void MyItem::getRangeValue(qreal maxValue, qreal minValue, qreal &currValue)
{
    if(currValue >= maxValue)
    {
        currValue = maxValue;
    }
    else if(currValue <= minValue)
    {
        currValue = minValue;
    }
}

//�����Ƿ���ק�����ת���Ƿ�ɼ�
void MyItem::setDragPointVisible(bool flag)
{
    leftTopPoint->setVisible(flag);
    rightTopPoint->setVisible(flag);
    leftBottomPoint->setVisible(flag);
    rightBottomPoint->setVisible(flag);

    topPoint->setVisible(flag);
    leftPoint->setVisible(flag);
    rightPoint->setVisible(flag);
    bottomPoint->setVisible(flag);

    rotateLine->setVisible(flag);
}

QVariant MyItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange && scene())
    {
        setDragPointVisible(true);
        isNeedBorder = true;
    }
    else if(change == ItemSelectedHasChanged && scene())
    {
        if(!isSelected()&&(currMouseType == MOUSE_NONE||currMouseType == MOUSE_RELEASE))
        {
            setDragPointVisible(false);
            isNeedBorder = false;
        }
    }
    else if(change  == QGraphicsItem::ItemPositionChange && scene())
    {

    }
    else if(change == QGraphicsItem::ItemPositionHasChanged && scene())
    {
    }


    return QGraphicsPolygonItem::itemChange(change,value);
}

//���ϡ����ϡ����¡�����������ʱҪ���ձ�������
//�ϡ����ҡ���������ʱֻ���ı�һ�߳���
void MyItem::procMouseState(MouseType type,PointType pointType,QPointF currPos)
{
    currMouseType = type;

    if(currMouseType == MOUSE_PRESS)
    {

    }
    else if(currMouseType == MOUSE_MOVE)
    {
        qreal leftTopX = mapToScene(boundRect.topLeft()).x();
        qreal leftTopY = mapToScene(boundRect.topLeft()).y();

        qreal rightTopX = mapToScene(boundRect.topRight()).x();
        qreal rightTopY = mapToScene(boundRect.topRight()).y();

        qreal rightBottomX = mapToScene(boundRect.bottomRight()).x();
        qreal rightBottomY = mapToScene(boundRect.bottomRight()).y();

        qreal leftBottomX = mapToScene(boundRect.bottomLeft()).x();
        qreal leftBottomY = mapToScene(boundRect.bottomLeft()).y();

        qreal w = boundRect.width();
        qreal h = boundRect.height();

        //����ƶ���ǰ����ֵ
        qreal px = currPos.x();
        qreal py = currPos.y();
        //�ƶ���x��y��w��h
        qreal tmpX,tmpY,tmpW,tmpH;
        //�ƶ������ĵ��x��y
        qreal centerX,centerY;

        qreal  factor = w / h;

        QPainterPath path;

        bool hasProcessed = false;                 //ѡ�еĿؼ�����קʱ�Ƿ����

        switch(pointType)
        {
            case TOP_LEFT:
            case TOP_RIGHT:
            case BOTTOM_LEFT:
            case BOTTOM_RIGHT:
                {
                    if(pointType ==  TOP_LEFT)
                    {
                        tmpW = w - px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        centerX = rightBottomX - tmpW/2;
                        centerY = rightBottomY - tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  TOP_RIGHT)
                    {
                        tmpW = w + px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        centerX = leftBottomX + tmpW/2;
                        centerY = leftBottomY - tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  BOTTOM_LEFT)
                    {
                        tmpW = w - px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        centerX = rightTopX - tmpW/2;
                        centerY = rightTopY + tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  BOTTOM_RIGHT)
                    {
                        tmpW = w + px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        centerX = leftTopX + tmpW/2;
                        centerY = leftTopY + tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }

                    switch(currItemType)
                    {
                        case GRA_SQUARE:
                        case GRA_RECT:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;

                        case GRA_ROUND_RECT:
                            itemPolygon.clear();

                            path.addRoundedRect(boundRect,10,10);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;

                        case GRA_CIRCLE:
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
                        case GRA_ELLIPSE:
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
                        case GRA_POLYGON:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(0.5*tmpX,-tmpY)<<
                                    QPointF(tmpX,tmpY)<<QPointF(-0.5*tmpX,tmpY);
                            hasProcessed = true;
                            break;
                    }
                    break;
                }
            case MIDDLE_RIGHT:
            case MIDDLE_LEFT:
            case TOP_MIDDLE:
            case BOTTOM_MIDDLE:
                {
                    if(currItemType == GRA_SQUARE ||currItemType == GRA_CIRCLE)
                    {
                        break;
                    }
                    if(pointType ==  MIDDLE_RIGHT)
                    {
                        tmpW = w + px /** QCOS(property.rotateDegree)*/;
                        tmpX = tmpW/2;
                        tmpY = h/2;
                        centerX = leftTopX + tmpX;
                        centerY = leftTopY + tmpY;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,h);
                    }
                    else if(pointType ==  MIDDLE_LEFT)
                    {
                        tmpW = w - px;
                        tmpX = tmpW/2;
                        tmpY = h/2;
                        centerX = rightBottomX - tmpW/2;
                        centerY = rightBottomY - tmpY;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,h);
                    }
                    else if(pointType ==  TOP_MIDDLE)
                    {
                        tmpH = h - py;
                        tmpX = w/2;
                        tmpY = tmpH/2;
                        centerX = rightBottomX - w/2;
                        centerY = rightBottomY - tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,w,tmpH);
                    }
                    else if(pointType ==  BOTTOM_MIDDLE)
                    {
                        tmpH = h + py;
                        tmpX = w/2;
                        tmpY = tmpH/2;
                        centerX = leftTopX + tmpX;
                        centerY = leftTopY + tmpY;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,w,tmpH);
                    }

                    switch(currItemType)
                    {
                        case GRA_RECT:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;

                        case GRA_ROUND_RECT:
                            itemPolygon.clear();

                            path.addRoundedRect(boundRect,10,10);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;

                        case GRA_ELLIPSE:
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
                        case GRA_POLYGON:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(0.5*tmpX,-tmpY)<<
                                    QPointF(tmpX,tmpY)<<QPointF(-0.5*tmpX,tmpY);
                            hasProcessed = true;
                            break;
                    }
                    break;
                }
        }

        if(hasProcessed)
        {
            setPos(QPointF(centerX,centerY));
            setPolygon(itemPolygon);
            procResizeItem();
            updateRotateLinePos();
            procResizeNodePort();

            property.itemRect.x = mapToScene(boundRect.topLeft()).x()+boundRect.width()/2;
            property.itemRect.y = mapToScene(boundRect.topLeft()).y()+boundRect.height()/2;
            property.itemRect.width = boundRect.width();
            property.itemRect.height = boundRect.height();

            emit propHasChanged(property);
        }
    }
    else if(currMouseType == MOUSE_RELEASE)
    {
        setSelected(true);
    }
}

//����8����ק���ε�λ��
void MyItem::procResizeItem()
{
    leftTopPoint->setPos(boundRect.topLeft());
    rightTopPoint->setPos(boundRect.topRight());
    leftBottomPoint->setPos(boundRect.bottomLeft());
    rightBottomPoint->setPos(boundRect.bottomRight());

    topPoint->setPos(QPointF(0,boundRect.topLeft().y()));
    leftPoint->setPos(QPointF(boundRect.topLeft().x(),0));
    rightPoint->setPos(QPointF(boundRect.bottomRight().x(),0));
    bottomPoint->setPos(QPointF(0,boundRect.bottomLeft().y()));
}

//��������˿ڵ�λ��
void MyItem::procResizeNodePort()
{
    QPointF newPoint;
    foreach (MyNodePort * nodePort, ports)
    {
        switch(nodePort->getDragDirect())
        {
            case DRAG_LEFT:
                          {
                               newPoint.setX(boundRect.topLeft().x());
                               newPoint.setY(nodePort->getScaleFactor()*boundRect.height() + boundRect.topLeft().y());
                          }
                           break;
            case DRAG_TOP:
                          {
                               newPoint.setX(nodePort->getScaleFactor()*boundRect.width() + boundRect.topLeft().x());
                               newPoint.setY(boundRect.topLeft().y());
                          }
                           break;
            case DRAG_RIGHT:
                          {
                               newPoint.setX(boundRect.topRight().x());
                               newPoint.setY(nodePort->getScaleFactor()*boundRect.height() + boundRect.topRight().y());
                          }
                           break;
            case DRAG_BOTTOM:
                          {
                               newPoint.setX(nodePort->getScaleFactor()*boundRect.width() + boundRect.bottomLeft().x());
                               newPoint.setY(boundRect.bottomLeft().y());
                          }
                           break;
        }

        nodePort->setPos(newPoint);
    }
}

//������ת�ؼ���ת��Ƕȵ�����
void MyItem::procRotate(MouseType mouseType,int degree)
{
    currMouseType = mouseType;

    if(currMouseType == MOUSE_MOVE)
    {
        setRotation(degree);

        property.rotateDegree = degree;
    }
    else if(currMouseType == MOUSE_RELEASE)
    {
        setSelected(true);
    }

    emit propHasChanged(property);
}

//���ÿؼ�����ʽ����
void MyItem::setProperty(ItemProperty property)
{
    this->property = property;
    setBrush(property.itemBrush);

    setRotation(property.rotateDegree);
    setZValue(property.zValue);
    setPos(QPointF (property.itemRect.x,property.itemRect.y));

    resetPolygon();

    myTextItem->setPlainText(property.content);
    myTextItem->updateFont(property.itemFont);
    myTextItem->setDefaultTextColor(property.fontColor);
    setText(myTextItem->toPlainText());

    foreach (MyNodePort * node, ports)
    {
        node->setBrush(property.itemBrush);
    }

    parentScene->update();
}

//���ش��ļ��󣬶Կؼ����������趨
void MyItem::resetPolygon()
{
    qreal tw = this->property.itemRect.width;
    qreal th = this->property.itemRect.height;
    qreal tx = tw/2;
    qreal ty = th/2;

    prepareGeometryChange();
    boundRect = QRectF(-tx,-ty,tw,th);

    itemPolygon.clear();

    switch(currItemType)
    {
            //������
            case GRA_SQUARE:
                               itemPolygon<<QPointF(-tx,-ty)<<QPointF(tx,-ty)<<
                                      QPointF(tx,ty)<<QPointF(-tx,ty);
                               break;
            //����
            case GRA_RECT:
                               itemPolygon<<QPointF(-tx,-ty)<<QPointF(tx,-ty)<<
                                       QPointF(tx,ty)<<QPointF(-tx,ty);
                               break;
           //Բ�Ǿ���
           case GRA_ROUND_RECT:
                            {
                                QPainterPath path;
                                path.addRoundedRect(boundRect,10,10);
                                itemPolygon = path.toFillPolygon();
                            }
                              break;
            //Բ��
            case GRA_CIRCLE:
                            {
                                QPainterPath path;
                                path.addEllipse(boundRect);
                                itemPolygon = path.toFillPolygon();
                            }
                               break;
            //��Բ
            case GRA_ELLIPSE:
                            {
                                QPainterPath path;
                                path.addEllipse(boundRect);
                                itemPolygon = path.toFillPolygon();
                            }
                               break;
            //����
            case GRA_POLYGON:
                            {
                               itemPolygon<<QPointF(-tx,-ty)<<QPointF(0.5*tx,-ty)<<
                                      QPointF(tx,ty)<<QPointF(-0.5*tx,ty);
                            }
                               break;
    }
    setPolygon(itemPolygon);

    procResizeItem();
    updateRotateLinePos();
    procResizeNodePort();
}

//����ת��������ת����µ�ǰ���Ե���ת�Ƕ�ֵ
void MyItem::updateRotation(int rotateValue)
{
    property.rotateDegree += rotateValue;
    property.rotateDegree = property.rotateDegree%360;
    emit propHasChanged(property);
}

MyItem::~MyItem()
{
    if(leftTopPoint)
    {
        delete leftTopPoint;
        leftTopPoint = NULL;
    }

    if(rightTopPoint)
    {
        delete rightTopPoint;
        rightTopPoint = NULL;
    }

    if(leftBottomPoint)
    {
        delete leftBottomPoint;
        leftBottomPoint = NULL;
    }

    if(rightBottomPoint)
    {
        delete rightBottomPoint;
        rightBottomPoint = NULL;
    }

    if(topPoint)
    {
        delete topPoint;
        topPoint = NULL;
    }

    if(leftPoint)
    {
        delete leftPoint;
        leftPoint = NULL;
    }

    if(rightPoint)
    {
        delete rightPoint;
        rightPoint = NULL;
    }

    if(bottomPoint)
    {
        delete bottomPoint;
        bottomPoint = NULL;
    }

    if(rotateLine)
    {
        delete rotateLine;
        rotateLine = NULL;
    }

    if(myTextItem)
    {
        delete myTextItem;
        myTextItem = NULL;
    }

    foreach (MyNodePort * node, ports)
    {
        delete node;
    }
    ports.clear();

}

/**
void ShapeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);
    op.state = QStyle::State_None;

    return QGraphicsLineItem::paint(painter, &op, widget);
}
*/
