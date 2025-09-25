#include "math.h"
#include "Command.h"
#include "CreateCmd.h"
#include "diagramscene.h"

#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QLineF>

CCreatePolygon::CCreatePolygon(DiagramScene* pDiagramScene)
	: m_begin(0,0), m_end(0,0)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDiagramScene = pDiagramScene;
}

CCreatePolygon::~CCreatePolygon()
{

}

int CCreatePolygon::GetType()
{
	return ctCreatePolygon; 
}

int	CCreatePolygon::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	m_nStep ++; // 每次单击鼠标左键时操作步加 1 

	switch(m_nStep) // 根据操作步执行相应的操作
	{
		case 1: 
		{
			m_begin = m_end = mouseEvent->scenePos();

			QVector<QPointF> vPoints;
			vPoints.push_back(m_begin);
			vPoints.push_back(m_begin);
			vPoints.push_back(m_begin);
			QPolygonF polygon(vPoints);
			m_pPolygon = new QGraphicsPolygonItem();
			m_pPolygon->setPolygon(polygon);
			m_pDiagramScene->addItem(m_pPolygon);

			QLineF line(m_begin, m_end);
			m_pTempLine = new QGraphicsLineItem();
			m_pTempLine->setLine(line);
			m_pDiagramScene->addItem(m_pTempLine);

			Prompt(QStringLiteral("请输入多边形的下一点："));
			break;
		}
		case 2:
		//{
		//	//绘制
		//	break;
		//}
		default:
		{
			//绘制临时线
			m_end = mouseEvent->scenePos();
			m_pTempLines.push_back(m_pTempLine);
			m_pDiagramScene->addItem(m_pTempLine);

			QLineF line(m_begin, m_end);
			m_pTempLine->setLine(line);

			Prompt(QStringLiteral("请输入多边形的下一点："));
			m_begin = m_end;
			//下一条临时线
			QLineF line2(m_begin, m_end);
			m_pTempLine = new QGraphicsLineItem();
			m_pTempLine->setLine(line2);
			m_pDiagramScene->addItem(m_pTempLine);
			break;
		}		
	}

	return 0;
}

//鼠标左键双击创建多线
int CCreatePolygon::OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent)
{
	//清楚临时线
	m_pDiagramScene->removeItem(m_pTempLine);
	delete m_pTempLine;
	QPolygonF polygon;
	QPointF lastPoint;
	foreach (QGraphicsLineItem* pItem, m_pTempLines)
	{
		polygon.append(pItem->line().p1());
		lastPoint = pItem->line().p2();
		m_pDiagramScene->removeItem(pItem);
		delete pItem;
	}
	m_pTempLines.clear();
	polygon.append(lastPoint);

	//更新多边形
	m_pPolygon->setPolygon(polygon);
	
	m_nStep = 0;  // 将操作步重置为 0
	Prompt(QStringLiteral("请输入多边形的起点："));
	
	return 0;
}

int	CCreatePolygon::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_POLYGON));

	switch(m_nStep)
	{
		case 0:
		{
			Prompt(QStringLiteral("请输入多边形的起点："));
			break;
		}
		default:
		{
			//绘制临时线
			m_end = mouseEvent->scenePos();
			QLineF line(m_begin, m_end);
			m_pTempLine->setLine(line);

			break;
		}
	}
	return 0;
}

// 单击鼠标右键取消当前的操作
int	CCreatePolygon::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent)
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("请输入多边形的起点："));
	return 0;
}
// 调用Cancel 函数取消本次操作
int CCreatePolygon::Cancel()
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作
	return 0 ;
}

