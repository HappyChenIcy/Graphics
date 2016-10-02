/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myiteminfo.h
**��  ��: wey       Version: 1.0       Date: 2016.10.02
**��  ��:
**Others: ������ʾMyItem��λ�ú���ת�Ƕ���Ϣ
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYITEMINFO_H
#define MYITEMINFO_H

#include "mysuperitem.h"

class MyItemInfo : public MySuperItem
{
public:
    MyItemInfo(GraphicsType type, QGraphicsItem * parent1 = 0 ,QObject* parent2 = 0);
    ~MyItemInfo();

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setRotationInfo(int degree);
    void setPosInfo(int x,int y);

private:
    QString showText;
};

#endif // MYITEMINFO_H
