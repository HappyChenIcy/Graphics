#include "serviceinfoprocess.h"

#include "sqldataadapter.h"
#include "../global.h"

ServiceInfoProcess * ServiceInfoProcess::serviceProcess = NULL;

ServiceInfoProcess::ServiceInfoProcess()
{
}

ServiceInfoProcess * ServiceInfoProcess::instance()
{
    if(!serviceProcess)
    {
        serviceProcess = new ServiceInfoProcess;
    }
    return serviceProcess;
}

//��ҳ��ѯ����Ļ�����Ϣ
bool ServiceInfoProcess::getServiceInfo(ServiceInfoList &list, const int startIndex, const int perPage)
{
    QString sql = "select * from business_softwareonline ";
    if(perPage != 0)
    {
        sql += (" limit ")+QString::number(startIndex) +","+QString::number(perPage);
    }

    return SQLDataAdapter::instance()->getServiceData(sql,list);
}

//��ȡ���е�������Ϣ
bool ServiceInfoProcess::getServiceProperty(QList<ServiceProperty> &properties)
{
    QString sql = "select s.name,s.status,s.descirption,sd.url,s.method from\
            business_softwareonline s left join business_softwareonline_deploy sd\
                on s.id = sd.relationId";

   return SQLDataAdapter::instance()->getSericeProperties(sql,properties);
}

//��ȡ���µĴ�����Ϣ
QString ServiceInfoProcess::getLastError()
{
    return GlobalLastSQLError;
}
