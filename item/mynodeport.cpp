#include "mynodeport.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "myitem.h"
#include "myarrow.h"
#include "mytextitem.h"
#include "mypathitem.h"
#include "../manager/actionmanager.h"

using namespace Graphics;

QDataStream & operator <<(QDataStream & dataStream,NodePortProperty & prop)
{
    dataStream<<prop.portType<<prop.content<<prop.itemBrush<<prop.direct
             <<prop.scaleFactor<<prop.startItemID<<prop.parentItemID;

    return dataStream;
}

QDataStream & operator >>(QDataStream & dataStream,NodePortProperty & prop)
{
    int direct;
    int type;

    dataStream>>type>>prop.content>>prop.itemBrush>>direct
             >>prop.scaleFactor>>prop.startItemID>>prop.parentItemID;

    prop.portType = (GraphicsType)type;
    prop.direct = (DragDirect)direct;

    return dataStream;
}

QDataStream & operator <<(QDataStream & dataStream,MyNodePort * nodePort)
{
    int type = nodePort->currItemType;
    dataStream<<type<<nodePort->nodeProperty;

#ifdef ADD_STATE_MODEL
    switch(nodePort->nodeProperty.portType)
    {
        case GRA_NODE_CIRCLE:
                            dataStream<<nodePort->statePortProp;
                            break;

        case GRA_NODE_TRIANGLE_OUT:
        case GRA_NODE_HALF_CIRCLE_OUT:
                            dataStream<<nodePort->stateInOutProp;
                            break;
    }
#endif

    return dataStream;
}

QDataStream & operator >>(QDataStream & dataStream,MyNodePort * nodePort)
{
    int type;
    NodePortProperty property;
    dataStream>>type>>property;

#ifdef ADD_STATE_MODEL
    switch(property.portType)
    {
        case GRA_NODE_CIRCLE:
                            dataStream>>nodePort->statePortProp;
                            break;

        case GRA_NODE_TRIANGLE_OUT:
        case GRA_NODE_HALF_CIRCLE_OUT:
                            dataStream>>nodePort->stateInOutProp;
                            break;
    }
#endif

    nodePort->currItemType = (GraphicsType)type;
    nodePort->nodeProperty = property;
    return dataStream;
}

#ifdef ADD_STATE_MODEL
//�Գ�ʼ���˿ڵı���
QDataStream & operator<<(QDataStream & stream,StatePortProperty &prop)
{
    stream<<prop.portName<<prop.portType;
    return stream;
}

QDataStream & operator>> (QDataStream & stream,StatePortProperty &prop)
{
    stream>>prop.portName>>prop.portType;
    return stream;
}

//����/���˿�����
QDataStream & operator<< (QDataStream & stream,StateInOutProperty & prop)
{
    stream<<prop.portName<<prop.portType;
    stream<<prop.props.size();

    for(int i=0;i<prop.props.size();i++)
    {
        StatePortProperty sprop = prop.props.at(i);
        stream<<sprop;
    }
    return stream;
}

QDataStream & operator>> (QDataStream & stream,StateInOutProperty & prop)
{
    stream>>prop.portName>>prop.portType;

    int size = 0;
    stream>>size;

    for(int i=0;i<size;i++)
    {
        StatePortProperty sprop;
        stream>>sprop;
        prop.props.append(sprop);
    }

    return stream;
}
#endif

MyNodePort::MyNodePort(GraphicsType type, MyItem *parentItem, QObject *parent1):
    MyNodeLine(type,parentItem,parent1)
{
    setFlag(QGraphicsItem::ItemIsSelectable,parentItem->isMoveable());

    radius = 8;
    currItemType = type;
    nodeProperty.portType = type;
    nextDirect = DRAG_NONE;
    arrivalLimitRang = false;
    isMoveable = true;

    nodeProperty.parentItemID = parentItem->getProperty().startItemID;     //���游�ڵ��ID��

    prepareGeometryChange();
    boundRect = QRectF(-radius,-radius,2*radius,2*radius);

    initWidget(parentItem);
}

//��ʼ���ؼ�
void MyNodePort::initWidget(MyItem *parentItem)
{
    //������Ϣ
    myTextItem = new MyTextItem(GRA_TEXT,this);
    myTextItem->setTextExistType(TEXT_CHILD);
    connect(myTextItem,SIGNAL(updateTextGeometry()),this,SLOT(updateTextPosByDirect()));

    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    myTextItem->setFlag(ItemIsFocusable,false);
    myTextItem->cleartText();

    initNodePortRightMenu();
    connect(this,SIGNAL(deletePort(MyNodePort*)),parentItem,SLOT(procDeleteNodePort(MyNodePort*)));
    connect(this,SIGNAL(editPort(MyNodePort*)),parentItem,SLOT(procEditNodePort(MyNodePort*)));
    connect(this,SIGNAL(dClickEditPort(MyNodePort*)),parentItem,SLOT(procDoubleClickEditNodePort(MyNodePort*)));
    connect(this,SIGNAL(portPosChanged(MouseType,QPointF)),parentItem,SLOT(procPortChanged(MouseType,QPointF)));
}

//���ö˿�����ķ���
void MyNodePort::setDragDirect(DragDirect direct)
{
    nodeProperty.direct = direct;
    prepareGeometryChange();

    itemPolygon.clear();
    if(currItemType == GRA_NODE_PORT)
    {
        itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)
                 <<QPointF(radius,radius)<<QPointF(-radius,radius);
    }
    else if(currItemType == GRA_NODE_CIRCLE)
    {
        QPainterPath path;
        path.addEllipse(QRectF(-radius,-radius,2*radius,2*radius));
        itemPolygon = path.toFillPolygon();
    }
    else
    {
        switch(direct)
        {
            case DRAG_LEFT:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(-radius,-radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),-90,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,-radius);
                                    path.lineTo(radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),-90,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(radius,-radius)<<QPointF(-radius,0)
                                                <<QPointF(radius,radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,0)
                                                <<QPointF(-radius,radius);
                                }
                                break;
                            }
            case DRAG_TOP:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,-radius);
                                    path.lineTo(-radius,-radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),180,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),180,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(radius,radius)<<QPointF(0,-radius)
                                                <<QPointF(-radius,radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(0,radius)
                                                <<QPointF(radius,-radius);
                                }
                                break;
                            }
            case DRAG_RIGHT:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,radius);
                                    path.lineTo(radius,-radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),90,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(-radius,-radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),-90,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,0)
                                                <<QPointF(-radius,radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(radius,-radius)<<QPointF(-radius,0)
                                                <<QPointF(radius,radius);
                                }
                                break;
                            }
            case DRAG_BOTTOM:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),180,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(-radius,-radius);
                                    path.lineTo(radius,-radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),0,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(0,radius)
                                                <<QPointF(radius,-radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(-radius,radius)<<QPointF(0,-radius)
                                                <<QPointF(radius,radius);
                                }
                                break;
                            }
        }
    }
    setPolygon(itemPolygon);

    updateTextPosByDirect();
}

//����յ�
void MyNodePort::setNextDirect(DragDirect direct)
{
    this->nextDirect = direct;
    arrivalLimitRang = true;
}

//�����Ƿ��Ѿ�����߽��
void MyNodePort::setArrivalLimitRang(bool flag)
{
    arrivalLimitRang = flag;
}

//������ʾ�����ĵ�
void MyNodePort::setPos(const QPointF &pos)
{
    QGraphicsPolygonItem::setPos(pos);
}

//��������λ��������߳��ȵı���
void MyNodePort::setScaleFactor(qreal scaleFactor)
{
    nodeProperty.scaleFactor = scaleFactor;
}

QRectF MyNodePort::boundingRect()const
{
    return boundRect;
}

void MyNodePort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    painter->setBrush(nodeProperty.itemBrush);

    painter->drawPolygon(itemPolygon);

    painter->restore();
}

void MyNodePort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_PRESS,event->pos());
    QGraphicsPolygonItem::mousePressEvent(event);
}

void MyNodePort::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_MOVE,event->pos());
    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void MyNodePort::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_RELEASE,event->pos());
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void MyNodePort::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit dClickEditPort(this);
    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
}

void MyNodePort::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    nodePortRightMenu->exec(event->screenPos());
}

//��ʼ���˿��Ҽ��˵�
void MyNodePort::initNodePortRightMenu()
{
    nodePortRightMenu = new QMenu;

    QAction * editAction = new QAction(QIcon(":/images/edit.png"),"�༭�ڵ�",nodePortRightMenu);
    QAction * deleteAction = new QAction(QIcon(":/images/delete.png"),"ɾ��",nodePortRightMenu);

    nodePortRightMenu->addAction(editAction);
    nodePortRightMenu->addSeparator();
    nodePortRightMenu->addAction(deleteAction);

    connect(editAction,SIGNAL(triggered()),this,SLOT(respEditAction()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(respDeleteAction()));
}

//��Ӧɾ��
void MyNodePort::respDeleteAction()
{
    emit deletePort(this);
}

//��Ӧ�༭
void MyNodePort::respEditAction()
{
    emit editPort(this);
}

//��ȡ�˿ڶ����ڵ�ǰ����ϵ�е�ֵ(Ҫ˳ʱ�뱣��)
QPolygonF MyNodePort::getScenePolygon()
{
    QPointF topLeftPoint = getParentItem()->mapToScene(boundingRect().topLeft() + pos());
    QPointF topRightPoint = getParentItem()->mapToScene(boundingRect().topRight() + pos());
    QPointF bottomRightPoint = getParentItem()->mapToScene(boundingRect().bottomRight() + pos());
    QPointF bottomLeftPoint = getParentItem()->mapToScene(boundingRect().bottomLeft() + pos());

    QPolygonF polygonF;
    polygonF<<topLeftPoint<<topRightPoint<<bottomRightPoint<<bottomLeftPoint;

    return polygonF;
}

//���ÿؼ�����ʽ����
void MyNodePort::setProperty(ItemProperty property)
{
    nodeProperty.itemBrush = property.itemBrush;

    update();
}

//���¶˿ڵ�ID�ţ���Ҫ���ڱ��ش�ʱ����ԭʼ��ID��
void MyNodePort::updatePortID(QString portId)
{
    nodeProperty.startItemID = portId;
}

//���½ڵ�����
void MyNodePort::setNodeProperty(NodePortProperty prop)
{
    this->nodeProperty = prop;
}

//�����Ƿ�����ƶ�
void MyNodePort::setMoveable(bool isMoveable)
{
    isMoveable = isMoveable;
    setFlag(QGraphicsItem::ItemIsSelectable,isMoveable);
}

QString MyNodePort::getText()
{
    return myTextItem->toPlainText();
}

//����������Ϣ��ͬʱ����textItem��Nodeport�е�λ��
void MyNodePort::setText(QString text)
{
    nodeProperty.content = text;
    myTextItem->setPlainText(text);
    updateTextPosByDirect();

#ifdef ADD_STATE_MODEL
    statePortProp.portName = text;
    stateInOutProp.portName = text;

#endif
}

//���ݵ�ǰ�ؼ����ڵ�Direct����̬�ĸı����ֵ�λ��
void MyNodePort::updateTextPosByDirect()
{
    QRectF rectF = myTextItem->getBoundRect();

    switch(nodeProperty.direct)
    {
          case DRAG_LEFT:
                            myTextItem->setPos(-(radius*2 + rectF.width()),-rectF.height());
                          break;
          case DRAG_TOP:
                            myTextItem->setPos(-rectF.width()/2,-(radius*2+rectF.height()));
                          break;
          case DRAG_RIGHT:
                            myTextItem->setPos(radius,-rectF.height());
                          break;
          case DRAG_BOTTOM:
                            myTextItem->setPos(-rectF.width()/2,radius);
                          break;
          default:
                    break;
    }
}

#ifdef ADD_STATE_MODEL
void MyNodePort::setPortProp(StatePortProperty &prop)
{
    statePortProp.portName = prop.portName;
    statePortProp.portType = prop.portType;
    setText(statePortProp.portName);
}

void MyNodePort::setPortInoutProp(StateInOutProperty &prop)
{
    stateInOutProp.portName = prop.portName;
    stateInOutProp.portType = prop.portType;
    stateInOutProp.props = prop.props;
    setText(stateInOutProp.portName);
}
#endif

//��������������λ��
void MyNodePort::updateLinePos()
{
    foreach(MyPathItem * pathItem,pathLines)
    {
        pathItem->updateCurrItemPos();
    }
}

//���˿ڵķ���ı����Ҫ�������ߵ����/�յ��ItemType
void MyNodePort::updatePathQuoteType()
{
    foreach(MyPathItem * pathItem,pathLines)
    {
        if(pathItem->getStartItem()->getProperty().startItemID == property.startItemID)
        {
            pathItem->setStartPointType(nodeProperty.direct);
        }
        else if(pathItem->getEndItem()->getProperty().startItemID == property.startItemID)
        {
            pathItem->setEndPointType(nodeProperty.direct);
        }
        pathItem->updateCurrItemPos();
    }
}

MyNodePort::~MyNodePort()
{
}
