/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myarrow.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ����ͷ����
**Others: 1��֧����ʽ������
**        2��֧���������ؼ�������
**
**�޸���ʷ:
**20160907:wey:��Ӽ�ͷ�Ļ���
**20160908:wey:֧�ֿؼ���ת�󣬼�ͷ��Ȼ��ؼ�������
*************************************************/
#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include <QPolygonF>
#include <QDataStream>

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

    friend QDataStream & operator <<(QDataStream &,MyArrow * item);
    friend QDataStream & operator >>(QDataStream &,MyArrow * item);

private:
    MyItem  * startItem;
    MyItem  * endItem;
    QPolygonF arrowHead;

    GraphicsType type;
    ItemProperty property;

    QRectF boundRect;
};

#endif // MYARROW_H
