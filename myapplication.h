/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: myapplication.h
**��  ��: wey       Version: 1.0       Date: 2016.10.25
**��  ��:
**Others: �Զ�������ʵ��
**
**�޸���ʷ:
**20161025:wey:����ȫ�ּ��̲������Alt+���Ҽ��л�
*************************************************/
#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>

class MainWindow;

class MyApplication : public QApplication
{
    Q_OBJECT
public:
    MyApplication(int & argc, char ** argv );

    bool notify(QObject *obj, QEvent *event);

};

#endif // MYAPPLICATION_H
