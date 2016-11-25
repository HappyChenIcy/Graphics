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

    QMap<QString,QString> result;

    if(code != 200)
    {
        result.insert("result","ִ�з���ʧ��!");
        emit lastUnitProcessOver(true,result);
    }
    else
    {
        bool hasFault = true;
        result = parseResult(replyText,hasFault);
        emit lastUnitProcessOver(hasFault,result);
    }
    reply->deleteLater();
}

QMap<QString,QString> MyWebService::parseResult(QString result,bool & hasFault)
{
    QDomDocument doc;
    doc.setContent(result);

    hasFault = true;

    QMap<QString,QString> results;

    //�Ƿ���ڴ���
    QDomNodeList nodeList = doc.elementsByTagName("faultstring");

    if(nodeList.size() > 0)
    {
        QDomNode node = nodeList.at(0);
        results.insert("result",node.toElement().text());
        return results;
    }

    nodeList = doc.elementsByTagName("ns:return");

    //�����������Ϊ1������ת��Ϊ��ά����
    if(nodeList.size() == 1)
    {
        hasFault = false;
        QDomNode node = nodeList.at(0);
        results.insert("result",node.toElement().text());
        return results;
    }
    //�������Ϊ2ά����,��2ά������н���
    else if(nodeList.size() == 2)
    {
        hasFault = false;

        QDomNodeList keyList = nodeList.at(0).toElement().elementsByTagName("ns:array");
        QDomNodeList valueList = nodeList.at(1).toElement().elementsByTagName("ns:array");

        if(keyList.size() == valueList.size())
        {
            for(int i = 0; i < keyList.size(); i++)
            {
                QString key = keyList.at(i).toElement().text();
                QString value = valueList.at(i).toElement().text();
                results.insert(key,value);
            }
        }
    }

    return results;
}
