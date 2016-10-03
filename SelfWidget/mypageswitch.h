/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mypageswitch.h
**��  ��: wey       Version: 1.0       Date: 2016.10.03
**��  ��:
**Others: ҳ���л�
**
**�޸���ʷ:
**
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

    struct PageMapping
    {
        QString id;
        MyPageItem * pageItem;
        MyScene * scene;
    };

public slots:
    void addPage();

private slots:
    void switchToPage(QString pageId);
    void deleteThisPage(QString pageId);

private:
    MyPageSwitch(QWidget * parent = 0);
    static MyPageSwitch * pageSwitch;

    void initWidget();

    QPushButton * addPageButt;
    QHBoxLayout * layout;

    QList<PageMapping *> pages;
};

#endif // MYPAGESWITCH_H
