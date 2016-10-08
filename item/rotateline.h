/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: rotateline.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: �ؼ���ת���Ƶ�
**Others: 1�����ڿ��ƿؼ�����ת
**
**�޸���ʷ:
**20160922:wey:���״̬������������ͼ��ı�
**20160930:wey:����������QGraphicsPolygonItem
**20161005:wey:ʵ�ֿؼ���ת��!!!�ش���¡�
*************************************************/
#ifndef ROTATELINE_H
#define ROTATELINE_H

#include <QPixmap>

#include "mysuperitem.h"
#include "ItemHeader.h"

class RotateLine : public MySuperItem
{
    Q_OBJECT
public:
    RotateLine(GraphicsType type,QGraphicsItem * parent = 0,QObject * parent1 = 0);
    ~RotateLine();

    void setMoveable(bool moveable);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void rotateItem(MouseType,qreal);

private:
    QPointF startPoint;

    qreal rotateDegree;
    bool isEnter;
    bool isMoveable;

    QPixmap pixmap;
    QPixmap rotatePixmap;
};

#endif // ROTATELINE_H
