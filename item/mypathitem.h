/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypathitem.h
**��  ��: wey       Version: 1.0       Date: 2016.09.09
**��  ��: ���ڻ�������
**Others:
**
**�޸���ʷ:
**20161209:wey:
*************************************************/
#ifndef MYPATHITEM_H
#define MYPATHITEM_H

#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QObject>

class MyNodePort;
class MyTextItem;
class MyNodeLine;

#include "../Header.h"

class MyPathItem : public QObject, public QGraphicsPathItem
{
public:
    MyPathItem(QObject * parent = 0,QGraphicsItem * parent1 = 0);
    ~MyPathItem();

    void setStartItem(MyNodeLine * startItem);
    MyNodeLine * getStartItem() const{ return startItem; }
    void setStartItemID(const QString id);

    void setEndItem(MyNodeLine * endItem);
    MyNodeLine * getEndItem() const{ return endItem; }
    void setEndItemID(const QString id);

    void setStartPoint(QPointF startPoint);
    QPointF getStartPoint(){return this->startPosPoint;}

    void setEndPoint(QPointF endPoint);
    QPointF getEndPoint(){return this->endPosPoint;}

    void setStartPointType(PointType type);
    void setEndPointType(PointType type);

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}
    GraphicsType getType(){return this->currItemType;}

    void setPath(const QPainterPath &path);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect()const;

    QList<QPointF> getPathPoints();
    void setPathPoints(QList<QPointF> & points);

    QPainterPath  shape() const;

    void updateCurrItemPos();
    void setLineType(LineType lineType);

    friend QDataStream & operator <<(QDataStream &,MyPathItem * item);
    friend QDataStream & operator >>(QDataStream &,MyPathItem * item);

private:
    void countPathItemPoints();
    void getNewLine();

    MyNodeLine * startItem;
    MyNodeLine * endItem;

    QPointF startPosPoint,endPosPoint; //������scene�е�λ��

    GraphicsType currItemType;
    ItemProperty property;             //���浱ǰ����
    GraphicsType type;

    QRectF boundRect;                  //�����ľ���
    QPainterPath painterPath;          //��ǰ·��

    QList<QPointF> points;             //���������յ�����������ǰ���ߵ�ÿ���ؼ���(���ߵ�)
};

#endif // MYPATHITEM_H
