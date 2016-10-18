#include "pageswitchbar.h"
#include "ui_pageswitchbar.h"

#include "../assisant/pageinfo.h"

PageSwitchBar::PageSwitchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSwitchBar)
{
    ui->setupUi(this);

    pageInfo = new PageInfo;
    setPageShowNum();

    initConnection();
}

void PageSwitchBar::initConnection()
{
    connect(ui->firstPage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));
    connect(ui->prePage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));
    connect(ui->nextPage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));
    connect(ui->lastPage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));

    connect(ui->search,SIGNAL(clicked()),this,SLOT(respSearch()));
//    connect(ui->filterItem,SIGNAL(clicked()),this,SLOT(setFilterCondition()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(respPerPageChange(int)));
//    connect(ui->insertData,SIGNAL(clicked()),this,SIGNAL(signalInserData()));

//    connect(ui->close,SIGNAL(clicked()),this,SIGNAL(signalCloseDialog()));
}

//�ۺ���Ӧҳ���л�
void PageSwitchBar::respSwitchPage()
{
    QString objName = QObject::sender()->objectName();

    if(objName == "firstPage")
    {
        pageInfo->getFirstPage();
    }
    else if(objName == "prePage")
    {
        pageInfo->getPrePage();
    }
    else if(objName == "nextPage")
    {
        pageInfo->getNextPage();
    }
    else if(objName == "lastPage")
    {
        pageInfo->getLastPage();
    }

    respSearch();
}

//��ѯ
void PageSwitchBar::respSearch()
{
    showTipInfo();
    emit signalBeginSearch();
}

//��ʾ��ǰҳ/��ҳ��
void PageSwitchBar::showTipInfo()
{
    ui->currPage->setText(pageInfo->setFormat());
}

//��Ӧÿҳ���ݸı�����´ӵ�һҳ��ʾ��
void PageSwitchBar::respPerPageChange(int)
{
    pageInfo->reset();
    setPageShowNum();
    emit signalBeginSearch();
}

//����ÿҳ��ʾ������
void PageSwitchBar::setPageShowNum()
{
    QString currText = ui->comboBox->currentText();
    pageInfo->setPerPageShow(currText.left(currText.size()-1).toInt());
}

//����ÿҳ��ʾ������
void PageSwitchBar::setPerPageShow(const int num)
{
    pageInfo->setPerPageShow(num);
}

//����������
void PageSwitchBar::setTotalNum(const int num)
{
    pageInfo->setTotalNum(num);
}

//���õ�ǰҳ
void PageSwitchBar::setCurrPage(const int num)
{
    pageInfo->setCurrPage(num);
}

//��ȡ��׼�����
QString PageSwitchBar::getFormatOutput(const QString &format)
{
   return pageInfo->setFormat(format);
}

//��ȡ�ܵ�����
const int PageSwitchBar::getTotalNums()
{
    return pageInfo->getTotalNums();
}

//��ȡ��ǰ��ҳ����
const int PageSwitchBar::getCurrPage()
{
    return pageInfo->getCurrPage();
}

//��ȡÿҳ��ʾ������
const int PageSwitchBar::getPerPageShow()
{
    return pageInfo->getPerPageShow();
}

//��ȡ�ܵ�ҳ��
const int PageSwitchBar::getTotalPage()
{
    return pageInfo->getTotalPages();
}

PageSwitchBar::~PageSwitchBar()
{
    if(pageInfo)
    {
        delete pageInfo;
        pageInfo = NULL;
    }
    delete ui;
}
