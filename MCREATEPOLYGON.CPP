#include "math.h"
#include "MCreatePolygon.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H" 
#include <QPolygonF>
#include <QLineF>
#include <QMouseEvent>

MCreatePolygon::MCreatePolygon(QCADView* pDiagramScene)
	: m_begin(0,0), m_end(0,0)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDC = pDiagramScene;
}

MCreatePolygon::~MCreatePolygon()
{

}

int MCreatePolygon::GetType()
{
	return ctCreatePolygon; 
}

int	MCreatePolygon::OnLButtonDown(QMouseEvent* mouseEvent)
{
	m_nStep ++; // 每次单击鼠标左键时操作步加 1 

	switch(m_nStep) // 根据操作步执行相应的操作
	{
		case 1: 
		{
			m_begin = m_end = m_pDC->ScreentoWorld(mouseEvent->pos());

			m_pTempLine = new MLine(m_begin, m_end);
			m_pDC->addEntity(m_pTempLine);

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
			m_pTempLines.push_back(m_pTempLine);

			//绘制临时线
			m_end = m_pDC->ScreentoWorld(mouseEvent->pos());

			Prompt(QStringLiteral("请输入多边形的下一点："));
			m_begin = m_end;
			//下一条临时线
			m_pTempLine = new MLine(m_begin, m_end);
			m_pDC->addEntity(m_pTempLine);

			break;
		}		
	}

	return 0;
}

//鼠标左键双击创建多线
int MCreatePolygon::OnLButtonDblClk(QMouseEvent* mouseEvent)
{
	//清楚临时线
	m_pDC->removeEntity(m_pTempLine);
	delete m_pTempLine;

	QPointF lastPoint;
	foreach (MLine* pItem, m_pTempLines)
	{
		m_pPositions.append(pItem->GetBeginPos());
		lastPoint = pItem->GetEndPos();
		m_pDC->removeEntity(pItem);

		delete pItem;
	}
	m_pTempLines.clear();
	m_pPositions.append(lastPoint);

	if (m_pPositions.size() < 3)
	{
		Cancel();
		m_pDC->update();
		m_pPositions.clear();

		return 0;
	}

	QPen pen(m_pDC->penStyle());
	pen.setColor(m_pDC->lineColor());
	pen.setWidthF(m_pDC->penWidth());
	QBrush brush(m_pDC->brushColor());

	//更新多边形
	m_pPolygon = new MPolygon(m_pPositions);
	m_pPolygon->SetPen(pen);
	m_pPolygon->SetBrush(brush);
	m_pDC->addEntity(m_pPolygon);
	m_pDC->update();
	m_pPositions.clear();

	m_nStep = 0;  // 将操作步重置为 0
	Prompt(QStringLiteral("请输入多边形的起点："));
	
	return 0;
}

int	MCreatePolygon::OnMouseMove(QMouseEvent* mouseEvent)
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
			m_end = m_pDC->ScreentoWorld(mouseEvent->pos());
			
			m_pTempLine->SetEndPos(m_end);
			m_pDC->update();

			break;
		}
	}
	return 0;
}

// 单击鼠标右键取消当前的操作
int	MCreatePolygon::OnRButtonDown(QMouseEvent* mouseEvent)
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
int MCreatePolygon::Cancel()
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作
	return 0 ;
}

