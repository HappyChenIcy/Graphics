#include "id.h"

Id::Id(const char *name):
    idName(name)
{

}

//����"<"����������ڶ�key��������
bool operator<(const Id & id1,const Id & id2)
{
    return id1.idName.compare(id2.idName);
}

bool operator==(const Id & id1,const Id & id2)
{
    return id1.idName.compare(id2.idName);
}

Id::~Id()
{

}
