/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mystatemodeldialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��:ģ�Ϳ�״̬����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSTATEMODELDIALOG_H
#define MYSTATEMODELDIALOG_H

#include <QDialog>

namespace Ui {
class MyStateModelDialog;
}

class MyStateModelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateModelDialog(QWidget *parent = 0);
    ~MyStateModelDialog();
    
private slots:
    void respContinueAction();
    void respAddItem();

private:
    Ui::MyStateModelDialog *ui;
};

#endif // MYSTATEMODELDIALOG_H
