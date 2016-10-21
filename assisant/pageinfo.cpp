#include "pageinfo.h"
#include "math.h"


PageInfo::PageInfo()
{
    reset();
}

void PageInfo::reset()
{
    perPageShow = 0;
    totalNums = 0;
    totalPages = 0;
    currPage = 0;
}

const QString PageInfo::setFormat(const QString &format)
{
    return QString::number(this->currPage+1)+format+QString::number(this->totalPages);
}

//�ֶ�������������
void PageInfo::setTotalNum(const int num)
{
    this->totalNums = num;
    countTotalPage();
}

//�ֶ����õ�ǰ��ʾ��ҳ��
void PageInfo::setCurrPage(const int num)
{
    if(num<0 || num >= totalPages)
    {
        this->currPage = 0;
    }
    else
    {
        this->currPage = num;
    }
}

//����һ����¼
void PageInfo::addNum()
{
    totalNums++;
    countTotalPage();
}

void PageInfo::countTotalPage()
{
    totalPages = ceil((float)totalNums/perPageShow);
}

//��һҳ
int PageInfo::getPrePage()
{
    currPage = (currPage-1<=0)?0:currPage-1;

    return this->currPage;
}

//��һҳ
int PageInfo::getNextPage()
{
    currPage = (currPage+1>=totalPages)?totalPages-1:currPage+1;

    fixCurrPage();

    return this->currPage;
}

//��һҳ
int PageInfo::getFirstPage()
{
    this->currPage = 0;
    return this->currPage;
}

//���һҳ
int PageInfo::getLastPage()
{
    currPage = totalPages -1;

    fixCurrPage();

    return this->currPage;
}

//��֤���޸���ǰҳ��Ϊ�������
void PageInfo::fixCurrPage()
{
    currPage = (currPage<=0)?0:currPage;
}
