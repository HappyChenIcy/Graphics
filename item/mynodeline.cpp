#include "mynodeline.h"

#include "myarrow.h"
#include "myitem.h"
#include "mypathitem.h"

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
    arrows.clear();
}

//���ĳ����ͷ�����ã�δɾ����ͷ��׼��
void MyNodeLine::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

//��������
void MyNodeLine::addPathLine(MyPathItem *item)
{
    pathLines.push_back(item);
}

//�Ƴ����е�����
void MyNodeLine::removePathLines()
{
    foreach (MyPathItem * pathItem, pathLines)
    {
        pathItem->getStartItem()->removePathLine(pathItem);
        pathItem->getEndItem()->removePathLine(pathItem);

        delete pathItem;
    }
    pathLines.clear();
}

//���ĳ�����ߵ����ã�δɾ����ͷ��׼��
void MyNodeLine::removePathLine(MyPathItem * item)
{
    int index = pathLines.indexOf(item);

    if (index != -1)
        pathLines.removeAt(index);
}

MyNodeLine::~MyNodeLine()
{

}
