/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myportoutputdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��: ����˿�����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYPORTOUTPUTDIALOG_H
#define MYPORTOUTPUTDIALOG_H

#include <QDialog>

#include "../Header.h"

namespace Ui {
class MyPortOutputDialog;
}

class MyPortOutputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortOutputDialog(QWidget *parent = 0);
    ~MyPortOutputDialog();

    void setInOutState(bool isInstate = true);
    void setProp(StateInOutProperty & prop);
    StateInOutProperty & getProp();

private slots:
    void updateProp();
    
private:
    Ui::MyPortOutputDialog *ui;

    StateInOutProperty sprop;
};

#endif // MYPORTOUTPUTDIALOG_H
