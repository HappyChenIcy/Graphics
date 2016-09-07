#include "righttoolbox.h"
#include "ui_righttoolbox.h"

#include <QFontDialog>
#include <QColorDialog>
#include <QDebug>

#include "../util.h"
#include "../global.h"

RightToolBox::RightToolBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RightToolBox)
{
    ui->setupUi(this);

    setFixedWidth(300);

    enableButtState(false);

    initConnection();

    isDataInit = false;

    ui->tabWidget->setCurrentIndex(0);
}

//��ʼ���źŲ�����
void RightToolBox::initConnection()
{
    //��ʽ-��ʽ
    connect(ui->butt_red,SIGNAL(clicked()),this,SLOT(changeFillColor()));
    connect(ui->butt_blue,SIGNAL(clicked()),this,SLOT(changeFillColor()));
    connect(ui->butt_orange,SIGNAL(clicked()),this,SLOT(changeFillColor()));
    connect(ui->butt_yellow,SIGNAL(clicked()),this,SLOT(changeFillColor()));
    connect(ui->butt_gray,SIGNAL(clicked()),this,SLOT(changeFillColor()));
    connect(ui->butt_white,SIGNAL(clicked()),this,SLOT(changeFillColor()));

    connect(ui->opacitySlider,SIGNAL(valueChanged(int)),this,SLOT(opacityChange(int)));

    connect(ui->colorFill,SIGNAL(toggled(bool)),this,SLOT(isViewFill(bool)));
    connect(ui->colorFillButt,SIGNAL(clicked()),this,SLOT(chooseFillColor()));
    connect(ui->fillKind,SIGNAL(currentIndexChanged(int)),this,SLOT(fillKindChange(int)));

    connect(ui->borderChecked,SIGNAL(toggled(bool)),this,SLOT(isViewBorder(bool)));
    connect(ui->lineColorButt,SIGNAL(clicked()),this,SLOT(chooseLineColor()));
    connect(ui->lineKind,SIGNAL(currentIndexChanged(int)),this,SLOT(borderKindChange(int)));
    connect(ui->lineWidth,SIGNAL(currentIndexChanged(int)),this,SLOT(borderWidthChange(int)));

    //��ʽ-����
    connect(ui->sizeWM,SIGNAL(clicked()),this,SLOT(sizeChange()));    //��ȳߴ����
    connect(ui->sizeWP,SIGNAL(clicked()),this,SLOT(sizeChange()));
    connect(ui->sizeHM,SIGNAL(clicked()),this,SLOT(sizeChange()));    //�߶ȳߴ����
    connect(ui->sizeHP,SIGNAL(clicked()),this,SLOT(sizeChange()));

    connect(ui->posXM,SIGNAL(clicked()),this,SLOT(positionChange()));    //��ȳߴ����
    connect(ui->posXP,SIGNAL(clicked()),this,SLOT(positionChange()));
    connect(ui->posYM,SIGNAL(clicked()),this,SLOT(positionChange()));    //�߶ȳߴ����
    connect(ui->posYP,SIGNAL(clicked()),this,SLOT(positionChange()));

    connect(ui->degreeM,SIGNAL(clicked()),this,SLOT(degreeChange()));
    connect(ui->degreeP,SIGNAL(clicked()),this,SLOT(degreeChange()));
    connect(ui->dial,SIGNAL(valueChanged(int)),this,SLOT(changeDegree(int)));

    connect(ui->deleteItem,SIGNAL(clicked()),this,SLOT(deleteItem()));

    connect(ui->selectFont,SIGNAL(clicked()),this,SLOT(chooseFont()));
}

//�ı�ؼ��������ɫ
void RightToolBox::changeFillColor()
{
    QPushButton * butt =dynamic_cast<QPushButton *>(QObject::sender());
    QString objname = butt->objectName().right(butt->objectName().length()-butt->objectName().indexOf("_")-1);

    if(objname=="red")
    {
         selectedFillColor.setRed(255);
         selectedFillColor.setGreen(0);
         selectedFillColor.setBlue(0);
    }
    else if(objname=="blue")
    {
        selectedFillColor.setRed(0);
        selectedFillColor.setGreen(85);
        selectedFillColor.setBlue(255);
    }
    else if(objname=="orange")
    {
        selectedFillColor.setRed(255);
        selectedFillColor.setGreen(170);
        selectedFillColor.setBlue(0);
    }
    else if(objname=="yellow")
    {
        selectedFillColor.setRed(255);
        selectedFillColor.setGreen(255);
        selectedFillColor.setBlue(0);
    }else if(objname=="gray")
    {
        selectedFillColor.setRed(114);
        selectedFillColor.setGreen(120);
        selectedFillColor.setBlue(108);
    }
    else if(objname=="white")
    {
        selectedFillColor.setRed(255);
        selectedFillColor.setGreen(255);
        selectedFillColor.setBlue(255);
    }
    initalFillColor = selectedFillColor;
    setFillColor();
    prepareBrush();
}

//͸���ȵĸı�
void RightToolBox::opacityChange(int value)
{
    if(isDataInit)
    {
        return;
    }

    isDataInit = false;
    ui->opacityShow->setText(QString::number(value)+"%");

    selectedFillColor.setAlpha((float)ui->opacitySlider->value()/100*255);
    selectedLineColor.setAlpha((float)ui->opacitySlider->value()/100*255);
    currItemProperty.alphaValue = value;
    setFillColor();
    setLineColor();

    if(!isDataInit)
    {
        preparePen();
        prepareBrush();
    }
}

//�Ƿ���ʾ���ɫ
void RightToolBox::isViewFill(bool flag)
{
    Q_UNUSED(flag)
    prepareBrush();
}

//��ˢ���ı�
void RightToolBox::fillKindChange(int index)
{
    Q_UNUSED(index)
    prepareBrush();
}

void RightToolBox::isViewBorder(bool flag)
{
    Q_UNUSED(flag)
    preparePen();
}

//��ʾ������ɫѡ��Ի���
void RightToolBox::chooseLineColor()
{
    selectedLineColor = QColorDialog::getColor(initalLineColor,0,"ѡ��߿�ɫ");
    selectedLineColor.setAlpha((float)ui->opacitySlider->value()/100*255);
    if(selectedLineColor.isValid())
    {
        initalLineColor = selectedLineColor;
        setLineColor();
        newPen.setColor(selectedLineColor);
        currItemProperty.itemPen = newPen;
        updateItemProperty();
    }
}

//�߿����͸ı�
void RightToolBox::borderKindChange(int i)
{
    Q_UNUSED(i)
    preparePen();
}

//�߿��ȸı�
void RightToolBox::borderWidthChange(int i)
{
    Q_UNUSED(i)
    preparePen();
}

void RightToolBox::preparePen()
{
    if(isDataInit)
    {
        return;
    }

    isDataInit = false;
    if(!ui->borderChecked->isChecked())
    {
        newPen.setStyle(Qt::NoPen);
    }
    else
    {
        switch(ui->lineKind->currentIndex())
        {
          case 0:
                 newPen.setStyle(Qt::SolidLine);
                 break;
          case 1:
                 newPen.setStyle(Qt::DashLine);
                 break;
          case 2:
                 newPen.setStyle(Qt::DotLine);
                 break;
          case 3:
                 newPen.setStyle(Qt::DashDotLine);
                 break;
          case 4:
                 newPen.setStyle(Qt::DashDotDotLine);
                 break;
        }
    }
    newPen.setColor(selectedLineColor);
    newPen.setWidth(ui->lineWidth->currentIndex()+1);

    currItemProperty.isNeedBorder = ui->borderChecked->isChecked();
    currItemProperty.itemPen = newPen;

    updateItemProperty();
}

//��ʾ��ɫѡ��Ի���
void RightToolBox::chooseFillColor()
{
    selectedFillColor = QColorDialog::getColor(initalFillColor,0,"ѡ�����ɫ");
    selectedFillColor.setAlpha((float)ui->opacitySlider->value()/100*255);
    prepareBrush();
}

//��ˢ��ʽ׼��
void RightToolBox::prepareBrush()
{
    if(isDataInit)
    {
       return;
    }

    isDataInit = false;

    if(!ui->colorFill->isChecked())
    {
         newBrush.setStyle(Qt::NoBrush);
         currItemProperty.isNeedBrush = false;
    }
    else
    {
        currItemProperty.isNeedBrush = true;

        if(selectedFillColor.isValid())
        {
            initalFillColor = selectedFillColor;
            setFillColor();
            newBrush.setStyle(Qt::SolidPattern);
            newBrush.setColor(selectedFillColor);
        }

        switch(ui->fillKind->currentIndex())
        {
           case 0:newBrush.setStyle(Qt::SolidPattern);break;
           case 1:newBrush.setStyle(Qt::HorPattern);break;
           case 2:newBrush.setStyle(Qt::VerPattern);break;
           case 3:newBrush.setStyle(Qt::CrossPattern);break;
           case 4:newBrush.setStyle(Qt::BDiagPattern);break;
           case 5:newBrush.setStyle(Qt::FDiagPattern);break;
           case 6:newBrush.setStyle(Qt::DiagCrossPattern);break;
        }

        currItemProperty.itemBrush = newBrush;
    }
    updateItemProperty();
}

//���������ɫ
void RightToolBox::setFillColor()
{
    Util::setWidgetColor(ui->colorFillButt,selectedFillColor);
}

//����������ɫ
void RightToolBox::setLineColor()
{
    Util::setWidgetColor(ui->lineColorButt,selectedLineColor);
}

//�ı�ߴ�
void RightToolBox::sizeChange()
{
    QString tname = dynamic_cast<QPushButton *>(QObject::sender())->objectName();
    if(tname=="sizeWM")
    {
        setNum(ui->sizew,true,currItemProperty.itemRect.width);
    }
    else if(tname=="sizeWP")
    {
        setNum(ui->sizew,false,currItemProperty.itemRect.width);
    }
    else if(tname=="sizeHM")
    {
        setNum(ui->sizeh,true,currItemProperty.itemRect.height);
    }
    else if(tname=="sizeHP")
    {
        setNum(ui->sizeh,false,currItemProperty.itemRect.height);
    }
    updateItemProperty();
}

//�ı�ߴ�֮�����·��͸��ؼ���
void RightToolBox::updateItemProperty()
{
    emit updateProperty(this->currItemProperty);
}

void RightToolBox::positionChange()
{
    QString tname = dynamic_cast<QPushButton *>(QObject::sender())->objectName();
    if(tname=="posXM")
    {
        setNum(ui->posx,true,currItemProperty.itemRect.x);
    }
    else if(tname=="posXP")
    {
        setNum(ui->posx,false,currItemProperty.itemRect.x);
    }
    else if(tname=="posYM")
    {
        setNum(ui->posy,true,currItemProperty.itemRect.y);
    }
    else if(tname=="posYP")
    {
        setNum(ui->posy,false,currItemProperty.itemRect.y);
    }
    updateItemProperty();
}

//������ֵ�ļӼ�����
void RightToolBox::setNum(QLabel *edit, bool minus, int &newValue)
{
    int num = getSizeValue(edit,2);

    if(minus)
    {
         num = (--num)>=1?num:1;
    }
    else
    {
         num = (++num)>= SceneWidth ? SceneWidth :num;
    }
    newValue = num;

    edit->setText(QString::number(num)+"px");
}

//��ÿؼ��е���ֵ,pos��ʾ��λ
int RightToolBox::getSizeValue(QLabel* edit,int pos)
{
    QString text = edit->text();
    return text.left(text.size()-pos).toInt();
}


//��ת�Ƕȵĸı䣬��С��0������360.
void RightToolBox::degreeChange()
{
    QPushButton * tmp = dynamic_cast<QPushButton *>(QObject::sender());
    QString objname = tmp->objectName();
    int degree = getSizeValue(ui->degreeValue,1);

    if(objname==ui->degreeM->objectName())
    {
        degree = (degree<=-360)?-360:--degree;
    }
    else
    {
        degree = (degree>=360)?360:++degree;
    }

    changeDegree(degree);

    ui->dial->setValue(degree);
}


//Dial�ı�ֵ
void RightToolBox::changeDegree(int value)
{
    if(isDataInit)
    {
        return;
    }
    isDataInit = false;
    ui->degreeValue->setText(QString::number(value)+"��");

    currItemProperty.rotateDegree = value;

    updateItemProperty();
}

//ɾ��ѡ�е�һ��ͼ��
void RightToolBox::deleteItem()
{
    emit deleteCurrItem();
}

//��ʾ����Ի���
void RightToolBox::chooseFont()
{
    bool ok;
    QFont seFont = QFontDialog::getFont(&ok,currItemProperty.itemFont, this);
    if (ok)
    {
        currItemProperty.itemFont = seFont;
        updateItemProperty();
    }
}

//���ƿؼ��Ƿ����ʹ��
void RightToolBox::enableButtState(bool isEnabled)
{
   ui->widget->setEnabled(isEnabled);
   ui->widget_2->setEnabled(isEnabled);
}

//����ѡ�в�ͬitem�����ù�����״̬
void RightToolBox::respInitToolBox(int seletedItemNum,ItemProperty property)
{
    if(seletedItemNum == 1)
    {
        enableButtState(true);
        this->currItemProperty = property;

        isDataInit = true;

        //��ת�Ƕ�
        ui->dial->setValue(currItemProperty.rotateDegree);
        ui->degreeValue->setText(QString::number(currItemProperty.rotateDegree)+"��");

        //w��h��x��y
        ui->sizew->setText(QString::number(currItemProperty.itemRect.width)+"px");
        ui->sizeh->setText(QString::number(currItemProperty.itemRect.height)+"px");
        ui->posx->setText(QString::number(currItemProperty.itemRect.x)+"px");
        ui->posy->setText(QString::number(currItemProperty.itemRect.y)+"px");

        ui->colorFill->setChecked(currItemProperty.isNeedBrush);
        ui->borderChecked->setChecked(currItemProperty.isNeedBorder);

        //������ɫ
        selectedFillColor = currItemProperty.itemBrush.color();
        selectedLineColor = currItemProperty.itemPen.color();

        QColor colorBrush = currItemProperty.itemBrush.color();
        Util::setWidgetColor(ui->colorFillButt,colorBrush);

        switch(currItemProperty.itemBrush.style())
        {
          case Qt::SolidPattern:
                 ui->fillKind->setCurrentIndex(0);
                 break;
          case Qt::HorPattern:
                 ui->fillKind->setCurrentIndex(1);
                 break;
          case Qt::VerPattern:
                 ui->fillKind->setCurrentIndex(2);
                 break;
          case Qt::CrossPattern:
                 ui->fillKind->setCurrentIndex(3);
                 break;
          case Qt::BDiagPattern:
                 ui->fillKind->setCurrentIndex(4);
                 break;
          case Qt::FDiagPattern:
                 ui->fillKind->setCurrentIndex(5);
                 break;
          case Qt::DiagCrossPattern:
                 ui->fillKind->setCurrentIndex(6);
                 break;
        }

        switch(currItemProperty.itemPen.style())
        {
              case Qt::SolidLine:
                     ui->lineKind->setCurrentIndex(0);
                     break;
              case Qt::DashLine:
                     ui->lineKind->setCurrentIndex(1);
                     break;
              case Qt::DotLine:
                     ui->lineKind->setCurrentIndex(2);
                     break;
              case Qt::DashDotLine:
                     ui->lineKind->setCurrentIndex(3);
                     break;
              case Qt::DashDotDotLine:
                     ui->lineKind->setCurrentIndex(4);
                     break;
        }

        ui->lineWidth->setCurrentIndex(currItemProperty.itemPen.width()-1);

        ui->opacitySlider->setValue(currItemProperty.alphaValue);
        ui->opacityShow->setText(QString::number(currItemProperty.alphaValue)+"%");

        isDataInit = false;
    }
    else
    {
        enableButtState(false);
    }
}

RightToolBox::~RightToolBox()
{
    delete ui;
}
