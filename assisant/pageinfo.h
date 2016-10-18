/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: pageinfo.h
**��  ��: wey       Version: 1.0       Date: 2016.07.05
**��  ��: ҳ���¼
**Others:
**      1�����ڸ��ݵ�ǰ��¼����������װ��һҳ����һҳ�Ȳ�����
**      2�����ĵ��趨��ǰҳ��0��ʼ�������10����¼��ÿҳ��ʾ4������ôtotalPages==3�����Է��ʵ�ҳΪ0,1,2

**�޸���ʷ:
**20160706:wey:�޸��˳�ʼ�����ݶ����»�ȡ��һҳ���ݳ�������
**20160707:wey:����ֶ��趨��ҳ������
**             ����ֶ��趨��ǰ��ʾ��ҳ������
*************************************************/
#ifndef PAGEINFO_H
#define PAGEINFO_H

#include <QString>

class PageInfo
{
public:
    //ҳ���л��ķ���
    enum SwitchDirect
    {
        LEFT2RIGHT,            //��������
        RIGHT2LEFT,            //��������
        TOP2BOTTOM,            //��������
        BOTTOM2TOP             //��������
    };

    PageInfo();

    void addNum();
    void reset();
    void setPerPageShow(const int num){this->perPageShow =num;}
    void setTotalNum(const int num);
    void setCurrPage(const int num);
    const QString setFormat(const QString & format ="/");
    const int getTotalPages(){return this->totalPages;}
    const int getTotalNums(){return this->totalNums;}
    const int getCurrPage(){return this->currPage;}
    const int getPerPageShow(){return this->perPageShow;}
    const int getPrePage();
    const int getNextPage();
    const int getFirstPage();
    const int getLastPage();

private:
    void countTotalPage();
    void fixCurrPage();

    int perPageShow;           //ÿҳ��ʾ������
    int totalNums;             //������
    int totalPages;            //��ҳ��
    int currPage;              //��ǰҳ��
};

#endif // PAGEINFO_H
