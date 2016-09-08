/*************************************************
**版  权：RenGu Company
**文件名: myarrow.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 带箭头的线
**Others: 1：支持样式的设置
**        2：支持与其它控件的连接
**
**修改历史:
**20160907:wey:添加箭头的绘制
*************************************************/
#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include <QPolygonF>

#include "../Header.h"

class MyItem;

class MyArrow : public QGraphicsLineItem
{
public:
    MyArrow(MyItem  * startItem,MyItem  * endItem,QGraphicsItem * parent = 0);
    ~MyArrow();

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void updatePosition();

    MyItem * getStartItem() const{ return startItem; }
    MyItem * getEndItem() const{ return endItem; }

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

private:
    MyItem  * startItem;
    MyItem  * endItem;
    QPolygonF arrowHead;

    ItemProperty property;

    QRectF boundRect;
};

#endif // MYARROW_H
