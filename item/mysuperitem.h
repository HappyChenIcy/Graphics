/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mysuperitem.h
**��  ��: wey       Version: 1.0       Date: 2016.09.30
**��  ��:
**Others: �����������пؼ��ĸ���
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSUPERITEM_H
#define MYSUPERITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>

#include "../Header.h"

class MySuperItem : public QObject,public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    MySuperItem(GraphicsType currItemType,QGraphicsItem * parent1 = 0,QObject * parent2 = 0);
    ~MySuperItem();

    ItemProperty getProperty(){return this->property;}
    GraphicsType getType(){return this->currItemType;}
    QGraphicsItem * getParentItem(){return this->parentItem;}

protected:
    QRectF boundRect;                    //�ؼ���Χ�߿�
    ItemProperty property;               //���浱ǰ����
    QPolygonF itemPolygon;               //��ǰͼ�εĸ����������꼯��
    qreal radius;                        //�ؼ��İ뾶
    GraphicsType currItemType;           //�ؼ�������

    QGraphicsItem * parentItem;          //���ؼ�
};

#endif // MYSUPERITEM_H
