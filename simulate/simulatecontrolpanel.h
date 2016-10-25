/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: simulatecontrolpanel.h
**��  ��: wey       Version: 1.0       Date: 2016.10.11
**��  ��:
**Others:���ݿ������
**
**�޸���ʷ:
**20161017:wey:����ģ����棬����ģ��ǰ�Կؼ�״̬�Ļָ�
**20161020:wey:������֤��ʾ���Լ��������й���
**20161021:wey:���Ӵ����ݶ���ѡ���Ӧ�Ĵ���Ԫ
**20161025:wey:���Ӵӹ����б�ֱ��˫����ʾ����Ԫ���Ա༭��Ϣ
*************************************************/
#ifndef SIMULATECONTROLPANEL_H
#define SIMULATECONTROLPANEL_H

#include <QWidget>
#include <QListWidgetItem>

class QLabel;
struct ProcessUnit;

namespace Ui {
class SimulateControlPanel;
}

class MyListWidgetItem : public QListWidgetItem
{
public:
    MyListWidgetItem(QListWidget * parent = 0, int type = Type);
    MyListWidgetItem(const QString & text, QListWidget * parent = 0, int type = Type);

    void bindProscessUnit(ProcessUnit *unit);
    ProcessUnit * getUnit(){return this->punit;}

private:
    ProcessUnit * punit;
};

class SimulateControlPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit SimulateControlPanel(QWidget *parent = 0);
    ~SimulateControlPanel();

signals:
    void resetSimluate();
    void sendSingleSimulate(ProcessUnit * unit);

private slots:
    void respStartSimulate();
    void showSimulateOperate(ProcessUnit * unit);
    void respItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
    void respItemDoubleClicked(QListWidgetItem * current);
    
private:

    void setFlagState(QLabel * label,bool isSuccess);
    void resetSimluateFlag();

    Ui::SimulateControlPanel *ui;
};

#endif // SIMULATECONTROLPANEL_H
