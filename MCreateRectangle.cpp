#include "MCreateRectangle.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"  // 包含 MRectangle 的定义

#include <QMouseEvent>

MCreateRectangle::MCreateRectangle(QCADView* pDiagramScene)
	: m_LeftTop(0, 0), m_RightBottom(0, 0), m_pRectangle(nullptr)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDC = pDiagramScene;

	//rubberBand = NULL;
}

MCreateRectangle::~MCreateRectangle()
{
    delete m_pRectangle;
    /*// 确保析构时释放未完成的矩形对象
    if (m_pRectangle != nullptr) {
        delete m_pRectangle;
        m_pRectangle = nullptr;
    }*/
}

int MCreateRectangle::GetType()
{
    return ctCreateRectangle; // 自定义的类型，用于标识绘制矩形
}

int MCreateRectangle::OnLButtonDown(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return 0;

    // 检查 m_pDC 是否已初始化，防止空指针
    if (m_pDC == nullptr) 
    {
        return 0;  // 如果绘图上下文为空，提前返回

    }
    QPen pen(m_pDC->penStyle());
    pen.setColor(m_pDC->lineColor());
    pen.setWidthF(m_pDC->penWidth());

    QBrush brush(m_pDC->brushColor());

    m_nStep++; // 每次单击鼠标左键时操作步加 1 

    switch (m_nStep)
    {
    case 1:
    {
        // 第一次单击，记录矩形的左上角点
        m_LeftTop = m_pDC->ScreentoWorld(mouseEvent->pos());
        m_RightBottom = m_LeftTop; // 初始时右下角点与左上角点相同

        //QBrush brush(m_pDC->brushColor());

        // 创建矩形实体
        m_pRectangle = new MRectangle(m_LeftTop, m_RightBottom);
        m_pRectangle->SetPen(pen);
        m_pRectangle->SetBrush(brush);
        //QBrush brush(m_pDC->brushColor());  // 使用 QCADView 的 brushColor 初始化画刷
        m_pRectangle->SetAttrib(&pen, &brush); // 设置矩形的属性
        m_pDC->addEntity(m_pRectangle); // 将矩形添加到场景

        QString sPrmt = QStringLiteral("请输入矩形的右下角点：");
        Prompt(sPrmt);
        break;
    }
    case 2:
    {
        if (m_pRectangle == nullptr) {
            // 防止空指针访问
            return 0;
        }
        // 第二次单击，确定右下角点，完成椭圆的绘制
        m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

        // 提示绘制完成
        Prompt(QStringLiteral("矩形绘制完成"));

        m_nStep = 0; // 完成矩形绘制后重置步骤
        break;
    }
    default:
        break;
    }

    return 0;
}

int MCreateRectangle::OnMouseMove(QMouseEvent* mouseEvent)
{
    MCommand::OnMouseMove(mouseEvent);

    if (m_nStep == 1 && m_pRectangle != nullptr)
    {
        // 当用户移动鼠标时，更新右下角点
        m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

        // 动态更新矩形的形状
        m_pRectangle->Move(m_LeftTop, m_RightBottom);
        m_pDC->update();
    }

    return 0;
}

int MCreateRectangle::OnRButtonDown(QMouseEvent* mouseEvent)
{
    // 右键点击取消当前操作
    Cancel();
    return 0;
}

int MCreateRectangle::Cancel()
{
    if (m_nStep == 1 && m_pRectangle != nullptr) {
        // 在取消时，可以删除未完成的矩形
        //m_pDC->removeEntity(m_pRectangle);
        m_pRectangle = nullptr;  // 避免再次访问空指针
    }

    m_nStep = 0;
    Prompt(QStringLiteral("请输入矩形的左上角点...")); // 提示新的操作
    return 0;
}