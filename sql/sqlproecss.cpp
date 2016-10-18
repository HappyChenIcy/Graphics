#include "sqlproecss.h"

#include <QSqlQuery>
#include <QSqlRecord>

#include "../assisant/properties.h"
#include "../global.h"

#include <QDebug>

QSqlDatabase sqlDatabase;

SQLProecss* SQLProecss::process = NULL;

SQLProecss * SQLProecss::instance()
{
    if(!process)
    {
        process = new SQLProecss;
    }
    return process;
}

SQLProecss::SQLProecss()
{
    prop = NULL;
}

///****************`*************************************
///**Function:initDatabase
///**Description:��ʼ����������
///**Input: QString dbFileName:���������ļ���
///**Output:
///**Return: bool : ���������Ƿ�ɹ���־
///**Others:
///****************************************************/
bool SQLProecss::initDatabase(const QString dbFileName)
{
    if(!prop)
    {
        prop = new Properties(dbFileName);
    }

    sqlDatabase = QSqlDatabase::addDatabase(prop->getValue("dataBase"));
    sqlDatabase.setHostName(prop->getValue("hostName"));
    sqlDatabase.setDatabaseName(prop->getValue("databaseName"));
    sqlDatabase.setUserName(prop->getValue("userName"));
    sqlDatabase.setPassword(prop->getValue("password"));

    return sqlDatabase.open();
}

///*****************************************************
///**Function:obtainData
///**Description:ͨ��ִ�����ݿ���ʣ������������ͽ��н���
///**Input:  QString sql : ��ѯ����
///**Output: DataList &result : �����ѯ�����ݽ��
///**Return:
///**Others:�ɡ�����װ��㡿����
///****************************************************/
const bool SQLProecss::obtainData(const QString sql, DataList & result)
{
    QSqlQuery query(sqlDatabase);

//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;

    if(query.exec(sql))
    {
        while(query.next())
        {
            VariantList variantList;

            for(int index = 0;  index < query.record().count();  index++ )
            {
                variantList.push_back( query.record().value(index) );
            }

            result.push_back(variantList);
        }
        return true;
    }
    return false;
}

///*****************************************************
///**Function:getSqlDataCount
///**Description:��ȡ��Ӧ���������ݵ�����
///**Input:  const QString sql������ִ������
///**Output:
///**Return: const int:���ݵ�����
///**Others:
///****************************************************/
const int SQLProecss::getSqlDataCount(const QString sql)
{
    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        if(query.next())
        {
            return query.value(0).toInt();
        }
    }
    return 0;
}

///*****************************************************
///**Function:insertData
///**Description:ͨ�á����������ݲ���
///**Input: const QString &sql����������������
///**Output:
///**Return: const bool : ����ɹ����ı�־
///**Others:
///****************************************************/
const bool SQLProecss::insert(const QString &sql, int &lastInserId)
{
    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        lastInserId = query.lastInsertId().toInt();
        return true;
    }
    return false;
}

///*****************************************************
///**Function:execute
///**Description:ִ��ͨ�õ�ɾ�������µ�ֻ�践�سɹ��������
///**Input:const QString sql����ִ�е��������
///**Output:��
///**Return:const bool��ִ�гɹ����
///**Others:
///****************************************************/
const bool SQLProecss::execute(const QString sql)
{
    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        return true;
    }
    return false;
}

///*****************************************************
///**Function:transaction��commit��rollback
///**Description:���ڿ������ݿ���������
///**Input:
///**Output:
///**Return:const bool : ��ǰ�����Ƿ�ɹ�
///**Others:
///****************************************************/
const bool SQLProecss::transaction()
{
    return sqlDatabase.transaction();
}

const bool SQLProecss::commit()
{
    return sqlDatabase.commit();
}

const bool SQLProecss::rollback()
{
    return sqlDatabase.rollback();
}

SQLProecss::~SQLProecss()
{
    if(process)
    {
        delete process;
        process = NULL;
    }

    if(prop)
    {
        delete prop;
        prop = NULL;
    }
}
