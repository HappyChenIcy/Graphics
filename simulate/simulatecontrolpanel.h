/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: simulatecontrolpanel.h
**��  ��: wey       Version: 1.0       Date: 2016.10.11
**��  ��:
**Others:���ݿ������
**
**�޸���ʷ:
**
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

private slots:
    void respStartSimulate();
    
private:
    Ui::SimulateControlPanel *ui;
};

#endif // SIMULATECONTROLPANEL_H
