/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: XXXXX.h
**��  ��: wey       Version: 1.0       Date: 2016.XX.XX
**��  ��:��ʼ������
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSTATESTARTDIALOG_H
#define MYSTATESTARTDIALOG_H

#include <QDialog>

namespace Ui {
class MyStateStartDialog;
}

class MyStateStartDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateStartDialog(QWidget *parent = 0);
    ~MyStateStartDialog();

    void setContent(QString content);
    QString getContent(){return this->content;}

private slots:
    void updateInfo();
    
private:
    Ui::MyStateStartDialog *ui;

    QString content;
};

#endif // MYSTATESTARTDIALOG_H
