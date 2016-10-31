/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: sqldataadapter.h
**��  ��: wey       Version: 1.0       Date: 2016.07.09
**��  ��:���ݿ�װ����
**Others:
**       1�����ڽ�������ʹ�ý��洫�ݵ����ݷ�������
**       2���������ݷ��ʲ㡾SQLProecss��ִ������
**       3����ȡ���ݷ��ʲ���������װ�䵽��Ӧ�Ľṹ�壬��������ʹ�ò�ʹ��
**       4����װ����ֻ�ڲ�ѯʱ�ŵ��á�
**
**�޸���ʷ:
**20160709:wey:�����ļ�����SQL���ʲ��ֽ��кϲ���
**20161031:wey:���ӶԲ����Ķ�ȡ
*************************************************/
#ifndef SQLDATAADAPTER_H
#define SQLDATAADAPTER_H

#include <QObject>

#include "../Header.h"

class SQLDataAdapter
{
public:
    static SQLDataAdapter * instance();

    bool getServiceData(QString sql,ServiceInfoList &list);

    bool getSericeProperties(QString sql, QList<ServiceProperty *> &list);
    bool getParameterById(QString sql,QList<Parameter> & list);

private:
    SQLDataAdapter();
    ~SQLDataAdapter();

    static SQLDataAdapter * adapter;

};

#endif // SQLDATAADAPTER_H
