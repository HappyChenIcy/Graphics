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

#include <QGraphicsObject>

#include "ItemHeader.h"

class MyItem;

class DragLinePoint : public QGraphicsObject
{
    Q_OBJECT
public:
    DragLinePoint(const PointType pointType, MyItem * parent = 0 );

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    QRectF boundRect;
    qreal radius;
    PointType pointType;
};

#endif // DRAGLINEPOINT_H
