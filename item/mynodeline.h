/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mynodeline.h
**��  ��: wey       Version: 1.0       Date: 2016.10.08
**��  ��:�ɲ��������߿ؼ��ĸ��࣬���ڲ�����ͷ
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYNODELINE_H
#define MYNODELINE_H

#include "mysuperitem.h"

class MyArrow;
class MyItem;

class MyNodeLine : public MySuperItem
{
public:
    MyNodeLine(GraphicsType type, MyItem * parent1 = 0 ,QObject* parent = 0);
    ~MyNodeLine();
    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);
    QList<MyArrow *> & getArrows(){return this->arrows;}

protected:
    QList<MyArrow *> arrows;           //������ӵļ�ͷ
};

#endif // MYNODELINE_H
