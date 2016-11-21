#include "mywebservice.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QDomDocument>
#include <QDebug>

MyWebService * MyWebService::service = NULL;

MyWebService * MyWebService::instance()
{
    if(!service)
    {
        service = new MyWebService();
    }
    return service;
}

MyWebService::MyWebService()
{
    accessManager = new QNetworkAccessManager;
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinshed(QNetworkReply*)));
}

//�ύ����
void MyWebService::submit(QString currUrl)
{
    QNetworkRequest url;
    url.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    url.setUrl(QUrl(currUrl));
    QString b("");
    accessManager->post(url,b.toAscii());
}

//��ȡ���ʷ�����Ϣ,200����ȷ�ķ�����
void MyWebService::replyFinshed(QNetworkReply *reply)
{
    QVariant code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QTextCodec * codec = QTextCodec::codecForName("utf8");
    QString replyText = codec->toUnicode(reply->readAll());

    if(code != 200)
    {
        emit lastUnitProcessOver(true,"ִ�з���ʧ��!");
    }
    else
    {
        bool hasFault = true;
        QString result = parseResult(replyText,hasFault);
        emit lastUnitProcessOver(hasFault,result);
    }
    reply->deleteLater();
}

QString MyWebService::parseResult(QString result,bool & hasFault)
{
    QDomDocument doc;
    doc.setContent(result);

    hasFault = true;

    //�Ƿ���ڴ���
    QDomNodeList nodeList = doc.elementsByTagName("faultstring");

    if(nodeList.size() > 0)
    {
        QDomNode node = nodeList.at(0);
        return node.toElement().text();
    }

    nodeList = doc.elementsByTagName("ns:return");

    if(nodeList.size() > 0 )
    {
        hasFault = false;
        QDomNode node = nodeList.at(0);
        return node.toElement().text();
    }

    return "";
}
