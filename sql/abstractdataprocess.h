/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: abstractdataprocess.h
**��  ��: wey       Version: 1.0       Date: 2016.07.12
**��  ��:�������ݲ����ӿ�
**Others:1���㼯ͨ�����ݲ���������Ŀǰֻ֧�ֵ��������������Ŀǰ��֧�֣�
**       2������ӵ����ݴ����輯�ɴ˽ӿ�
**
**�޸���ʷ:
**20160712:wey:ʵ����ӡ�ɾ��������ͨ�÷��ʷ�װ
*************************************************/
#ifndef ABSTRACTDATAPROCESS_H
#define ABSTRACTDATAPROCESS_H

#include <QStringList>
#include "../Header.h"

class AbstractDataProcess
{
public:
    AbstractDataProcess();

    virtual bool insertData(const QString tableName, const QStringList insertKey, const QStringList insertValue, int &insertId);
    virtual bool insertData(const QString tableName, const QMap<QString, QString> insertKeyAndValue, int &insertId);

    virtual bool updateData(const QString tableName,const QStringList updateKey, const QStringList updateValue,
                                  const QStringList conditionKey,const QStringList conditionValue);
    virtual bool updateData(const QString tableName,const QMap<QString, QString> updateKeyAndValue,
                                  const QMap<QString, QString> conditionKeyAndValue);

    virtual bool deleteData(const QString tableName, const QStringList deleteKey,const QStringList deleteValue);
    virtual bool deleteData(const QString tableName, const QMap<QString, QString> deleteKeyAndValue);


    virtual bool getUnWrappedData(const QString tableName, const QStringList selectKey,
                                  const QStringList conditionKey, const QStringList conditionValue, DataList &result);

    virtual int getDataCount(const QString tableName);

};

#endif // ABSTRACTDATAPROCESS_H
