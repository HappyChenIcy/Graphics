/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: aboutsoft.h
**��  ��: wey       Version: 1.0       Date: 2016.11.10
**��  ��: ���������Ϣ
**Others:
**
**�޸���ʷ:
**
*************************************************/
#ifndef ABOUTSOFT_H
#define ABOUTSOFT_H

#include <QDialog>

class AboutSoft : public QDialog
{
public:
    explicit AboutSoft(QWidget * parent = 0);

private:
    void initDialog();
};

#endif // ABOUTSOFT_H
