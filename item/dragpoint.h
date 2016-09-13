/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: dragpoint.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ���ŵ�
**Others: 1���������ſؼ�
**
**�޸���ʷ:
**
*************************************************/
#ifndef DRAGPOINT_H
#define DRAGPOINT_H

#include <QGraphicsObject>

#include "ItemHeader.h"

class MyItem;

class DragPoint : public QGraphicsObject
{
    Q_OBJECT
public:
    DragPoint(const PointType pointType, MyItem * parent = 0 );
    void updatePos(PointType type,QPointF point);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void currMouseState(MouseType,PointType,QPointF);
    void resizeItemSize();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QRectF boundRect;

    int radius;

    PointType pointType;
};

#endif // DRAGPOINT_H
