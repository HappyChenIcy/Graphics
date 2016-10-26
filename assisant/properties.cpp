#include "properties.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "util.h"

Properties::Properties(QString fileName):
    fileName(fileName)
{
    parseFile();
}

void Properties::parseFile()
{
    QFile file(fileName);
    if(!file.exists())
    {
        Util::showWarn(fileName+" �ļ�������,������ȷ��!");
        return;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        Util::showWarn(fileName+" �ļ����ܶ�!");
        return;
    }
    QTextStream stream(&file);
    QString temp;
    int index;
    while(!stream.atEnd())
    {
       temp = stream.readLine();
       if((index=temp.indexOf(QRegExp("/")))>0)   //�������ע�ͣ������ע������
       {
            temp = temp.left(index);
       }
       QStringList list = temp.split("=");
       temp = list.at(1);         //��δ���пո���
       maps.insertMulti(list.at(0),temp);
    }
}

QString Properties::getValue(QString key)
{
    if(maps.contains(key))
    {
         return maps.value(key);
    }
    else
    {
         return "";
    }
}
