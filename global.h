#ifndef GLOBAL_H
#define GLOBAL_H

#include "Header.h"

extern GraphicsType CurrAddGraType;              //��ǰ��ӵ�����

extern int SceneWidth,SceneHeight;               //scene�Ŀ�Ⱥ͸߶�
extern int ScreenWidth,ScreenHeight;             //��Ļ�Ŀ�Ⱥ͸߶�
extern qreal GlobalItemZValue;                   //item��������ȣ�����������ؼ�ʱ�Զ�������

extern QPointF SceneLastClickPoint;              //scene�����һ���������λ�ã���Ϊճ��ʱ��λ��

extern QString SaveFileSuffix;                   //�����ļ��ĺ�׺��
extern QString SaveFileHeadFlag;                 //�����ļ����ļ�ͷ�������ж���ѡ����ļ��Ƿ��ǳ���֧�ֵĸ�ʽ

#endif // GLOBAL_H
