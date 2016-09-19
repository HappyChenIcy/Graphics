#include "mynodeport.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QDebug>

#include "myitem.h"
#include "../actionmanager.h"

using namespace Graphics;

MyNodePort::MyNodePort(MyItem *parentItem):
    QGraphicsObject(parentItem)
{
//    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);

    radius = 15;

    boundRect = QRectF(-radius,-radius,2*radius,2*radius);

    initNodePortRightMenu();
    connect(this,SIGNAL(deletePort(MyNodePort*)),parentItem,SLOT(procDeleteNodePort(MyNodePort*)));
    connect(this,SIGNAL(editPort(MyNodePort*)),parentItem,SLOT(procEditNodePort(MyNodePort*)));
}

//���ö˿�����ķ���
void MyNodePort::setDragDirect(DragDirect direct)
{
    this->dragDirect = direct;
}

//������ʾ�����ĵ�
void MyNodePort::setPos(const QPointF &pos)
{
    QGraphicsObject::setPos(pos);
}

//��������λ��������߳��ȵı���
void MyNodePort::setScaleFactor(qreal scaleFactor)
{
    this->scaleFactor = scaleFactor;
}

//���ö˿ڵĻ�ˢ
void MyNodePort::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

QRectF MyNodePort::boundingRect()const
{
    return boundRect;
}

void MyNodePort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setBrush(brush);

    painter->drawRect(boundRect);

    painter->restore();
}

void MyNodePort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_PRESS,event->pos());
    QGraphicsObject::mousePressEvent(event);
}

void MyNodePort::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_MOVE,event->pos());
    QGraphicsObject::mouseMoveEvent(event);
}

void MyNodePort::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_RELEASE,event->pos());
    QGraphicsObject::mouseReleaseEvent(event);
}

void MyNodePort::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit editPort(this);
    QGraphicsObject::mouseDoubleClickEvent(event);
}

void MyNodePort::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    nodePortRightMenu->exec(event->screenPos());
}

//��ʼ���˿��Ҽ��˵�
void MyNodePort::initNodePortRightMenu()
{
    nodePortRightMenu = new QMenu;


    MyAction * editAction = ActionManager::instance()->crateAction(Constants::NODE_EDIT_ID,QIcon(":/images/edit.png"),"�༭�ڵ�");

    MyAction * deleteAction = ActionManager::instance()->crateAction(Constants::NODE_DELETE_ID,QIcon(":/images/delete.png"),"ɾ��");

    nodePortRightMenu->addAction(ActionManager::instance()->action(Constants::NODE_EDIT_ID));
    nodePortRightMenu->addSeparator();
    nodePortRightMenu->addAction(ActionManager::instance()->action(Constants::NODE_DELETE_ID));

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

MyNodePort::~MyNodePort()
{

}
