/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mysplashscreen.h
**��  ��: wey       Version: 1.0       Date: 2017.03.30
**��  ��: ������������
**Others:
**
**�޸���ʷ:
**20170330:wey:֧�����ö����ǩ��Ϣ
*************************************************/
#ifndef MYSPLASHSCREEN_H
#define MYSPLASHSCREEN_H

#include <QSplashScreen>
#include <QList>
#include <QApplication>

class QLabel;
class MySettings;

struct SplashDesc;

class MySplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit MySplashScreen(const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0 );
    ~MySplashScreen();

    void setSettings(MySettings * settings);
    void setStartText(QString text);

private:
    void setValue();

    MySettings * settings;

    QList<SplashDesc *> splashDescs;
};

#endif // MYSPLASHSCREEN_H
