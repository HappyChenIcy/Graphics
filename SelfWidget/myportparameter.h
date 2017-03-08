/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myportparameter.h
**��  ��: wey       Version: 1.0       Date: 2017.03.08
**��  ��: ����˿ڲ�������
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYPORTPARAMETER_H
#define MYPORTPARAMETER_H

#include <QDialog>

class QAbstractButton;

namespace Ui {
class MyPortParameter;
}

class MyPortParameter : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortParameter(QWidget *parent = 0);
    bool getResult(QString &pname,QString &ptype);

    ~MyPortParameter();

private slots:
    void getButton(QAbstractButton* butt);
    
private:
    Ui::MyPortParameter *ui;

    QString pname,ptype;
    bool isOkPressed;
};

#endif // MYPORTPARAMETER_H
