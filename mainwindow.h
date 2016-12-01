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
**20161003:wey:��ӽڵ����Ա༭
**20161011:wey:��ӹ����������л�����ģ/��������
**20161017:wey:��ӿ�ݼ�Ctrl+M/Ctrl+Shift+M�����л���������
**20161018:wey:�޸��л�����ģʽ����ʾ/�����Ҳ���������Ч����
**20161020:wey:��ӹر���ʾ����
**             ��ӳ���ͼ��
**20161026:wey:�޸��������л������¼������Ӧ����
**20161115:wey:������Ϊ����
**20161118:wey:�޸�����ģʽ���л�����ģ״̬����ģ��ť�����л������⡣
**20161201:wey:�����л��������󣬴�����ʾ��������Ϣ
**             ���ӿ�ݼ��б���ʾ
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
class SimulateControlPanel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPress(QKeyEvent * event);
    void respShowStatusInfo(QString);
    ~MainWindow();

signals:
    void updateProperty(ItemProperty property);

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void fileOpen();
    void fileClear();
    void recordClickedItem();
    void respRestItemAction();
    void switchFullScreen();
    void hideSubWidget();
    void getTecSupport();
    void showAbout();
    void respDeletePage();
    void switchWorkModel();
    void updateWindowTitle();
    void showAssisantList();

private:
    void createActionAndMenus();
    void createSceneAndView();
    void createToolBar();
    void createStatusBar();
    void createLineComboBox();
    void resetEditActionState(bool state);
    void setIconWidgetState();
    void setToolWidgetState(bool flag = true);
    void setWorkspaceState();
    void setSimulateWidgetState(bool flag = true);

    Ui::MainWindow *ui;
    QActionGroup * itemGroup;
    QActionGroup * workModelGroup;

    MySlider * mySlider;
    MyGraphicsView * view;
    LeftIconWidget * leftIconWidget;
    RightToolBox * rightToolBox;
    SimulateControlPanel * simulatePanel;

    QToolBar * fileBar;
    QToolBar * itemBar;
    QToolBar * editBar;
    QToolBar * sceneBar;
    QToolBar * databaseBar;

    CutInfo cutTmpInfo;               //���������Ϣ

    QStringList windowTitles;         //���ڱ���
    QString currWorkspaceFullPath;    //��ǰ������ȫ·��
};

#endif // MAINWINDOW_H
