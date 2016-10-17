/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: sceneitempickup.h
**��  ��: wey       Version: 1.0       Date: 2016.10.11
**��  ��:
**Others:�����ؼ���ȡ
**      ��1��:��ȡ��ǰ�����пؼ�������
 *      ��2��:�ж��Ƿ������ʼ�ͽ���
**�޸���ʷ:
**20161011:wey:������ʼ�汾��ʵ�ֿؼ�ѡȡ����ʼ����֤
*************************************************/
#ifndef SCENEITEMPICKUP_H
#define SCENEITEMPICKUP_H

#include "SimulateHeader.h"
class QGraphicsItem;

class SceneItemPickup
{
public:
    static SceneItemPickup * instance();

    ReturnType pickUpItems(QList<QGraphicsItem *> &existedItems);
    ReturnType validateStartEnd(QList<QGraphicsItem *> &existedItems);

private:
    SceneItemPickup();
    static SceneItemPickup * pickup;
};

#endif // SCENEITEMPICKUP_H
