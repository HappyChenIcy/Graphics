/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myconditionsetting.h
**��  ��: wey       Version: 1.0       Date: 2016.10.28
**��  ��:
**Others:���������
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYCONDITIONSETTING_H
#define MYCONDITIONSETTING_H

#include <QDialog>

namespace Ui {
class MyConditionSetting;
}

class MyConditionSetting : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyConditionSetting(QWidget *parent = 0);
    ~MyConditionSetting();
    
private:
    Ui::MyConditionSetting *ui;
};

#endif // MYCONDITIONSETTING_H
