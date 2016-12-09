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
**20160929:wey:����������ݼ�(Ctrl+Shift+L)
**             �޸��ؼ����ֱ�ߣ������������Item��Text�ϣ������޷������ֱ������
**             �޸��ؼ��򿪵�����뱣����Ȳ�һ������
**             �޸��ļ���ʱ�������������Ӳ�������
**20161002:wey:�����ʾitemλ�ú���ת��ϢMyItemInfo
**20161003:wey:�޸����MyItemInfo���������ʾ���³��������⡣
**20161008:wey:�޸����ش򿪺���ʱ����δ�������
**20161025:wey:�޸���ͬһ�ؼ����ɾ�����ߺ󣬵��±�������
**20161026:wey:���ȷ���Ҽ��˵�ֻѡ��һ���ؼ�
**20161208:wey:�������
*************************************************/
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class MyTextItem;
class MyItem;
class MyPathItem;
class MyNodePort;
class MyItemInfo;
class MyNodeLine;
class MyArrow;
class DragLinePoint;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QObject * parent = 0 );
    void addItem(QGraphicsItem *item);
    void addItem(CutInfo cutInfo,bool isCopy = false);
    void addItem(QList<CutInfo *> & cutInfos);
    void addItem(GraphicsType type,QPointF pos);
    void removeItem(QGraphicsItem *item);
    void setId(QString id){this->uuid = id;}
    QString getId(){return this->uuid;}
    void setAssociation(bool flag);
    bool getAssociation(){return this->isSceneAssociated;}
    void clear();
    ~MyScene();

signals:
    void resetItemAction();
    void deleteKeyPress();
    void ctrlLockKeyPress();
    void ctrlUnLockKeyPress();
    void itemSizeChanged(int size);
    void itemPropChanged(ItemProperty);
    void editCurrItem();
    void editCurrPort();
    void ctrlPropEditKeyPress();

private slots:
    void respTextLostFocus(MyTextItem * item);
    void itemSelectedChanged();
    void showItemRotationInfo(MyItem *);
    void showItemPosInfo(MyItem *);
    void respClearOtherSelectedItems(MyItem*);

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
    void createItemInfo();
    MyArrow *createArrow(LineType type, MyNodeLine * startNode,MyNodeLine * endNode);
    MyPathItem * createPathItem(LineType type, MyNodeLine *startNode, MyNodeLine *endNode);
    QRectF getHorizonalRoundedRect(MyItem *item);
    void resetItemSelection();

    QGraphicsLineItem * insertTmpLine;
    MyPathItem * insertTmpPath;
    MyItemInfo * myItemInfo;           //�ؼ���Ϣ

    QList<MyItem*> localItems;
    QList<MyNodePort*> localNodeports;

    bool isLocalFileOpened;            //�Ƿ��Ǳ����ļ���
    bool isDragLine;                   //�Ƿ��ڴ�nodeline���������״̬
    bool isDragPathLine;               //�Ƿ���������ߵ�״̬
    bool isClear;                      //�Ƿ������״̬�����ڿ���clear()��MyIemInfo��Ӱ��
    bool isSceneAssociated;            //��ͼ�Ƿ񱻹��������Ϊfalse������ʱ��Ҫ�������ڣ�
    QString uuid;
    QList<MyItem *> mouseItems;        //��ק������ʱ����¼���λ���²�����item
    QString startMouseItemId;          //��ק��ʱ����DragLinePoint�ĸ�item��id
};

#endif // MYSCENE_H
