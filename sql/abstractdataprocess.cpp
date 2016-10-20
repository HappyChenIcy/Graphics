#include "abstractdataprocess.h"

#include "sqlproecss.h"

#include <QDebug>

AbstractDataProcess::AbstractDataProcess()
{
}

///*****************************************************
///**Function:insertData
///**Description:ͨ�����ݿ����
///**Input:const QString tableName�����������ݱ���
///        const QStringList key���������ֶμ���
///        const QStringList value���������Ӧ�ֶ�ֵ����
///**Output:int & insertId:���ز���ɹ����ݿ�ID��
///**Return:bool:�������ݳɹ����
///**Others:
///****************************************************/
bool AbstractDataProcess::insertData(const QString tableName, const QStringList insertKey,
                                           const QStringList insertValue, int & insertId)
{
    if(insertKey.size() != insertValue.size())
    {
        return false;
    }
    QString sql = "insert into "+ tableName + " ( ";

    for(int i = 0; i < insertKey.size(); i++)
    {
        sql += insertKey.at(i);
        if(i!=insertKey.size()-1)
        {
           sql += ",";
        }
    }

    sql += " ) values (";

    for(int i = 0; i < insertValue.size(); i++)
    {
        sql += insertValue.at(i);

        if(i!=insertValue.size()-1)
        {
           sql += (",");
        }
    }

    sql += " )";

    return SQLProecss::instance()->insert(sql,insertId);
}

///*****************************************************
///**Function:insertData
///**Description:ͨ�����ݿ����
///**Input:const QString tableName�����������ݱ���
///        const QMap<QString, QVariant> insertKeyAndValue���������ֶ���ֵ����
///**Output:int & insertId:���ز���ɹ����ݿ�ID��
///**Return:bool:�������ݳɹ����
///**Others:
///****************************************************/
bool AbstractDataProcess::insertData(const QString tableName, const QMap<QString, QString> insertKeyAndValue,
                                           int &insertId)
{
    return insertData(tableName,QStringList(insertKeyAndValue.keys()),
                      QStringList(insertKeyAndValue.values()),insertId);
}

///*****************************************************
///**Function:deleteData
///**Description:ͨ�����ݼ�¼ɾ��
///**Input:const QString tableName����ɾ�������ݱ���
///        const QStringList deleteKey����ɾ�����ݼ�¼�ֶμ���
///        const QStringList deleteValue����ɾ�����ݼ�¼�ֶ�ֵ����
///**Output:��
///**Return:bool������ɾ���ĳɹ����
///**Others:
///****************************************************/
bool AbstractDataProcess::deleteData(const QString tableName,  const QStringList deleteKey,
                                           const QStringList deleteValue)
{
    if(deleteKey.size() != deleteValue.size())
    {
        return false;
    }
    QString sql = "delete from " + tableName + " where ";

    for(int i = 0; i < deleteKey.size();  i++)
    {
        sql += (deleteKey.at(i) + " = " + deleteValue.value(i));

        if(i != deleteKey.size()-1)
        {
            sql += " and ";
        }
    }

    return SQLProecss::instance()->execute(sql);
}

bool AbstractDataProcess::deleteData(const QString tableName, const QMap<QString, QString> deleteKeyAndValue)
{
    return deleteData(tableName,QStringList(deleteKeyAndValue.keys()),QStringList(deleteKeyAndValue.values()));
}

///*****************************************************
///**Function:updateData
///**Description:ͨ�����ݿ����
///**Input:const QString tableName�����������ݱ���
///        const QStringList key���������ֶμ���
///        const QStringList value�������¶�Ӧ�ֶ�ֵ����
///        const QStringList updateKey�����µ������ֶμ���
///        const QStringList updateValue:����������Ӧ���ֶ�ֵ����
///**Output:��
///**Return:bool:�������ݳɹ����
///**Others:
///****************************************************/
bool AbstractDataProcess::updateData(const QString tableName, const QStringList updateKey,
                                           const QStringList updateValue,  const QStringList conditionKey,
                                           const QStringList conditionValue)
{
    if(updateKey.size() != updateValue.size() || updateKey.size() != updateValue.size() )
    {
        return false;
    }

    QString sql = "update "+tableName +" set ";

    for(int i = 0; i<updateKey.size(); i++)
    {
        sql += (updateKey.at(i) + " = "+ updateValue.at(i));

        if(i != updateKey.size()-1)
        {
            sql += ",";
        }
    }

    sql += " where ";

    for(int i = 0; i<conditionKey.size(); i++)
    {
        sql += (conditionKey.at(i) + " = "+ conditionValue.at(i));

        if(i != conditionKey.size()-1)
        {
            sql += " and ";
        }
    }

    return SQLProecss::instance()->execute(sql);
}

bool AbstractDataProcess::updateData(const QString tableName, const QMap<QString, QString> updateKeyAndValue,
                                           const QMap<QString, QString> conditionKeyAndValue)
{
    return updateData(tableName,QStringList(updateKeyAndValue.keys()),QStringList(updateKeyAndValue.values())
                      ,QStringList(conditionKeyAndValue.keys()),QStringList(conditionKeyAndValue.values()));
}

///*****************************************************
///**Function:getDataCount
///**Description:��ȡ��Ӧ���ݱ��е���������
///**Input:const QString tableName�����ݿ����
///**Output:��
///**Return:const int�����ض�Ӧ���ݱ������
///**Others:
///****************************************************/
int AbstractDataProcess::getDataCount(const QString tableName)
{
    QString condition = "select count(*) from "+tableName;

    return SQLProecss::instance()->getSqlDataCount(condition);
}

///*****************************************************
///**Function:getUnWrappedData
///**Description:���ݲ�ѯ�����Բ�ѯ������н��������ɵ�����ֱ�ӽ���
///**Input:const QString tableName������ѯ����
///        const QStringList selectKey������ѯ���ֶ�
///        const QStringList conditionKey:����ѯ����
///        const QStringList conditionValue������ѯ������Ӧֵ
///**Output:DataList & result����ѯ���
///**Return:const bool��ִ�гɹ�����ʧ��
///**Others:
///****************************************************/
bool AbstractDataProcess::getUnWrappedData(const QString tableName, const QStringList selectKey,
                                                 const QStringList conditionKey, const QStringList conditionValue,
                                                 DataList & result)
{
    if(conditionKey.size() != conditionValue.size())
    {
        return false;
    }

    QString sql = " select ";
    if(selectKey.size()==0)
    {
        sql += " *";
    }
    else
    {
        for(int i = 0; i < selectKey.size(); i++)
        {
            sql += selectKey.at(i);
            if(i != selectKey.size()-1)
            {
                sql += " , ";
            }
        }
    }

    sql +=(" from " + tableName);

    if(conditionKey.size() > 0)
    {
        sql += " where ";

        for(int i = 0; i < conditionKey.size(); i++)
        {
            sql += ( conditionKey.at(i) + " = " +conditionValue.at(i));
            if(i != conditionKey.size()-1)
            {
                sql += " , ";
            }
        }
    }

    return SQLProecss::instance()->obtainData(sql,result);
}


