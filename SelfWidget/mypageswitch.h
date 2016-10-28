/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypageswitch.h
**��  ��: wey       Version: 1.0       Date: 2016.10.03
**��  ��:
**Others: ҳ���л�
**
**�޸���ʷ:
**20161004:wey:�޸�ɾ����������������
**20161017: gx:�����½���������Ĭ�ϲ��뵽���
**20161015:wey:����Alt+���Ҽ������л���������
**20161025:wey:���ӹ��������Ҽ��˵���֧�ֹرա����桢�ر�������С��ر��Ҳ�����
**20161026:wey:�޸������������Ϊ0ʱ����Ȼ������ӦCtrl+w���¼�
*************************************************/
#ifndef MYPAGESWITCH_H
#define MYPAGESWITCH_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;
class MyScene;
class MyPageItem;
class QAction;

class MyPageSwitch : public QWidget
{
    Q_OBJECT
public:
    static MyPageSwitch * instance();

    void switchFrontBack(bool isFront = true);
    void addSwitchContextMenu();

    struct PageMapping
    {
        QString id;
        MyPageItem * pageItem;
        MyScene * scene;
        QString pageName;
        qreal hScrollValue;
        qreal vScrollValue;
        qreal scaleView;
    };

signals:
    void deletePage();
    void switchPage();

public slots:
    void addPage();
    void closePage();

private slots:
    void switchToPage(QString pageId);
    void deleteThisPage(QString pageId);
    void closeLeftPage();
    void closeRightPage();

private:
    MyPageSwitch(QWidget * parent = 0);
    static MyPageSwitch * pageSwitch;

    void siwtchPage(QString pageId,bool firstView = false);
    int getPageIndex(QString id);

    void initWidget();

    QPushButton * addPageButt;
    QHBoxLayout * layout;

    QList<PageMapping *> pages;
    int pagePosition;                     //ҳ��λ��
    PageMapping * selectedPage;
    bool isFirstView;
};

#endif // MYPAGESWITCH_H
