/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: beforesimulateserviceconfig.h
**��  ��: wey       Version: 1.0       Date: 2016.12.16
**��  ��: ��������ǰͳһ��������ģ��
**Others:�����̼�����󣬲���������֮ǰ�������˿��������ڶ�
**       ����������Ϣ��ȷ�ϣ�����༭���޸ġ�
**
**�޸���ʷ:
**
*************************************************/
#ifndef BEFORESIMULATESERVICECONFIG_H
#define BEFORESIMULATESERVICECONFIG_H

#include <QDialog>

namespace Ui {
class BeforeSimulateServiceConfig;
}
class QListWidgetItem;
class MyChooseBar;
class MyItem;
class ServiceInputTableView;

struct ProcessUnit;
struct ServiceProperty;

class BeforeSimulateServiceConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit BeforeSimulateServiceConfig(QWidget *parent = 0);
    ~BeforeSimulateServiceConfig();

    void setProcedureData(QList<ProcessUnit *> & procUnits);

private slots:
    void respSwitchService(int currIndex);
    void switchServiceInfo(int currIndex);
    void applyService();
    
private:
    void initServiceData();
    void findHasExistedService(ServiceProperty * prop);
    void updateDelegateList(QList<MyItem *> pItems);
    void updateLeftQuoteTable(QList<MyItem *> & pItems);

    Ui::BeforeSimulateServiceConfig *ui;

    MyChooseBar * chooseBar;
    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    QList<ProcessUnit *>  procUnits;        //ֻ���洦�����
    bool isComboxAutoChanged;               //����������Զ��ı䣬��������Ӧ�ۺ���
    int currServiceIndex;                   //��ǰ��������
};

#endif // BEFORESIMULATESERVICECONFIG_H
