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
**20161201:wey:���Ӹ����ļ���ȫ·���л�
*************************************************/
#ifndef MYPAGESWITCH_H
#define MYPAGESWITCH_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;
class MyScene;
class MyPageItem;
class QAction;

//�����ļ���ҳ�桢����֮��Ĺ�ϵ����ʵ������������ǰҳ��Ļ�����Ϣ
struct PageMapping
{
    PageMapping()
    {
        isAssociated = false;
    }
    QString id;
    MyPageItem * pageItem;
    MyScene * scene;         //������scene
    QString pageName;        //��������
    QString pathName;        //·����(�ļ�����):c:/abc
    QString fullPathName;    //(�������)��ǰ�����ļ���ʾͼԪ��ȫ·������:c:/abc/123.bin
    bool isAssociated;       //�Ƿ񱻹��������false������ʱ��Ҫ������ʾ�򣬷������id��·���Զ����档
    qreal hScrollValue;      //��ǰҳ��ˮƽ������ֵ
    qreal vScrollValue;      //��ǰҳ����ֱ������ֵ
    qreal scaleView;         //��ǰҳ�����ű���ֵ
};

class MyPageSwitch : public QWidget
{
    Q_OBJECT
public:
    static MyPageSwitch * instance();

    void switchFrontBack(bool isFront = true);
    void addSwitchContextMenu();

    bool openPage(QString pageId);
    void updateCurrMappingInfo(QString & fileName);

    void switchToPageByFileName(QString fileName);

    void emitSwitchPage();

    const PageMapping * currPageMapping();

    QStringList getOpenedFullFileNames();
    bool hasContainFile(QString & fileName);

    int  count()const;

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
    void addPage(QString id, bool isAssociated = false);
    void initWidget();

    QPushButton * addPageButt;
    QHBoxLayout * layout;

    QList<PageMapping *> pages;
    int pagePosition;                     //ҳ��λ��
    PageMapping * selectedPage;
    bool isFirstView;
};

#endif // MYPAGESWITCH_H
