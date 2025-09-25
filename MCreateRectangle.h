#pragma once
#include "MCommand.h" 
#include <QPointF>
class MRectangle;
class MCreateRectangle :

	public MCommand
{
public:
    MCreateRectangle(QCADView* pDiagramScene);  // ���캯��
    ~MCreateRectangle();  // ��������

int GetType();  // ��ȡ��������
int OnLButtonDown(QMouseEvent* mouseEvent);  // �����������¼�
int OnMouseMove(QMouseEvent* mouseEvent);    // ��������ƶ��¼�
int OnRButtonDown(QMouseEvent* mouseEvent);  // �����Ҽ�����¼�
int Cancel();  // ȡ������

private:
    QPointF m_LeftTop;      // ���Ͻǵ�
    QPointF m_RightBottom;  // ���½ǵ�
    MRectangle* m_pRectangle;   // ָ��ǰ�����ָ��
};