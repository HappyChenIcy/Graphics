/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mainwindow.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ������
**Others:1���ṩ��������֧�ֲ�ͬ���Ϳؼ������
**
**
**�޸���ʷ:
**20160908:wey:�޸�ɾ��ֻѡ������ʱ��������
**             ����ļ��������������½���ɾ��
**             ���������Ϣɾ��
**             ��Ӽ��С����ơ�ճ��
*************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Header.h"

class QGraphicsView;
class MyScene;
class QActionGroup;
class MySlider;
class RightToolBox;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void initToolBox(int selectedNum,ItemProperty property);
    void updateProperty(ItemProperty property);

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileClear();
    void exitApp();

    void cutItem();
    void copyItem();
    void pasteItem();

    void rotateItem();
    void bringZItem();
    void deleteItem();

    void editTextItem();

    void addItem();

    void respRestItemAction();
    void updateActions();
    void sceneScaled(int currScale);
    void respPropertyUpdate(ItemProperty property);
    void respItemSizeChanged(int size);
    
private:
    void createActionAndMenus();
    void createSceneAndView();
    void createContextMenu();
    void createToolBar();

    Ui::MainWindow *ui;

    MyScene * scene;
    QGraphicsView * view;

    QMenu * rightMenu;

    QActionGroup * itemGroup;

    MySlider * mySlider;
    RightToolBox * rightToolBox;

    CutInfo cutTmpInfo;               //���������Ϣ
};

#endif // MAINWINDOW_H
