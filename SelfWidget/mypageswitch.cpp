#include "mypageswitch.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QMessageBox>

#include "../manager/mypageitem.h"
#include "../util.h"
#include "../item/myscene.h"
#include "../item/mygraphicsview.h"
#include "../global.h"
#include "Header.h"

MyPageSwitch::MyPageSwitch(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(25);
    isFirstView = false;

    pagePosition = 0;
    selectedPage = NULL;
    initWidget();
}

void MyPageSwitch::initWidget()
{
    addPageButt = new QPushButton();
    addPageButt->setFixedSize(20,20);
    addPageButt->setToolTip("���빤��ҳ(Ctrl+N)");
    addPageButt->setText("+");
    connect(addPageButt,SIGNAL(clicked()),this,SLOT(addPage()));

    layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(addPageButt);
    layout->addStretch(0);
    setLayout(layout);
}

//���ҳ��
void MyPageSwitch::addPage()
{
    QString id = Util::getUUID();

    MyPageItem  * item = PageManager::instance()->addPageItem();
    connect(item,SIGNAL(switchPage(QString)),this,SLOT(switchToPage(QString)));
    connect(item,SIGNAL(deletePage(QString)),this,SLOT(deleteThisPage(QString)));
    item->setId(id);

    PageMapping * page = new PageMapping;
    page->id = id;
    page->pageItem = item;
    page->scene = MyGraphicsView::instance()->addScene(id);
    page->scaleView = 100;
    page->hScrollValue = SceneWidth /2;
    page->vScrollValue = SceneHeight /2;
    selectedPage = page;

    pages.append(page);
    layout->insertWidget(++pagePosition,item);
    item->setText(QString("������%1").arg(PageManager::instance()->getPageCount()));

    if(!isFirstView)
    {
        isFirstView = true;
        siwtchPage(id,isFirstView);
    }
    else
    {
        switchToPage(id);
    }
}

//�л�ҳ��.�л�ǰ��Ҫ�ȱ��浱ǰscene�еĳߴ���Ϣ
void MyPageSwitch::switchToPage(QString pageId)
{
    siwtchPage(pageId);
}

//�л�ҳ��
void MyPageSwitch::siwtchPage(QString pageId, bool firstView)
{
    if(selectedPage && !firstView)
    {
        selectedPage->hScrollValue = MyGraphicsView::instance()->getHorizonalValue();
        selectedPage->vScrollValue = MyGraphicsView::instance()->getVertiaclValue();
        selectedPage->scaleView = MyGraphicsView::instance()->getScaleValue();
    }

    foreach (PageMapping * mapping, pages)
    {
        if(mapping->id == pageId)
        {
            selectedPage = mapping;
            mapping->pageItem->setSelected(true);
            MyGraphicsView::instance()->transformView(mapping->hScrollValue,mapping->vScrollValue);
            MyGraphicsView::instance()->setScaleValue(mapping->scaleView);
            MyGraphicsView::instance()->showScene(mapping->scene);
        }
        else
        {
            mapping->pageItem->setSelected(false);
        }
    }
}

//ɾ����ǰҳ�棬�ú���һ��ҳ��
void MyPageSwitch::deleteThisPage(QString pageId)
{
    int result = QMessageBox::warning(0,"����","�Ƿ�ɾ���˹�����?",QMessageBox::Yes,QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        int index = -1;
        for( int i = 0; i < pages.size() ; i++)
        {
            if(pages.at(i)->id == pageId )
            {
                index = i;
                break;
            }
        }
        if( index >=0  && index < pages.size()-1)
        {
            switchToPage(pages.at(index + 1)->id);
        }
        else if(index == pages.size()-1 && index >0)
        {
            switchToPage(pages.at(index - 1)->id);
        }
        else if(index <0)
        {
            return;
        }

        pages.at(index)->scene->clear();

        delete pages.at(index)->scene;
        delete pages.at(index)->pageItem;
        pages.removeAt(index);

        --pagePosition;
        if(pages.size() == 0)
        {
            MyGraphicsView::instance()->deleteScene();
        }
        emit deletePage();
    }
}

//�رյ�ǰҳ
void MyPageSwitch::closePage()
{
    deleteThisPage(selectedPage->id);
}

MyPageSwitch * MyPageSwitch::pageSwitch = NULL;

MyPageSwitch * MyPageSwitch::instance()
{
    if(!pageSwitch)
    {
        pageSwitch = new MyPageSwitch;
    }
    return pageSwitch;
}
