/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myscene.h
**��  ��: wey       Version: 1.0       Date: 2016.09.08
**��  ��: �Զ��峡��
**Others: 1�����ڴ�����װ���������ӿؼ������ӿؼ��Ĺ���
**
**
**�޸���ʷ:
**20160907:wey:��Ӷ�delete�����¼���֧��ѡ��ɾ��
**20160908:wey:��Ӷ�item�����ļ��
**20160913:wey:�޸����ƻ���к�ťδ��ʱ����
**20160921:wey:�޸����������пؼ�IDͬ������δ��������
**             �޸�������ֺ��޷����С������뿨�����⡾sceneδ����ļ����¼�Ҫ����ϵͳ����
**             �����������
**20160925:wey:���Ӽ��̶�����(Ctrl+L)/����(Ctrl+U)��֧��
**20160927:wey:�����Զ��屳��
*************************************************/
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class QMenu;
class MyTextItem;
class MyItem;
class MyPathItem;
class MyNodePort;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QMenu * menu,QObject * parent = 0 );
    void addItem(QGraphicsItem *item);
    void addItem(CutInfo cutInfo,bool isCopy = false);
    void addItem(QList<CutInfo *> & cutInfos);
    void addItem(GraphicsType type,QPointF pos);
    void removeItem(QGraphicsItem *item);
    ~MyScene();

signals:
    void resetItemAction();
    void deleteKeyPress();
    void ctrlLockKeyPress();
    void ctrlUnLockKeyPress();
    void itemSizeChanged(int size);
    void itemPropChanged(ItemProperty);
    void editCurrItem();

private slots:
    void respTextLostFocus(MyTextItem * item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    int findItemById(QList<MyItem *> &localItem, QString Id);
    int findItemById(QList<MyNodePort *> &localNode, QString Id);
    void addMyItemConnect(MyItem * item);
    void addMyTextConnect(MyTextItem  * item);

    QMenu * rightMenu;

    QGraphicsLineItem * insertTmpLine;
    MyPathItem * insertTmpPath;

    QList<MyItem*> localItems;
    QList<MyNodePort*> localNodeports;
};

#endif // MYSCENE_H
