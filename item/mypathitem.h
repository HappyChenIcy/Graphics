/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypathitem.h
**��  ��: wey       Version: 1.0       Date: 2016.09.09
**��  ��: ���ڻ�������
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYPATHITEM_H
#define MYPATHITEM_H

#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QObject>

class MyItem;

#include "../Header.h"

class MyPathItem : public QObject, public QGraphicsPathItem
{
public:
    MyPathItem(MyItem  * startItem,MyItem  * endItem,QObject * parent = 0,QGraphicsItem * parent1 = 0);
    ~MyPathItem();

    MyItem * getStartItem() const{ return startItem; }
    MyItem * getEndItem() const{ return endItem; }

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}
    GraphicsType getType(){return this->currItemType;}

    void setPath(const QPainterPath &path);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect()const;

    QPainterPath  shape() const;

private:
    MyItem  * startItem;
    MyItem  * endItem;

    GraphicsType currItemType;
    ItemProperty property;             //���浱ǰ����

    QRectF boundRect;                  //�����ľ���

    QPainterPath painterPath;          //��ǰ·��

    QList<QPointF> points;

};

#endif // MYPATHITEM_H
