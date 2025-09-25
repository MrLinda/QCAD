#ifndef MCREATEELLIPSE_H
#define MCREATEELLIPSE_H

#include "MCommand.h" 
#include <QPointF>

class MEllipse;  // ǰ�������������������Ҫ��ͷ�ļ�

class MCreateEllipse : public MCommand
{
public:
    MCreateEllipse(QCADView* pDiagramScene);  // ���캯��
    ~MCreateEllipse();  // ��������

    int GetType();  // ��ȡ��������
    int OnLButtonDown(QMouseEvent* mouseEvent);  // �����������¼�
    int OnMouseMove(QMouseEvent* mouseEvent);    // ��������ƶ��¼�
    int OnRButtonDown(QMouseEvent* mouseEvent);  // �����Ҽ�����¼�
    int Cancel();  // ȡ������

private:
    QPointF m_LeftTop;      // ��Բ�����Ͻǵ�
    QPointF m_RightBottom;  // ��Բ�����½ǵ�
    MEllipse* m_pEllipse;   // ָ��ǰ��Բ�����ָ��
};

#endif // MCREATEELLIPSE_H