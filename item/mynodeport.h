/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mynodeport.h
**��  ��: wey       Version: 1.0       Date: 2016.09.18
**��  ��: �ؼ��ϵĶ˿�
**Others:1.��Ҫ֧������
**       2.֧���ƶ�����̬���
**       3.֧������
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYNODEPORT_H
#define MYNODEPORT_H

#include <QGraphicsObject>
#include <QBrush>

#include "ItemHeader.h"

class MyItem;

class MyNodePort : public QGraphicsObject
{
    Q_OBJECT
public:
    MyNodePort(QGraphicsItem * parentItem);
    ~MyNodePort();

    QRectF boundingRect() const;

    void setPos(const QPointF &pos);

    void setBrush(const QBrush & brush);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDragDirect(DragDirect direct);
    DragDirect getDragDirect(){return this->dragDirect;}

    void setScaleFactor(qreal scaleFactor);
    qreal getScaleFactor(){return this->scaleFactor;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void portPosChanged(MouseType,QPointF);

private:
    QRectF boundRect;
    int radius;

    QPolygonF itemPolygon;             //��ǰͼ�εĸ����������꼯��

    DragDirect dragDirect;

    qreal scaleFactor;                 //�����λ���൱�ڵ�ǰһ�����ڵı���

    QBrush brush;

};

#endif // MYNODEPORT_H
