/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myshortkey.h
**��  ��: wey       Version: 1.0       Date: 2016.12.01
**��  ��: ��ʾ��ݼ�
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSHORTKEY_H
#define MYSHORTKEY_H

#include <QDialog>

namespace Ui {
class MyShortKey;
}

class MyShortKey : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyShortKey(QWidget *parent = 0);
    ~MyShortKey();
    
private:
    void init();

    Ui::MyShortKey *ui;
};

#endif // MYSHORTKEY_H
