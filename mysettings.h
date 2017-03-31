/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: MySettings.h
**��  ��: wey       Version: 1.0       Date: 2017.03.31
**��  ��:
**Others: ͨ������
**
**�޸���ʷ:
**
*************************************************/
#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>
#include <QColor>
#include <QApplication>
#include <QFont>

struct SplashDesc
{
    SplashDesc()
    {
        scaleX = 0.1;
        scaleY = 0.1;
        scaleWidth = 0.2;
        scaleHeight = 0.05;
        fontSize = 20;
        fontColor = QColor(255,255,255);
        fontFamily = "����";
        isBold = false;
        splashContent = qApp->applicationName();
    }
    QString splashContent;
    QFont splashFont;
    double scaleX,scaleY,scaleWidth,scaleHeight;
    int fontSize;
    QString fontFamily;
    bool isBold;
    QColor fontColor;
};

class MySettings : public QSettings
{
public:
    MySettings(const QString & fileName, Format format, QObject * parent = 0);

    void readSplashSettings(QList<SplashDesc *> &splashDescs);
    void writeSplashSettings(QList<SplashDesc *> &splashDescs);
};

#endif // MYSETTINGS_H
