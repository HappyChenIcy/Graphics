#include "sqlproecss.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "../assisant/properties.h"
#include "../global.h"
#include "../util.h"

#include <QDebug>

QSqlDatabase sqlDatabase;

#define MY_CHECK_DATABASE {if(!sqlDatabase.isOpen()){GlobalLastSQLError = "���ݿ�����δ��!" ; return false;}}

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

    bool isOpen = sqlDatabase.open();

    if(!isOpen)
    {
        GlobalLastSQLError = sqlDatabase.lastError().text();
    }

    return isOpen;
}

///*****************************************************
///**Function:obtainData
///**Description:ͨ��ִ�����ݿ���ʣ������������ͽ��н���
///**Input:  QString sql : ��ѯ����
///**Output: DataList &result : �����ѯ�����ݽ��
///**Return:
///**Others:�ɡ�����װ��㡿����
///****************************************************/
bool SQLProecss::obtainData(const QString sql, DataList & result)
{
    MY_CHECK_DATABASE

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
    else
    {
       GlobalLastSQLError = query.lastError().text();
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
int SQLProecss::getSqlDataCount(const QString sql)
{
    MY_CHECK_DATABASE

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
    else
    {
        GlobalLastSQLError = query.lastError().text();
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
bool SQLProecss::insert(const QString &sql, int &lastInserId)
{
    MY_CHECK_DATABASE

    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        lastInserId = query.lastInsertId().toInt();
        return true;
    }
    else
    {
        GlobalLastSQLError = query.lastError().text();
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
bool SQLProecss::execute(const QString sql)
{
    MY_CHECK_DATABASE

    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        return true;
    }
    else
    {
        GlobalLastSQLError = query.lastError().text();
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
bool SQLProecss::transaction()
{
    return sqlDatabase.transaction();
}

bool SQLProecss::commit()
{
    return sqlDatabase.commit();
}

bool SQLProecss::rollback()
{
    return sqlDatabase.rollback();
}

//��ȡ���µĴ�����Ϣ
QString SQLProecss::getLastError()
{
    return GlobalLastSQLError;
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
