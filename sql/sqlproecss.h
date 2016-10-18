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

    const int  getSqlDataCount(const QString sql);
    const bool insert(const QString & sql,int & lastInserId);
    const bool execute(const QString sql);
    const bool obtainData(const QString sql, DataList & result);

    const bool transaction();
    const bool commit();
    const bool rollback();

private:
    SQLProecss();
    ~SQLProecss();

    static SQLProecss * process;

    Properties * prop;
};

#endif // SQLPROECSS_H
