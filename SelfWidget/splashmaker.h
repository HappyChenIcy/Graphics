/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: SplashMaker.h
**��  ��: wey       Version: 1.0       Date: 2017.03.31
**��  ��:
**Others:��������ҳ����Ϣ
**
**�޸���ʷ:
**
*************************************************/
#ifndef SPLASHMAKER_H
#define SPLASHMAKER_H

#include <QDialog>
#include <QList>
#include <QModelIndex>

class  MySettings;
struct SplashDesc;

namespace Ui {
class SplashMaker;
}

class SplashMaker : public QDialog
{
    Q_OBJECT
    
public:
    static SplashMaker * instance();

private slots:
    void execMe();
    void addContent();
    void removeContent();
    void modifyCurrText(QModelIndex index);

private:
    Ui::SplashMaker *ui;

    void readSettings();
    void writeSettings();

    SplashMaker(QWidget *parent = 0);
    ~SplashMaker();

    static SplashMaker * splashMaker;

    MySettings * settings;

    QList<SplashDesc *> splashlist;
};

#endif // SPLASHMAKER_H
