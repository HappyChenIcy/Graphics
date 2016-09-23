/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mygraphicsview.h
**��  ��: wey       Version: 1.0       Date: 2016.09.18
**��  ��:
**Others:
**
**�޸���ʷ:
**20160919:wey:��ӵ���ģʽ
**             ��Ӷ˿ڱ༭ҳ��
**20160920:wey:�޸�����˿ڷ����ڿؼ�����Ч����
**20160922:wey:��������view�Ƿ���ק
**             ������ռ��а幦��
**20160923:wey:������������ؼ�ѡ��
**             ����Ctrl+������С��ͼ
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

#include "../Header.h"

class MyNodePort;
class NodeEditDialog;
class MyScene;
class MainWindow;
class QMenu;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    static MyGraphicsView * instance();

    MyGraphicsView(MainWindow * parent = 0);
    ~MyGraphicsView();

    void showNodePortEdit(MyNodePort * nodePort);

    void clearItems();
    void addContextMenuItem();

    MyScene * scene();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void initToolBox(int selectedNum,ItemProperty property);
    void itemPropChanged(ItemProperty);
    void zoomIn();
    void zoomOut();

private slots:
    void undoAndRedoItem();
    void cutItem();
    void copyItem();
    void pasteItem();
    void clearPasteItem();

    void rotateItem();
    void bringZItem();
    void lockAndunlockItem();
    void deleteItem();
    void setViewDragEnable(bool enable);

    void sceneScaled(int currScale);
    void updateActions();
    void editTextItem();
    void respPropertyUpdate(ItemProperty property);

private:
    void initView();

    static MyGraphicsView * viewManager;

    NodeEditDialog * nodeEdit;
    MyScene * myScene;
    MainWindow * parentWindow;
    QMenu * rightMenu;
    QMenu * viewRightMenu;            //���ڵ��Ҽ��˵�

    CutInfo cutTmpInfo;               //���������Ϣ

    QPoint pressPoint,movePoint;
    bool isMoving;
    bool viewIsDragable;              //�����Ƿ����ƽ��
    bool isCtrlPressed;               //Crtrl���Ƿ񱻰���
};

#endif // MYGRAPHICSVIEW_H
