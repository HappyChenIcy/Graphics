#include "util.h"

#include <QWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QUuid>

#include "qmath.h"
#include "global.h"

#define TABLE_ROW_HEIGHT  30      //���߶�

void Util::setWidgetColor(QWidget *widget, QColor &color)
{
    widget->setStyleSheet("background-color:rgba("+QString::number(color.red())\
                          +","+QString::number(color.green())\
                          +","+QString::number(color.blue())\
                          +","+QString::number(color.alpha())+")");
}

//��ȡ�������Ϣ
QString Util::getFontInfo(QFont font)
{
    return QString("%1px,%2").arg(font.pointSize()).arg(font.family());
}

//����ÿ���ؼ���������ȣ�����ӻ���ʱ���Զ�����
qreal Util::getGlobalZValue()
{
    GlobalItemZValue += 0.1;
    return GlobalItemZValue;
}

//����տؼ�ʱ�Զ���0
void Util::resetGlobalZValue()
{
    GlobalItemZValue = 0;
}

//��ȡΨһ��ʶ��
QString Util::getUUID()
{
    return QUuid::createUuid().toString();
}

qreal Util::getMaxNum(qreal &a, qreal &b)
{
    if(a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
    return 0;
}

//���ñ�ͷ����ʽ
void Util::setTableStylesheet(QTableWidget *table)
{
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setFocusPolicy(Qt::NoFocus);
    table->setStyleSheet("selection-color:white;selection-background-color:#567dbc");//
//    table->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;border:none;border-radius:5px;height:23px;margin-left:1px; \
//                                                     background-color:#567dbc;}");
    table->horizontalHeader()->setVisible(true);
}

//�������Ԫ��,�������ø��б�ɫ
void Util::createTableItem(const int rowCount, QTableWidget *table)
{
    for(int i=0 ; i<rowCount ; i++)
    {
        int row = table->rowCount();
        table->setRowCount(row+1);
        table->setRowHeight(row,TABLE_ROW_HEIGHT);

        for(int j = 0 ; j < table->columnCount(); j++)
        {
            QTableWidgetItem * item = new QTableWidgetItem();
            item->setTextColor(Qt::black);
            item->setTextAlignment(Qt::AlignCenter);

            if(row%2==0)
            {
                item->setBackgroundColor(QColor(100,100,100,15));
            }
            table->setItem(i,j,item);
        }
    }
}

//ɾ����������еĵ�Ԫ��(����ͷ)
void Util::removeTableItem(QTableWidget *table)
{
    int rowCount = table->rowCount();

    for(int i = rowCount-1; i >= 0; i--)
    {
         table->removeRow(i);
    }
}

//�����������(����ͷ)
void Util::clearTableData(QTableWidget *table)
{
    int rowCount = table->rowCount();
    int columnCount = table->columnCount();
    for(int i = 0 ; i <rowCount ; i++ )
    {
        for(int j = 0 ; j < columnCount;  j++)
        {
            table->item(i,j)->setText("");
        }
    }
}

//������ת��Ϊ�Ƕ�
qreal Util::radinToAngle(const qreal &radin)
{
     return 180/M_PI*radin;
}

//���ݲ�ͬ���ͼ��ض�Ӧ��ͼƬ
void Util::loadPixmapByGType(GraphicsType type, QPixmap &pixmap)
{
    switch(type)
    {
        case GRA_SQUARE:
                           pixmap.load(":/images/square.png");
                           break;
        case GRA_RECT:
                           pixmap.load(":/images/rectange.png");
                           break;
        case GRA_ROUND_RECT:
                           pixmap.load(":/images/roundedrect.png");
                           break;
        case GRA_CIRCLE:
                           pixmap.load(":/images/circle.png");
                           break;
        case GRA_ELLIPSE:
                           pixmap.load(":/images/ellipse.png");
                           break;
        case GRA_POLYGON:
                           pixmap.load(":/images/diamonds.png");
                           break;
        case GRA_TEXT:
                           pixmap.load(":/images/text.png");
                           break;
        case GRA_NODE_PORT:
                           pixmap.load(":/images/nodePort.png");
                           break;
        case GRA_PARALLELOGRAM:
                           pixmap.load(":/images/parallelogram.png");
                           break;
        case GRA_LOOP_UP:
                           pixmap.load(":/images/loop_up.png");
                           break;
        case GRA_LOOP_DOWN:
                           pixmap.load(":/images/loop_down.png");
                           break;
        case GRA_ANNOTATION:
                           pixmap.load(":/images/annotation.png");
                           break;
        case GAR_PARALLE:
                           pixmap.load(":/images/parallel.png");
                           break;
        default:
                           break;
    }
}
