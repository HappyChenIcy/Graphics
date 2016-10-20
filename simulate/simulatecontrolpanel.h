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
*************************************************/
#ifndef SIMULATECONTROLPANEL_H
#define SIMULATECONTROLPANEL_H

#include <QWidget>

class QLabel;
struct ProcessUnit;

namespace Ui {
class SimulateControlPanel;
}

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
    
private:

    void setFlagState(QLabel * label,bool isSuccess);
    void resetSimluateFlag();

    Ui::SimulateControlPanel *ui;
};

#endif // SIMULATECONTROLPANEL_H
