#pragma once
#include "MCommand.h"

class CArc;  // 声明弧的类
class MLine;

class MCreateArc : public MCommand
{
private:
    QPointF m_center;    // 圆弧的中心点
    QPointF m_begin;     // 圆弧的起点
    QPointF m_end;       // 圆弧的终点
    CArc* m_pArc;        // 当前创建的弧对象
    MLine* m_pLine1; //第一条辅助线
    MLine* m_pLine2; //第二条

public:
    // 构造函数和析构函数
    MCreateArc(QCADView* pDiagramScene);
    ~MCreateArc();


    int GetType();
    int OnLButtonDown(QMouseEvent* mouseEvent);
    int OnRButtonDown(QMouseEvent* mouseEvent);
    int OnMouseMove(QMouseEvent* mouseEvent);
    int Cancel();
};
#pragma once
