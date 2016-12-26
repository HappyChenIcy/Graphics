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
**20160928:wey:���Ӹ���ѡ����������
**20161003:wey:���ӿؼ����Ա༭����
**20161017: gx:�޸������ת��󣬸ı��Ҳ���ʽ���ԣ���ѡ�ؼ���������ʽ���⡣
**          gx:�޸��������������½Ƕ�Ϊ������
**20161018:wey:�޸���ݼ�Ctrl+O�򿪱���ѡ���ļ���Ctrl�����ͷ��޷����񣬵��¹��������¼�����
**20161021:wey:�޸�ɾ��item����պͱ��ر��水ť��Ȼ��������
**             �޸�����������ʱ��ճ����ť����Ϊ��������
**             ��ӹ������л��󣬹�����������Ӧ�Ŀؼ����Զ��ĸ���״̬
**20161026:wey:�޸���ת�ؼ�ʱ����ˢ�²���ʱ����������ʾ�ص�����
**20161108:wey:��Ӷ�״̬ͼ֧��
**20161109:wey:��ӶԸ���ģ�����ͼ֧��(˫������һ��)
**20161110:wey:��������ؼ��һ���˫�����¼���Ӧ
**20161130:wey:����֧���ļ����빦��
**20161201:wey:���ӱ��ش��ļ����ж��Ƿ��Ѿ���
**20161209:wey:����ɾ������
**20161215:wey:�޸�����ԭ��ͼʱ�Զ��������ļ���δ��Ӱ汾��׺����
**20161223:wey:����������ʽ�趨��������ʼ��ͷ
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

#include "../Header.h"

class MyNodePort;
class NodeEditDialog;
class MyScene;
class MainWindow;
class MyItem;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    static MyGraphicsView * instance();

    MyGraphicsView(MainWindow * parent = 0);
    ~MyGraphicsView();

    MyItem * getTopSelectedItem();

    void showNodePortEdit(MyNodePort * nodePort);
    void openLocalFile(QString fileName,bool openFile = false);

    QString getFirstSelectedItem();

    void clearItems();
    void addContextMenuItem();
    void addViewContextMenu();

    MyScene * scene();
    MyScene * addScene(QString id);
    void showScene(MyScene *scene);
    void deleteScene();

    qreal getHorizonalValue();
    qreal getVertiaclValue();
    qreal getScaleValue();
    void  transformView(qreal hValue,qreal value);
    void  setScaleValue(int);
    void  setKeyCtrlStated(bool isPressed){this->isCtrlPressed = isPressed;}

    int  getItemSize();
    void resetItemProcessType();

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
    void scaleValue(int);

public slots:
    void sceneScaled(int currScale);
    void respResetSimluate();
    void respItemSizeChanged(int size);
    void showSelectedItemPropEdit(MyItem * item,QVariant userRoleData = "",bool isPanelEditable = true);

private slots:
    void addPage(QString proPath,QString proName);
    void fileSave();
    void fileSaveAs();

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
    void respCtrlLockKeyPress();

    void updateActions();
    void editTextItem();
    void respEditText();
    void editPropertyItem();
    void respPropertyUpdate(ItemProperty property);
    void setSelectedLineType(int type);
    void setToolBarState();

private:
    void initView();
    void getSelectedLockState();
    void setSelectedItemLockState(bool flag);

    static MyGraphicsView * viewManager;

    NodeEditDialog * nodeEdit;
    MyScene * myScene;
    MainWindow * parentWindow;
    CutInfo cutTmpInfo;               //���������Ϣ

    QPoint pressPoint,movePoint;
    bool isMoving;
    bool viewIsDragable;              //�����Ƿ����ƽ��
    bool isCtrlPressed;               //Crtrl���Ƿ񱻰���
    int sceneScaleFactor;             //��ͼ��������

    ItemLockState lockState;          //ѡ��ؼ�����״̬
};

#endif // MYGRAPHICSVIEW_H
