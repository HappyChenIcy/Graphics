/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: ItemHeader.h
**��  ��: wey       Version: 1.0       Date: 2016.09.01
**��  ��: ��ͼģ��ͷ�ļ�
**Others:
**
**�޸���ʷ:
**20160918:wey:���DragDirectö������
*************************************************/
#ifndef ITEMHEADER_H
#define ITEMHEADER_H

enum PointType
{
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT
};

enum MouseType
{
    MOUSE_NONE,
    MOUSE_PRESS,
    MOUSE_MOVE,
    MOUSE_RELEASE
};

//�߶����������ӵ�����
enum LineType
{
    LINE_NONE,
    LINE_MYITEM,
    LINE_NODEPORT
};

#define PI 3.141592653
#define ROTATE_WIDTH   5
#define ROTATE_SQUARE_WIDTH 5
#define ALLOW_DROP_RANGE 10             //�ؼ������������µķ�Χ
#define MOVE_LIMIT_RANG  5              //�˿��ƶ����յ����������ⷽ���ƶ�����ô��Ҫ�ж�����ڴ˷����ϵ�ֵ���ٴ��ڴ˺�


#endif // HEADER_H
