/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: util.h
**��  ��: wey       Version: 1.0       Date: 2016.XX.XX
**��  ��:
**Others:
**
**�޸���ʷ:
**20161018:gx:���ȽǶȻ���radinToAngle
**20161026:wey:����ȫ�ֵĵ�����
**20161108:wey:��Ӷ�״̬ͼ/���ֵ�֧��
*************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <QColor>
#include <QFont>
#include <QPixmap>
#include <QMessageBox>

#include "Header.h"

class QWidget;
class QTableWidget;

class Util
{
public:
    static void setWidgetColor(QWidget * widget,QColor & color);
    static QString getFontInfo(QFont font);
    static qreal getGlobalZValue();
    static void  resetGlobalZValue();
    static QString getUUID();
    static qreal getMaxNum(qreal & a,qreal & b);
    static void setTableStylesheet(QTableWidget * table);

    static void createTableItem(const int rowCount,QTableWidget * table);
    static void removeTableItem(QTableWidget * table);
    static void clearTableData(QTableWidget * table);
    static qreal radinToAngle(const qreal &radin);
    static void loadPixmapByGType(GraphicsType type,QPixmap & pixmap);

    static void showWarn(QString tipText);
    static int getWarnChoice(QString tipText, QMessageBox::StandardButton butt = QMessageBox::No);
    static void showInfo(QString tipText);
    static QMessageBox::StandardButton getInfoChoice(QString tipText,QMessageBox::StandardButton butt = QMessageBox::Yes);
};

#endif // UTIL_H
