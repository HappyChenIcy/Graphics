/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mystateinternaleventdialog.h
**��  ��: wey       Version: 1.0       Date: 2016.11.08
**��  ��: ״̬�ڲ��¼�����
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSTATEINTERNALEVENTDIALOG_H
#define MYSTATEINTERNALEVENTDIALOG_H

#include <QDialog>

namespace Ui {
class MyStateInternalEventDialog;
}

class MyStateInternalEventDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateInternalEventDialog(QWidget *parent = 0);
    ~MyStateInternalEventDialog();
    
private:
    Ui::MyStateInternalEventDialog *ui;
};

#endif // MYSTATEINTERNALEVENTDIALOG_H
