#include "mynodeport.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "myitem.h"
#include "myarrow.h"
#include "../actionmanager.h"

using namespace Graphics;

MyNodePort::MyNodePort(MyItem *parentItem):
    parentMyItem(parentItem),
    QGraphicsObject(parentItem)
{
//    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);

    radius = 13;

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

//������ӵļ�ͷ
void MyNodePort::addArrow(MyArrow *arrow)
{
    arrows.push_back(arrow);
}

//ɾ���˿ؼ�ʱ���Ƴ����еļ�ͷ
void MyNodePort::removeArrows()
{
    foreach (MyArrow *arrow, arrows)
    {
        arrow->getStartNodePort()->removeArrow(arrow);
        arrow->getEndNodePort()->removeArrow(arrow);
//        scene()->removeItem(arrow);
        delete arrow;
    }
}

void MyNodePort::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

//���ÿؼ�����ʽ����
void MyNodePort::setProperty(ItemProperty property)
{
    this->property = property;
}

MyNodePort::~MyNodePort()
{
//    if(nodePortRightMenu)
//    {
//        delete nodePortRightMenu;
//        nodePortRightMenu = NULL;
//    }
}
