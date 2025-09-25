#pragma once
#include "MCommand.h"
class CCircle;  // entity������Բ����


class MCreateCircle : public MCommand
{
private:
    QPointF m_center;    // Բ��
    double m_radius;     // Բ�İ뾶
    CCircle* m_pCircle;  // ��ǰ������Բ����

public:
    // ���캯��
    MCreateCircle(QCADView* pDiagramScene);
    ~MCreateCircle();
    int GetType();
    int OnLButtonDown(QMouseEvent* mouseEvent);
    int	OnRButtonDown(QMouseEvent* mouseEvent);
    int OnMouseMove(QMouseEvent* mouseEvent);
    int Cancel();
};
#pragma once

