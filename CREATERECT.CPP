#include "math.h"
#include "Command.h"
#include "CreateCmd.h"
#include "diagramscene.h"

#include <QGraphicsRectItem>
#include <QRectF>

CCreateRect::CCreateRect(DiagramScene* pDiagramScene)
	: m_LeftTop(0,0), m_RightBottom(0,0)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDiagramScene = pDiagramScene;
}

CCreateRect::~CCreateRect()
{
}

int CCreateRect::GetType()
{
	return ctCreateRectangle; 
}

int	CCreateRect::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	m_nStep ++; // 每次单击鼠标左键时操作步加 1 
	switch(m_nStep) // 根据操作步执行相应的操作
	{
		case 1: 
		{
			m_LeftTop = m_RightBottom = mouseEvent->scenePos();
			Prompt(QStringLiteral("请输入矩形的右下角点："));

			m_pRect = new QGraphicsRectItem();
			QRectF rect;
			rect.setBottomLeft(m_LeftTop);
			rect.setTopRight(m_RightBottom);
			m_pRect->setRect(rect);
			m_pDiagramScene->addItem(m_pRect);

			break;
		}
		case 2:
		{
			m_RightBottom = mouseEvent->scenePos();
			QRectF rect;
			rect.setBottomLeft(m_LeftTop);
			rect.setTopRight(m_RightBottom);
			m_pRect->setRect(rect);

			m_nStep = 0;  // 将操作步重置为 0
			Prompt(QStringLiteral("请输入矩形的左上角点："));
			break;
		}
		
	}
	return 0;
}

int	CCreateRect::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_RECT));

	switch(m_nStep)
	{
		case 0:
			Prompt(QStringLiteral("请输入矩形的左上角点："));
			break;
		case 1:
		{
			m_RightBottom = mouseEvent->scenePos();
			QRectF rect;
			rect.setBottomLeft(m_LeftTop);
			rect.setTopRight(m_RightBottom);
			m_pRect->setRect(rect);
			break;
		}
	}
	return 0;
}
// 单击鼠标右键取消当前的操作
int	CCreateRect::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("请输入矩形的左上角点："));
	return 0;
}
// 调用Cancel 函数取消本次操作
int CCreateRect::Cancel()
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作
	return 0 ;
}

