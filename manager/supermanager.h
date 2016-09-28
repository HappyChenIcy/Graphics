#ifndef SUPERMANAGER_H
#define SUPERMANAGER_H

#include <QMap>

#include "id.h"

template<class T>
class SuperManager
{
public:
    bool contains(const char * name);

    //qmap��key���Ϊ�Զ��������ͣ���ôҪ����<�����
    QMap<Id,T*> maps;
};
#endif // SUPERMANAGER_H
