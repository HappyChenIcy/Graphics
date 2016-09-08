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
**
*************************************************/
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class QMenu;
class MyTextItem;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QMenu * menu,QObject * parent = 0 );
    ~MyScene();

signals:
    void resetItemAction();
    void deleteKeyPress();

private slots:
    void respTextLostFocus(MyTextItem * item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QMenu * rightMenu;

    QGraphicsLineItem * insertTmpLine;

};

#endif // MYSCENE_H
