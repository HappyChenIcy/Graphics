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
**20160909:wey:��ӿؼ��ı��桢��(�������ļ�)�������ļ�ͷ�ж�
**             �޸�ѡ��ĳ��item���༭�����ֺ󣬵���Ҳ๤������������ʧ���⡣
**             ���Ӷ����(Z����ֵ)�ı���ͽ���
**20160913:wey:�޸�������ת90�ȵ�����ת��������
**20160919:wey:���С��������ӶԶ˿ڹ���
**20160920:wey:���������������ؼ��������ͳһ�ĳ����봴��
**             �޸�ɾ��nodeport������������
**20160922:wey:��ӿؼ����˿ڵ�����/��������
**             ��MainWindow��MyGraphicsView�������ع���ʹ�����߹�����Զ�������������
**20160925:wey:�������ҹ���������ʾ����
**             ���ӿؼ��Ͱ����˵���
*************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Header.h"

class MyGraphicsView;
class QActionGroup;
class MySlider;
class RightToolBox;
class LeftIconWidget;
class HideSplit;

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
    void updateProperty(ItemProperty property);

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileClear();
    void exitApp();
    void recordClickedItem();
    void respRestItemAction();
    void respItemSizeChanged(int size);
    void switchFullScreen();
    void hideSubWidget();
    void getTecSupport();
    void showAbout();

private:
    void createActionAndMenus();
    void createSceneAndView();
    void createToolBar();
    void createStatusBar();
    void createLineComboBox();
    void respShowStatusInfo(QString);
    void resetEditActionState(bool state);

    Ui::MainWindow *ui;
    QActionGroup * itemGroup;

    MySlider * mySlider;
    MyGraphicsView * view;
    LeftIconWidget * leftIconWidget;
    RightToolBox * rightToolBox;

    CutInfo cutTmpInfo;               //���������Ϣ
};

#endif // MAINWINDOW_H
