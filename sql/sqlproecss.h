/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: initdatamanager.h
**��  ��: wey       Version: 1.0       Date: 2016.07.09
**��  ��: ���ݷ��ʲ�
**Others:1���������ݿ�����
**       2����������װ��㡾SQLDataAdapter�������ݷ������󣬽�ִ�н������
**       3������Ψһ�����ݿ���ʵ���ڣ��˲㲻���ľ�����������ԣ�ִֻ�в�����
**
**�޸���ʷ:
**20160709:wey:�����ʸĳɵ���ģʽ
**             �ṩ����ͨ�õ����ݿ���ʲ�������ֱ��������װ�����н���
**20161026:wey:���ӶԴ�����Ϣ�ı���ͻ�ȡ
*************************************************/
#ifndef SQLPROECSS_H
#define SQLPROECSS_H

#include "../Header.h"

class Properties;

class SQLProecss
{
public:
    static SQLProecss * instance();
    bool initDatabase(const QString dbFileName);

    int  getSqlDataCount(const QString sql);
    bool insert(const QString & sql,int & lastInserId);
    bool execute(const QString sql);
    bool obtainData(const QString sql, DataList & result);

    QString getLastError();

    bool transaction();
    bool commit();
    bool rollback();

private:
    SQLProecss();
    ~SQLProecss();

    static SQLProecss * process;

    Properties * prop;
};

#endif // SQLPROECSS_H
