#include "MCreateCircle.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"
#include <QMouseEvent>


MCreateCircle::MCreateCircle(QCADView* pDiagramScene)
    : m_center(0, 0), m_radius(0)
{
    m_nStep = 0;  // 初始化操作步为 0
    m_pDC = pDiagramScene;
}
MCreateCircle::~MCreateCircle()
{
}

int MCreateCircle::GetType()
{
    return ctCreateCircle;  
}
//根据鼠标操作开启绘制
int MCreateCircle::OnLButtonDown(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return 0;

    QPen pen(m_pDC->penStyle());
    pen.setColor(m_pDC->lineColor());
    pen.setWidthF(m_pDC->penWidth());
    QBrush brush = m_pDC->brushColor();

    m_nStep++;  // 每次单击鼠标左键时操作步加 1

    switch (m_nStep)  // 根据操作步执行相应的操作
    {
    case 1:
    {
        m_center = m_pDC->ScreentoWorld(mouseEvent->pos());  // 记录圆心位置
        QString sPrmt = QStringLiteral("请输入圆周上的任意点：");
        Prompt(sPrmt);

        m_radius = 0; // 计算半径
        m_pCircle = new CCircle(m_center, m_radius);  // 创建 MCircle 实体
        m_pCircle->SetPen(pen);
        m_pCircle->SetBrush(brush);
        m_pDC->addEntity(m_pCircle);  // 添加到视图中

        break;
    }
    case 2:
    {
        QString sPrmt = QStringLiteral("请输入圆心点：");
        Prompt(sPrmt);

        m_nStep = 0;  // 重置操作步骤
        break;
    }
    default:
        break;
    }

    return 0;
}

int MCreateCircle::OnMouseMove(QMouseEvent* mouseEvent)
{
    MCommand::OnMouseMove(mouseEvent);

    if (m_nStep == 1 && m_pCircle != nullptr)
    {
        // 当用户移动鼠标时，更新右下角点
        QPointF curPos = m_pDC->ScreentoWorld(mouseEvent->pos());
        QPointF dp = curPos - m_center;
        double r_del = sqrt(dp.x()*dp.x() + dp.y()*dp.y());
        m_pCircle->SetRadius(r_del);
        m_pDC->update();
    }
    else
    {
        QString sPrmt = QStringLiteral("请输入圆心点：");
        Prompt(sPrmt);
    }

    return 0;
}

int	MCreateCircle::OnRButtonDown(QMouseEvent* mouseEvent) {
    Cancel();
    return 0;
}

int MCreateCircle::Cancel()
{
    m_nStep = 0;  // 重置操作步骤
    Prompt(QStringLiteral("请输入圆心："));  // 提示重新输入
    return 0;
}
