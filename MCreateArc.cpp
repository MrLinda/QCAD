#include "MCreateArc.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"
#include <cmath>
#include "qevent.h"


MCreateArc::MCreateArc(QCADView* pDiagramScene)
    : m_center(0, 0), m_begin(0, 0), m_end(0, 0)
{
    m_nStep = 0;  // 初始化操作步为 0
    m_pDC = pDiagramScene;

    m_pLine1 = NULL;
    m_pLine2 = NULL;
}

MCreateArc::~MCreateArc()
{
}

int MCreateArc::GetType()
{
    return ctCreateArc;  // 返回创建弧的操作类型
}

int MCreateArc::OnLButtonDown(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return 0;

    QPen pen(m_pDC->penStyle());
    pen.setColor(m_pDC->lineColor());
    pen.setWidthF(m_pDC->penWidth());

    m_nStep++;  // 每次单击鼠标左键时操作步加 1

    switch (m_nStep)  // 根据操作步执行相应的操作
    {
    case 1:
    {
        m_center = m_pDC->ScreentoWorld(mouseEvent->pos());  // 记录弧的中心
        QString sPrmt = QStringLiteral("请输入弧的起点：");
        Prompt(sPrmt);

        //绘制第一条辅助线
        m_pLine1 = new MLine(m_center, m_center);
        m_pLine1->SetPen(pen);
        m_pDC->addEntity(m_pLine1);
        break;
    }
    case 2:
    {
        m_begin = m_pDC->ScreentoWorld(mouseEvent->pos());  // 记录弧的起点
        QString sPrmt = QStringLiteral("请输入弧的终点：");
        Prompt(sPrmt);
        m_end = m_pDC->ScreentoWorld(mouseEvent->pos());  // 记录弧的终点
        m_pArc = new CArc(m_center, m_begin, m_end);  // 创建 MArc 实体
        m_pArc->SetPen(pen);
        m_pDC->addEntity(m_pArc);  // 添加到视图中
        //绘制第二条辅助线
        m_pLine2 = new MLine(m_center, m_end);
        m_pLine2->SetPen(pen);
        m_pDC->addEntity(m_pLine2);
        break;
    }
    case 3:
    {
        QString sPrmt = QStringLiteral("请输入弧的圆心点：");
        Prompt(sPrmt);

        //删除辅助线
        m_pDC->removeEntity(m_pLine1);
        m_pDC->removeEntity(m_pLine2);
        delete m_pLine1;
        delete m_pLine2;
        m_pDC->update();

        m_nStep = 0;  // 重置操作步骤
        break;
    }
    default:
        break;
    }

    return 0;
}

int MCreateArc::OnMouseMove(QMouseEvent* mouseEvent)
{
    if (m_nStep == 1)
    {
        m_end = m_pDC->ScreentoWorld(mouseEvent->pos());
        m_pLine1->SetEndPos(m_end);
        m_pDC->update();
    }
    if (m_nStep == 2 && m_pArc != nullptr)
    {
        m_end = m_pDC->ScreentoWorld(mouseEvent->pos());  // 记录弧的终点
        m_pArc->SetEndPos(m_end);

        m_pLine2->SetEndPos(m_end);
        m_pDC->update();
    }

    return 0;
}

int MCreateArc::OnRButtonDown(QMouseEvent* mouseEvent)
{
    Cancel();
    return 0;
}

int MCreateArc::Cancel()
{
    m_nStep = 0;  // 重置操作步骤
    if (m_pLine1) {
        delete m_pLine1;
        m_pLine1 = NULL;
    }
    if (m_pLine2) {
        delete m_pLine2;
        m_pLine2 = NULL;
    }
    Prompt(QStringLiteral("请输入弧的中心点："));  // 提示重新输入
    return 0;
}
