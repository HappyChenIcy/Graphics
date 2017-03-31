#ifndef GLOBAL_H
#define GLOBAL_H

#include "Header.h"

class MainWindow;

extern MainWindow * GlobalMainWindow;            //�����������

extern GraphicsType CurrAddGraType;              //��ǰ��ӵ�����

extern int SceneWidth,SceneHeight;               //scene�Ŀ�Ⱥ͸߶�
extern int ScreenWidth,ScreenHeight;             //��Ļ�Ŀ�Ⱥ͸߶�
extern qreal GlobalItemZValue;                   //item��������ȣ�����������ؼ�ʱ�Զ�������

extern QPointF SceneLastClickPoint;              //scene�����һ���������λ�ã���Ϊճ��ʱ��λ��

extern QString SaveFileSuffix;                   //�����ļ��ĺ�׺��
extern QString SaveFileHeadFlag;                 //�����ļ����ļ�ͷ�������ж���ѡ����ļ��Ƿ��ǳ���֧�ֵĸ�ʽ

extern QString GlobalLastSQLError;               //���ݿ���ʴ��󣬱������µĴ���

extern WindowState GlobalWindowState;            //ȫ�ֵĹ�����������

extern int GlobalRotateStep; 		 		 	  //��ת����Ӧ�ǶȲŽ��нǶ��л�
extern qreal GlobalMinMumSize;            		  //�ؼ���С����С��Χ

extern QList<ServiceProperty *> GlobalServiceProperties;     //ȫ�ֵķ�����Ϣ
extern QList<ServiceProperty *> PreExeServices;              //Ԥ�������
extern QList<ServiceProperty *> ResetExeServices;            //���÷���

extern bool GlobalIsSimulateState;               //�Ƿ�Ϊ����״̬����������������״̬���
extern bool IsConfigIniExisted;                  //ȫ�ֵ������ļ��Ƿ����

extern QMap<QString,QString> GlobalKeyMap;       //��ݼ�ע����Ϣ

#endif // GLOBAL_H
