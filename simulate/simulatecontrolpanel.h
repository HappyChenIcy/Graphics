/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: simulatecontrolpanel.h
**��  ��: wey       Version: 1.0       Date: 2016.10.11
**��  ��:
**Others:���ݿ������
**
**�޸���ʷ:
**20161017:wey:����ģ����棬����ģ��ǰ�Կؼ�״̬�Ļָ�
*************************************************/
#ifndef SIMULATECONTROLPANEL_H
#define SIMULATECONTROLPANEL_H

#include <QWidget>

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

private slots:
    void respStartSimulate();
    
private:
    Ui::SimulateControlPanel *ui;
};

#endif // SIMULATECONTROLPANEL_H
