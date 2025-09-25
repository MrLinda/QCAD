#include "math.h"

#include "Command.h"
#include "CreateCmd.h"

#include "diagramscene.h"

#include <QPainter>
#include <QGraphicsPathItem>
#include <QRectF>
#include <QGraphicsLineItem>
#include <QLineF>
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/*	
 *	CLASS	CCreateArc
 */
CCreateArc::CCreateArc(DiagramScene* pDiagramScene)
	: m_center(0,0), m_begin(0,0), m_end(0,0)
{
	m_nStep = 0; // 初始化操作步为 0	
	m_pDiagramScene = pDiagramScene;
}

CCreateArc::~CCreateArc()
{
}

int CCreateArc::GetType()
{
	return ctCreateArc;
}

int	CCreateArc::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	m_nStep ++; // 每次单击鼠标左键时操作步加 1 

	QPainterPath path;
	switch(m_nStep)
	{
		case 1:
		{
			m_center = mouseEvent->scenePos();
			Prompt(QStringLiteral("请输入圆弧的起始点："));

			path.arcMoveTo(m_center.x(), m_center.y(),0,0,0);
			path.arcTo(m_center.x(), m_center.y(), 0, 0, 0,0);
			m_pArc = new QGraphicsPathItem;
			m_pArc->setPath(path);
			//pArc->setPen();
			m_pDiagramScene->addItem(m_pArc);

			//辅助线
			QLineF line(m_center, m_center);
			m_pLineItem = new QGraphicsLineItem();
			m_pLineItem->setLine(line);
			m_pDiagramScene->addItem(m_pLineItem);

			break;
		}
		case 2:
		{
			m_begin = mouseEvent->scenePos();
			Prompt(QStringLiteral("请输入圆弧的终点："));
			break;
		}
		case 3:
		{
			m_end = mouseEvent->scenePos();
			//qreal r; //半径
			////r = sqrt((m_center.x() - m_end.x()) * (m_center.x() - m_end.x()) + (m_center.y() - m_end.y()) * (m_center.y() - m_end.y()));
			//qreal delX = m_begin.x() - m_center.x();
			//qreal delY = m_begin.y() - m_center.y();
			//r = sqrt(delX * delX + delY * delY);
			//qreal startAng = acos(delX / r);
			//delX = m_end.x() - m_center.x();
			//delY = m_end.y() - m_center.y();
			//r = sqrt(delX * delX + delY * delY);
			//qreal endAng = acos(delX / r);
			//path.arcMoveTo(m_center.x(), m_center.y(), 2*r, 2*r, startAng*57);
			//path.arcTo(m_center.x(), m_center.y(), 2*r, 2*r, startAng, endAng*57);
			////QGraphicsPathItem* pArc = new QGraphicsPathItem;
			//m_pArc->setPath(path);

			m_pDiagramScene->removeItem(m_pLineItem);
			delete m_pLineItem;

			m_nStep = 0; 
			Prompt(QStringLiteral("请输入圆弧的中心点："));
			break;
		}
	}
	return 0;
}

int	CCreateArc::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_ARC));

	switch(m_nStep)
	{
		case 0:
			Prompt(QStringLiteral("请输入圆的中心点："));
			break;
		case 1:
		{
			QLineF line(m_center, mouseEvent->scenePos());
			m_pLineItem->setLine(line);
			Prompt(QStringLiteral("请输入圆弧的起点："));
			break;
		}
		case 2:
		{
			Prompt(QStringLiteral("请输入圆弧的终点："));
			m_end = mouseEvent->scenePos();
			qreal r; //半径
			//r = sqrt((m_center.x() - m_end.x()) * (m_center.x() - m_end.x()) + (m_center.y() - m_end.y()) * (m_center.y() - m_end.y()));
			qreal delX = m_begin.x() - m_center.x();
			qreal delY = m_begin.y() - m_center.y();
			r = sqrt(delX * delX + delY * delY);
			QPointF leftBottom(m_center.x() - r, m_center.y() - r);
			QPointF rightTop(m_center.x() + r, m_center.y() + r);
			QRectF rect(leftBottom, rightTop);

			qreal startAng = acos(delX / r);
			delX = m_end.x() - m_center.x();
			delY = m_end.y() - m_center.y();
			r = sqrt(delX * delX + delY * delY);
			qreal endAng = acos(delX / r);

			QPainterPath path;
			path.arcMoveTo(rect, startAng*180/3.14);
			path.arcTo(rect, startAng* 180 / 3.14, (endAng-startAng)* 180 / 3.14);
			//QGraphicsPathItem* pArc = new QGraphicsPathItem;
			m_pArc->setPath(path);
			break;
		}
	}
	return 0;
}

int	CCreateArc::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	// 如果当前的操作步为 2 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 2){ 

	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("请输入圆弧的中心点："));

	return 0;
}

// 调用Cancel 函数取消本次操作
int CCreateArc::Cancel()
{
	// 如果当前的操作步为 2 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 2){ 
		;
	}
	
	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作
	
	return 0;
}
