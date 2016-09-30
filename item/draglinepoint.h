/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: draglinepoint.h
**��  ��: wey       Version: 1.0       Date: 2016.09.30
**��  ��:
**Others: ��ק�����ڵ�
**
**�޸���ʷ:
**
*************************************************/
#ifndef DRAGLINEPOINT_H
#define DRAGLINEPOINT_H

#include <QObject>
#include <QGraphicsPolygonItem>

#include "ItemHeader.h"

class MyItem;

class DragLinePoint : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    DragLinePoint(const PointType pointType, MyItem * parent1 = 0 ,QObject* parent = 0);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    QRectF boundRect;
    qreal radius;
    PointType pointType;
    QPolygonF polygon;
};

#endif // DRAGLINEPOINT_H
