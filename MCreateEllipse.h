#ifndef MCREATEELLIPSE_H
#define MCREATEELLIPSE_H

#include "MCommand.h" 
#include <QPointF>

class MEllipse;  // 前向声明，避免包含不必要的头文件

class MCreateEllipse : public MCommand
{
public:
    MCreateEllipse(QCADView* pDiagramScene);  // 构造函数
    ~MCreateEllipse();  // 析构函数

    int GetType();  // 获取命令类型
    int OnLButtonDown(QMouseEvent* mouseEvent);  // 处理左键点击事件
    int OnMouseMove(QMouseEvent* mouseEvent);    // 处理鼠标移动事件
    int OnRButtonDown(QMouseEvent* mouseEvent);  // 处理右键点击事件
    int Cancel();  // 取消操作

private:
    QPointF m_LeftTop;      // 椭圆的左上角点
    QPointF m_RightBottom;  // 椭圆的右下角点
    MEllipse* m_pEllipse;   // 指向当前椭圆对象的指针
};

#endif // MCREATEELLIPSE_H