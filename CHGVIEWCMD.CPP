#include "math.h"
#include "ChgViewCmd.h"
#include "diagramscene.h"

#include <QGraphicsView>
#include <QTransform>
#include <QGraphicsRectItem>
#include <QRectF>
///////////////////////////////////////////////////////////////
/*	
 *	CLASS	CZoomPanCmd
 */
CZoomPanCmd::CZoomPanCmd(DiagramScene* pScene)
{
	m_nStep = 0; // 初始化操作步为 0

	m_pDiagramScene = pScene;
	QList<QGraphicsView*> lstViews = m_pDiagramScene->views();
	m_pView = *lstViews.begin();
}

CZoomPanCmd::~CZoomPanCmd()
{
}

int	CZoomPanCmd::GetType()
{
	return ctZoomPan;
}

int	CZoomPanCmd::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	m_nStep ++ ;

	switch(m_nStep)
	{
		case 1:
		{
			m_basePos = m_desPos = mouseEvent->scenePos();
			Prompt(QStringLiteral("请输入平移画面的目标点："));
			break;
		}
		case 2:
		{


			//	设置步骤为0
			m_nStep = 0;
			Prompt(QStringLiteral("请输入平移画面的起点："));
			break;
		}
		default:
			break;
	}
	return 0;
}

int	CZoomPanCmd::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	m_desPos = mouseEvent->scenePos();
	qreal dX = m_desPos.x() - m_basePos.x();
	qreal dY = m_desPos.y() - m_basePos.y();

	switch(m_nStep)
	{
		case 0:
		{
			Prompt(QStringLiteral("请输入平移画面的起点："));
			break;
		}
		case 1:
		{
			m_desPos = mouseEvent->scenePos();
			qreal dX = m_desPos.x() - m_basePos.x();
			qreal dY = m_desPos.y() - m_basePos.y();
			QTransform trans(1, 0, 0, 0, 1, 0, dX, dY, 1);
			//m_pView->setTransform(trans);
			QRectF rect = m_pView->sceneRect();
			QPointF leftB(rect.bottomLeft().x() - dX, rect.bottomLeft().y() - dY);
			QPointF rightT(rect.topRight().x() - dX, rect.topRight().y() - dY);
			QRectF rectNew(leftB, rightT);
			m_pView->setSceneRect(rectNew);

			//QTransform trans(1,0,0,0,1,0,dX,dY,1);
			//m_pView->setTransform(trans);
			break;
		}
		default:
			break;
	}
	return 0;
}

// Click right button to cancel the operation
int	CZoomPanCmd::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	m_nStep = 0;
	return 0;
}

int CZoomPanCmd::Cancel()
{
	return 0 ;
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/*	
 *	CLASS	CZoomWindowCmd
 */
CZoomWindowCmd::CZoomWindowCmd(DiagramScene* pScene)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDiagramScene = pScene;

	QList<QGraphicsView*> lstViews = m_pDiagramScene->views();
	m_pView = *lstViews.begin();
}

CZoomWindowCmd::~CZoomWindowCmd()
{
}

int CZoomWindowCmd::GetType()
{
	return ctZoomWindow;
}

int	CZoomWindowCmd::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	m_nStep ++;
	switch(m_nStep)
	{
		case 1:
		{
			m_basePos = m_desPos = mouseEvent->scenePos();
			m_pRect = new QGraphicsRectItem();
			QRectF rect(m_basePos, m_desPos);
			m_pRect->setRect(rect);
			m_pDiagramScene->addItem(m_pRect);

			Prompt(QStringLiteral("请输入缩放矩形的右下角点：")) ;
			break;
		}
		case 2:
		{
			m_desPos = mouseEvent->scenePos();
			QRectF rectNew(m_basePos, m_desPos);
			QRectF rectOld = m_pView->sceneRect();
			m_pView->setSceneRect(rectNew);
			qreal dx = m_pView->viewport()->geometry().width() / m_pView->scene()->width();
			qreal dy = m_pView->viewport()->geometry().height() / m_pView->scene()->height();
			qreal scale = dx < dy ? dx : dy;
			m_pView->scale(scale, scale);

			m_pDiagramScene->removeItem(m_pRect);
			delete m_pRect;

			m_nStep = 0;
			Prompt(QStringLiteral("请输入缩放矩形的左上角点："));
			break;
		}
		default:
			m_nStep = 0;
			break;		
	}
	return 0;
}

int	CZoomWindowCmd::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	switch(m_nStep)
	{
	case 0:
	{
		Prompt(QStringLiteral("请输入缩放矩形的左上角点："));
		break;
	}
		case 1:
		{
			m_desPos = mouseEvent->scenePos();
			QRectF rect(m_basePos, m_desPos);
			m_pRect->setRect(rect);

			break;
		}
	}
	return 0;
}

int	CZoomWindowCmd::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{

	//::ClipCursor(NULL);

	m_nStep = 0;
	return 0;
}

int CZoomWindowCmd::Cancel()
{
	return 0 ;
}