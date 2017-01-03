#include "serviceinfoprocess.h"

#include "sqldataadapter.h"
#include "../global.h"

#include <QDebug>

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
bool ServiceInfoProcess::getServiceProperty(QList<ServiceProperty *> &properties)
{
    QString sql = "select s.id,s.name,s.status,s.description,sd.url,s.method,s.outputName,s.outputType from\
            business_softwareonline s left join business_softwareonline_deploy sd\
                on s.id = sd.relationId where s.status = '"+QString(SERVICE_ENABLE)+"'";
    bool flag = SQLDataAdapter::instance()->getSericeProperties(sql,properties);

    if(flag)
    {
        QString psql = "select p.id,p.parameterName,p.parameterType,p.addTime from\
                business_softwareonline s left join business_softwareonline_parameter p\
                on s.id = p.relationId where s.id = ";

        foreach(ServiceProperty *prop,properties)
        {
            QList<Parameter> paras;

            QString fsql = psql + " '"+prop->id+"' ";

            bool tFlag = SQLDataAdapter::instance()->getParameterById(fsql,paras);
            if(tFlag)
            {
                  foreach(Parameter para,paras)
                  {
                    if(para.pId.size() > 0)
                    {
                        Parameter * p = new Parameter;
                        p->pId = para.pId;
                        p->pName = para.pName;
                        p->pType = para.pType;
                        p->pRemark = para.pRemark;
                        prop->inputParas.append(p);
                    }
                  }
            }
        }
    }

   return flag;
}

//���÷����״̬
bool ServiceInfoProcess::setServiceStatus(QString id,bool isEnable)
{
    QString tableName = "business_softwareonline";
    QMap<QString,QString> updateKey;
    if(isEnable)
    {
        updateKey.insert("status",QString("'"+QString(SERVICE_ENABLE)+"'"));
    }
    else
    {
        updateKey.insert("status",QString("'"+QString(SERVICE_DISABLE)+"'"));
    }

    QMap<QString,QString> conditionKeyAndValue;
    conditionKeyAndValue.insert("id",QString("'"+id+"'"));

    return updateData(tableName,updateKey,conditionKeyAndValue);
}

//���ݷ��������Ҷ�Ӧ��URL
bool ServiceInfoProcess::getUrlByServiceName(QString sname,QString &result)
{
    QString sql = "select sd.url from business_softwareonline s left join business_softwareonline_deploy sd\
            on s.id = sd.relationId where s.name='"+sname+"'";
    return SQLDataAdapter::instance()->getServiceUrl(sql,result);
}

//��ȡ���µĴ�����Ϣ
QString ServiceInfoProcess::getLastError()
{
    return GlobalLastSQLError;
}
