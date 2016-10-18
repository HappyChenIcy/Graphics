/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: serviceinfoprocess.h
**��  ��: wey       Version: 1.0       Date: 2016.10.18
**��  ��:
**Others: ������Ϣ����
**
**�޸���ʷ:
**
*************************************************/
#ifndef SERVICEINFOPROCESS_H
#define SERVICEINFOPROCESS_H

#include "abstractdataprocess.h"
#include "../Header.h"

class ServiceInfoProcess : public AbstractDataProcess
{
public:
    static ServiceInfoProcess * instance();
    void getServiceInfo(ServiceInfoList & list,const int startIndex = 0,const int perPage = 0);

private:
    ServiceInfoProcess();
    static ServiceInfoProcess * serviceProcess;

};

#endif // SERVICEINFOPROCESS_H
