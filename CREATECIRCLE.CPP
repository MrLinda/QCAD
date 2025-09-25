#include "math.h"
#include "Command.h"
#include "CreateCmd.h"
#include "diagramscene.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QRectF>
#include <QLineF>

CCreateCircle::CCreateCircle(DiagramScene* pDiagramScene)
	: m_center(0,0), m_pos(0,0)
{
	m_nStep = 0; // 初始化操作步为 0	

	m_pDiagramScene = pDiagramScene;
}

CCreateCircle::~CCreateCircle()
{
}

int CCreateCircle::GetType()
{
	return ctCreateCircle; 
}

int	CCreateCircle::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	m_nStep ++; // 每次单击鼠标左键时操作步加 1 
	switch(m_nStep) // 根据操作步执行相应的操作
	{
		case 1: 
		{
			m_center = m_pos = mouseEvent->scenePos();
			
			QRectF rect(m_center.x(), m_center.y(), 0, 0);
			m_pCircle = new QGraphicsEllipseItem(rect);
			//m_pCircle->setPen();
			m_pDiagramScene->addItem(m_pCircle);

			//画临时线
			m_pTempLine = new QGraphicsLineItem();
			QLineF line(m_center, m_pos);
			m_pTempLine->setLine(line);
			m_pDiagramScene->addItem(m_pTempLine);

			Prompt(QStringLiteral("请输入圆周上的任意点："));
			break;
		}
		case 2:
		{
			m_pos = mouseEvent->scenePos();
			qreal r; //半径
			r = sqrt((m_center.x() - m_pos.x()) * (m_center.x() - m_pos.x()) + (m_center.y() - m_pos.y()) * (m_center.y() - m_pos.y()));
			QPointF leftBottom(m_center.x() - r, m_center.y() - r);
			QPointF rightTop(m_center.x() + r, m_center.y() + r);
			QRectF rect(leftBottom, rightTop);
			m_pCircle->setRect(rect);

			m_pDiagramScene->removeItem(m_pTempLine);
			delete m_pTempLine;

			m_nStep = 0;  // 将操作步重置为 0
			Prompt(QStringLiteral("请输入圆周上的任意点："));
			break;
		}
		
	}
	return 0;
}

int	CCreateCircle::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_CIRCLE));

	switch(m_nStep)
	{
		case 0:
			Prompt(QStringLiteral("请输入圆的中心点：")) ;
			break;
		case 1:
		{
			m_pos = mouseEvent->scenePos();
			qreal r; //半径
			r = sqrt((m_center.x() - m_pos.x()) * (m_center.x() - m_pos.x()) + (m_center.y() - m_pos.y()) * (m_center.y() - m_pos.y()));
			QPointF leftBottom(m_center.x() - r, m_center.y() - r);
			QPointF rightTop(m_center.x() + r, m_center.y() + r);
			QRectF rect(leftBottom, rightTop);
			m_pCircle->setRect(rect);

			//画临时线
			QLineF line(m_center, m_pos);
			m_pTempLine->setLine(line);

			break;
		}
	}
	return 0;
}
// 单击鼠标右键取消当前的操作
int	CCreateCircle::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}
	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("请输入圆的中心点：")) ;
	
	return 0;
}
// 调用Cancel 函数取消本次操作
int CCreateCircle::Cancel()
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}
	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作

	return 0 ;
}

