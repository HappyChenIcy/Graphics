#ifndef RIGHTTOOLBOX_H
#define RIGHTTOOLBOX_H

#include <QWidget>
#include <QPen>
#include <QBrush>

#include "Header.h"

class QLabel;

namespace Ui {
class RightToolBox;
}

class RightToolBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit RightToolBox(QWidget *parent = 0);
    ~RightToolBox();

signals:
    void updateProperty(ItemProperty property);           //���µ��������õ�ѡ�е�item֮��
    void deleteCurrItem();

private slots:
    void respInitToolBox(int seletedItemNum,ItemProperty property);
    void respItemPosChanged(MyRect rect);

    void changeFillColor();              //�ı������ɫ
    void opacityChange(int value);       //�ı�͸����
    void isViewFill(bool flag);          //�Ƿ����
    void chooseFillColor();              //��ʾ��ɫѡ��Ի���
    void fillKindChange(int index);      //��ˢ���ı�
    void isViewBorder(bool flag);        //�Ƿ���ʾ�߿�
    void chooseLineColor();              //��ʾ������ɫѡ��Ի���
    void borderKindChange(int i);        //�߿����͸ı�
    void borderWidthChange(int i);       //�߿��ȸı�
    void sizeChange();                   //�ı�ߴ�
    void positionChange();               //�ı�λ��
    void degreeChange();                 //�ı�Ƕ�
    void changeDegree(int value);        //��תdiral
    void deleteItem();                   //ɾ��item
    void chooseFont();                   //ѡ������
    void chooseFontColor();              //��ʾ������ɫѡ��Ի���
    
private:
    void initConnection();
    void setFillColor();
    void setLineColor();
    void enableButtState(bool isEnabled);
    void prepareBrush();                 //׼����ˢ
    void preparePen();                   //׼������
    void setNum(QLabel *edit, bool minus,int &newValue);
    int getSizeValue(QLabel* edit,int pos);
    void updateItemProperty();

    Ui::RightToolBox *ui;

    bool isDataInit;                     //�Ƿ������ݵĳ�ʼ��ʱ���û�����ֵ����ô�Ͳ�Ҫ�ٷ��ź��ˡ�

    QBrush newBrush;
    QColor selectedFillColor;
    QColor selectedLineColor;
    QColor selectedFontColor;
    QColor initalFillColor,initalLineColor,initalFontColor;  //�����ڵ�������ɫѡ��Ի��������ó�ʼ����ɫ
    QPen newPen;

    ItemProperty currItemProperty;
};

#endif // RIGHTTOOLBOX_H
