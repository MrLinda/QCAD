#include "math.h"
#include "Command.h"
#include "CreateCmd.h"
#include "diagramscene.h"

#include <QGraphicsEllipseItem>
#include <QRectF>

CCreateEllipse::CCreateEllipse(DiagramScene* pDiagramScene)
	: m_LeftTop(0,0), m_RightBottum(0,0)
{
	m_nStep = 0; // 初始化操作步为 0	
	m_pDiagramScene = pDiagramScene;
}

CCreateEllipse::~CCreateEllipse()
{
}

int CCreateEllipse::GetType()
{
	return ctCreateEllipse; 
}

int	CCreateEllipse::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	m_nStep ++; // 每次单击鼠标左键时操作步加 1 
	switch(m_nStep) // 根据操作步执行相应的操作
	{
		case 1: 
		{
			m_LeftTop = m_RightBottum = mouseEvent->scenePos();
			Prompt(QStringLiteral("请输入椭圆的右下点："));
			m_pEllipse = new QGraphicsEllipseItem();
			QRectF rect;
			rect.setBottomLeft(m_LeftTop);
			rect.setTopRight(m_RightBottum);
			m_pEllipse->setRect(rect);
			m_pDiagramScene->addItem(m_pEllipse);

			break;
		}
		case 2:
		{
			m_RightBottum = mouseEvent->scenePos();
			QRectF rect;
			rect.setBottomLeft(m_LeftTop);
			rect.setTopRight(m_RightBottum);
			m_pEllipse->setRect(rect);

			m_nStep = 0;  // 将操作步重置为 0
			Prompt(QStringLiteral("请输入椭圆的左上点："));
			break;
		}
		
	}
	return 0;
}

int	CCreateEllipse::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_ELLIPSE));

	switch(m_nStep)
	{
		case 0:
			Prompt(QStringLiteral("请输入椭圆的左上点："));
			break;
		case 1:
		{
			m_RightBottum = mouseEvent->scenePos();
			QRectF rect;
			rect.setBottomLeft(m_LeftTop);
			rect.setTopRight(m_RightBottum);
			m_pEllipse->setRect(rect);
			break;
		}
	}
	return 0;
}
// 单击鼠标右键取消当前的操作
int	CCreateEllipse::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("请输入椭圆的左上点："));
	return 0;
}
// 调用Cancel 函数取消本次操作
int CCreateEllipse::Cancel()
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作
	return 0 ;
}

