/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypageswitch.h
**��  ��: wey       Version: 1.0       Date: 2016.10.03
**��  ��:
**Others: ҳ���л�
**
**�޸���ʷ:
**20161004:wey:�޸�ɾ����������������
*************************************************/
#ifndef MYPAGESWITCH_H
#define MYPAGESWITCH_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;
class MyScene;
class MyPageItem;

class MyPageSwitch : public QWidget
{
    Q_OBJECT
public:
    static MyPageSwitch * instance();

    void closePage();

    struct PageMapping
    {
        QString id;
        MyPageItem * pageItem;
        MyScene * scene;
        qreal hScrollValue;
        qreal vScrollValue;
        qreal scaleView;
    };

signals:
    void deletePage();

public slots:
    void addPage();

private slots:
    void switchToPage(QString pageId);
    void deleteThisPage(QString pageId);

private:
    MyPageSwitch(QWidget * parent = 0);
    static MyPageSwitch * pageSwitch;

    void siwtchPage(QString pageId,bool firstView = false);

    void initWidget();

    QPushButton * addPageButt;
    QHBoxLayout * layout;

    QList<PageMapping *> pages;

    PageMapping * selectedPage;
    bool isFirstView;
};

#endif // MYPAGESWITCH_H
