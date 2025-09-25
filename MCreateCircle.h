#pragma once
#include "MCommand.h"
class CCircle;  // entity里声明圆的类


class MCreateCircle : public MCommand
{
private:
    QPointF m_center;    // 圆心
    double m_radius;     // 圆的半径
    CCircle* m_pCircle;  // 当前创建的圆对象

public:
    // 构造函数
    MCreateCircle(QCADView* pDiagramScene);
    ~MCreateCircle();
    int GetType();
    int OnLButtonDown(QMouseEvent* mouseEvent);
    int	OnRButtonDown(QMouseEvent* mouseEvent);
    int OnMouseMove(QMouseEvent* mouseEvent);
    int Cancel();
};
#pragma once

