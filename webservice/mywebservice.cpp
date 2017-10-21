#include "mywebservice.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QDomDocument>
#include <QStringList>
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

    qDebug()<<currUrl;

    accessManager->post(url,b.toAscii());
}

//��ȡ���ʷ�����Ϣ,200����ȷ�ķ�����
void MyWebService::replyFinshed(QNetworkReply *reply)
{
    QVariant code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QTextCodec * codec = QTextCodec::codecForName("utf8");
    QString replyText = codec->toUnicode(reply->readAll());

    QMap<QString,QString> result;

    if(code.toInt() != 200)
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

//��20161130��Ŀǰ�������������ʽ���ڶ�������������������������ж�ά����ʱ����1�б�ʾ���������ڶ��б�ʾ����ֵ
//��ʱҲ����ֱ������ļ��Ϻͱ���ֵ�ļ��ϲ�һ�£��������ֵ���ϵĳ���С�ڱ��������ϳ��ȣ���������Ӧ�����ڲ�����Ҫ���
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

        /*<ns:return>
          <ns:array>���</ns:array><ns:array>����Ŀ��</ns:array><ns:array>�ɻ�γ��</ns:array>
          <ns:array>�ɻ�����</ns:array><ns:array>�ɻ��߶�</ns:array><ns:array>�ɻ�����</ns:array>
          </ns:return>
        */
        if(keyList.size() > 1)
        {
            for(int i = 0; i < keyList.size(); i++)
            {
                QString key = keyList.at(i).toElement().text();
                QString value = "";
                if(i < valueList.size())
                {
                    value = valueList.at(i).toElement().text();
                }

                results.insert(key,value);
            }
        }
        //�����������ʽ��<ns:array>γ�ȣ����ȣ����Σ��ٶȣ�����</ns:array>
        else if(keyList.size() == 1 && valueList.size() > 0)
        {
           QString varName =  keyList.at(0).toElement().text();
           QStringList varList = varName.split(",");
//           for(int i=0;i<valueList.size();i++)
//           {
//               qDebug()<<"var:"<<varList.at(i);
//           }

//           for(int i=0;i<valueList.size();i++){
//               qDebug()<<"val:"<<valueList.at(i).toElement().text();
//           }
           if(varList.size() == valueList.size())
           {
               for(int i = 0; i < varList.size(); i++)
               {
                   QString value = valueList.at(i).toElement().text();
                   results.insert(varList.at(i),value);
               }
           }
        }
    }

    return results;
}
