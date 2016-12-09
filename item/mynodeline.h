/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mynodeline.h
**��  ��: wey       Version: 1.0       Date: 2016.10.08
**��  ��:�ɲ��������߿ؼ��ĸ��࣬���ڲ�����ͷ
**Others:
**
**�޸���ʷ:
**20161208:wey:�������
*************************************************/
#ifndef MYNODELINE_H
#define MYNODELINE_H

#include "mysuperitem.h"

class MyArrow;
class MyItem;
class MyPathItem;

class MyNodeLine : public MySuperItem
{
public:
    MyNodeLine(GraphicsType type, MyItem * parent1 = 0 ,QObject* parent = 0);
    ~MyNodeLine();
    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);

    void addPathLine(MyPathItem * item);
    void removePathLines();
    void removePathLine(MyPathItem * item);

    QList<MyArrow *> & getArrows(){return this->arrows;}

protected:
    QList<MyArrow *> arrows;           //������ӵļ�ͷ
    QList<MyPathItem *> pathLines;     //������ӵ�����
};

#endif // MYNODELINE_H
