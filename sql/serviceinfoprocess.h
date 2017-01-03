/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: serviceinfoprocess.h
**��  ��: wey       Version: 1.0       Date: 2016.10.18
**��  ��:
**Others: ������Ϣ����
**
**�޸���ʷ:
**20170103:wey:��Ӹ��ݷ��������Ҷ�Ӧ��URL
*************************************************/
#ifndef SERVICEINFOPROCESS_H
#define SERVICEINFOPROCESS_H

#include "abstractdataprocess.h"
#include "../Header.h"

class ServiceInfoProcess : public AbstractDataProcess
{
public:
    static ServiceInfoProcess * instance();
    bool getServiceInfo(ServiceInfoList & list,const int startIndex = 0,const int perPage = 0);

    bool getServiceProperty(QList<ServiceProperty *> &properties);

    QString getLastError();

    bool setServiceStatus(QString id, bool isEnable);

    bool getUrlByServiceName(QString sname, QString &result);

private:
    ServiceInfoProcess();
    static ServiceInfoProcess * serviceProcess;

};

#endif // SERVICEINFOPROCESS_H
