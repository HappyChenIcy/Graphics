#include "mynodeline.h"

#include "myarrow.h"
#include "myitem.h"

MyNodeLine::MyNodeLine(GraphicsType type, MyItem *parent1, QObject *parent2):
    MySuperItem(type,parent1,parent2)
{

}

//������ӵļ�ͷ
void MyNodeLine::addArrow(MyArrow *arrow)
{
    arrows.push_back(arrow);
}

//ɾ���˿ؼ�ʱ���Ƴ����еļ�ͷ
void MyNodeLine::removeArrows()
{
    foreach (MyArrow *arrow, arrows)
    {
        arrow->getStartItem()->removeArrow(arrow);
        arrow->getEndItem()->removeArrow(arrow);

        delete arrow;
    }
}

//���ĳ����ͷ�����ã�δɾ����ͷ��׼��
void MyNodeLine::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

MyNodeLine::~MyNodeLine()
{

}
