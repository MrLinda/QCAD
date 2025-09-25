#pragma once
#include "MCommand.h" 
#include <QPointF>
class MRectangle;
class MCreateRectangle :

	public MCommand
{
public:
    MCreateRectangle(QCADView* pDiagramScene);  // 构造函数
    ~MCreateRectangle();  // 析构函数

int GetType();  // 获取命令类型
int OnLButtonDown(QMouseEvent* mouseEvent);  // 处理左键点击事件
int OnMouseMove(QMouseEvent* mouseEvent);    // 处理鼠标移动事件
int OnRButtonDown(QMouseEvent* mouseEvent);  // 处理右键点击事件
int Cancel();  // 取消操作

private:
    QPointF m_LeftTop;      // 左上角点
    QPointF m_RightBottom;  // 右下角点
    MRectangle* m_pRectangle;   // 指向当前对象的指针
};