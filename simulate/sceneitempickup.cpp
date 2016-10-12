#include "sceneitempickup.h"

#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../item/myitem.h"

SceneItemPickup * SceneItemPickup::pickup = NULL;

SceneItemPickup::SceneItemPickup()
{

}

SceneItemPickup * SceneItemPickup::instance()
{
    if(!pickup)
    {
        pickup = new SceneItemPickup();
    }
    return pickup;
}

//��ȡ�����д��ڵĿؼ�,ֻҪ��ȡMyItem���߶�(MyArrow)
ReturnType SceneItemPickup::pickUpItems(QList<QGraphicsItem *> &existedItems)
{
    //�������ɷ���
    if(MyGraphicsView::instance()->scene() == NULL)
    {
        return SCENE_NOT_EXIST;
    }

    //��������Ϊ��
    if(MyGraphicsView::instance()->scene()->items().size() == 0)
    {
        return SCENE_EMPTY;
    }

    QList<QGraphicsItem *> items = MyGraphicsView::instance()->scene()->items();

    //��1��:��ȡ��ǰ�����пؼ�������
    QString typeId;
    foreach(QGraphicsItem * item,items)
    {
        typeId = TYPE_ID(*item);
        if(typeId == TYPE_ID(MyItem) || typeId == TYPE_ID(MyArrow))
        {
            existedItems.push_back(item);
        }
    }

    return validateStartEnd(existedItems);
}

//��֤��ʼ�ͽ����Ƿ���ȷ
ReturnType SceneItemPickup::validateStartEnd(QList<QGraphicsItem *> &existedItems)
{
    //��2��:�ж��Ƿ������ʼ�ͽ���
    int startItemCount = 0;
    int endItemCount = 0;

    foreach(QGraphicsItem * item,existedItems)
    {
        if(TYPE_ID(*item) == TYPE_ID(MyItem))
        {
            MyItem * tmpItem = dynamic_cast<MyItem *>(item);
            if(tmpItem && tmpItem->getType() == GRA_ROUND_RECT)
            {
                QString text = tmpItem->getText();
                if(text == "��ʼ" || text.toLower().contains("start"))
                {
                    startItemCount++;
                }
                else if(text == "����" || text.toLower().contains("end"))
                {
                    endItemCount++;
                }
            }
        }
    }

    if(startItemCount == 0)
    {
        return SCENE_WITHOUT_START;
    }
    else if(startItemCount > 1)
    {
        return SCENE_MORE_START;
    }

    if(endItemCount == 0)
    {
        return SCENE_WITHOUT_END;
    }
    else if(endItemCount > 1)
    {
        return SCENE_MORE_END;
    }

    return RETURN_SUCCESS;
}
