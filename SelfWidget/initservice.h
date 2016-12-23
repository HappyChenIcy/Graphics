/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: initservice.h
**��  ��: wey       Version: 1.0       Date: 2016.12.19
**��  ��: ��ʼ�������ִ�л���
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef INITSERVICE_H
#define INITSERVICE_H

#include <QDialog>

namespace Ui {
class InitService;
}

class QListWidgetItem;
class MyChooseBar;
class MyItem;
class ServiceInputTableView;

struct ProcessUnit;
struct ServiceProperty;

class InitService : public QDialog
{
    Q_OBJECT
    
public:
    explicit InitService(QWidget *parent = 0);
    ~InitService();

    void preService();
    void resetService();

private slots:
    void respSwitchService(QListWidgetItem *item);
    void switchServiceInfo(int currIndex);
    void addCurrService();
    void removeCurrService();
    void applyUpdateService();
    
private:
    void initServiceData();
    void updateDelegateList(QList<MyItem *> pItems);
    bool findHasExistedService(QString sname);
    void showSelectedItemInfo(ServiceProperty *prop);
    void setButtState(bool isAddEnabled);

    Ui::InitService *ui;

    MyChooseBar * chooseBar;
    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    bool isComboxAutoChanged;               //����������Զ��ı䣬��������Ӧ�ۺ���
    bool isPreModel;                        //�Ƿ�ΪԤ����ģʽ
    int currServiceIndex;                   //listwidget����ʾ������
};

#endif // INITSERVICE_H
