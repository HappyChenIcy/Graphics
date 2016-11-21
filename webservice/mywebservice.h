/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: mywebservice.h
**��  ��: wey       Version: 1.0       Date: 2016.11.10
**��  ��: ���������
**Others:
**
**�޸���ʷ:
**20161118:wey:�޸����÷����ڴ�һֱ�������⡾reply->deleteLater()��
*************************************************/
#ifndef MYWEBSERVICE_H
#define MYWEBSERVICE_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class MyWebService : public QObject
{
    Q_OBJECT
public:
    static MyWebService * instance();

    void submit(QString currUrl);

signals:
    void lastUnitProcessOver(bool,QString);

private slots:
    void replyFinshed(QNetworkReply* reply);

private:
    MyWebService();
    static MyWebService * service;

    QString parseResult(QString result, bool &hasFault);

    QNetworkAccessManager * accessManager;          //�������
};

#endif // MYWEBSERVICE_H
