/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: initdatamanager.h
**��  ��: wey       Version: 1.0       Date: 2016.07.09
**��  ��:����.properties���͵������ļ�
**Others:
**      1.���Է��ؽ������ļ�-ֵ��
*       2.����ͨ�����ķ��ʻ��Ӧ��ֵ
**�޸���ʷ:
**
*************************************************/
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QMap>
#include <QString>

class Properties
{
public:
    Properties(QString fileName);
    QString getValue(QString key);

private:
    void parseFile();                         //�ӱ��ؽ����ļ������뵽map��

    QString fileName;
    QMap<QString , QString> maps;
};

#endif // PROPERTIES_H
