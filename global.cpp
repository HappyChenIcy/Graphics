#include "global.h"

MainWindow * GlobalMainWindow;            //�����������

GraphicsType CurrAddGraType;              //��ǰ��ӵ�����

int SceneWidth,SceneHeight;               //scene�Ŀ�Ⱥ͸߶�
int ScreenWidth,ScreenHeight;             //��Ļ�Ŀ�Ⱥ͸߶�
qreal GlobalItemZValue;                   //item��������ȣ�����������ؼ�ʱ�Զ�������

QPointF SceneLastClickPoint;              //scene�����һ���������λ�ã���Ϊճ��ʱ��λ��

QString SaveFileSuffix;                   //�����ļ��ĺ�׺��
QString SaveFileHeadFlag;                 //�����ļ����ļ�ͷ�������ж���ѡ����ļ��Ƿ��ǳ���֧�ֵĸ�ʽ

QString GlobalLastSQLError;               //���ݿ���ʴ��󣬱������µĴ���

WindowState GlobalWindowState;            //ȫ�ֵĹ�����������

int GlobalRotateStep; 		 		 	  //��ת����Ӧ�ǶȲŽ��нǶ��л�
qreal GlobalMinMumSize;            		  //�ؼ���С����С��Χ
qreal GlobalParalMinMumSize;              //ƽ�пؼ���С����С��Χ

QList<ServiceProperty> GlobalServiceProperties;     //ȫ�ֵķ�����Ϣ
