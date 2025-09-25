#ifndef _CreateCmd_h_
#define _CreateCmd_h_

#include <QVector>
#include "COMMAND.H"

#ifdef __cplusplus

class QGraphicsLineItem;
class QGraphicsEllipseItem;
class QGraphicsPathItem;
class QGraphicsRectItem;
class QGraphicsPolygonItem;
///////////////////////////////////////////////////////////////
/*	
 *	CCreateLine
 */
class CCreateLine : public CCommand
{
private:
	QPointF m_begin;	// 直线的起点
	QPointF m_end;		// 直线的终点 
	std::vector<QPointF> m_pPositions;
	QGraphicsLineItem* m_pLine;

public:
	CCreateLine(DiagramScene* pDiagramScene) ;
	~CCreateLine() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent);
	int		Cancel() ;
} ;

///////////////////////////////////////////////////////////////
/*	
 *	CCreateConnect
 */
class CCreateConnect : public CCommand
{
private:
	QPointF m_begin;	// 直线的起点
	QPointF m_end;		// 直线的终点 
	QPointF *m_pPositions;
	int		 m_bImport; //1, Im, -1, Out, 0 else
public:
	CCreateConnect() ;
	~CCreateConnect() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent);
	int		Cancel() ;
} ;

/*
 *	CCreatePolygon
 */
class CCreatePolygon : public CCommand
{
private:
	QPointF m_begin;	// 起点
	QPointF m_end;		// 临时终点 
	QVector<QPointF>    m_pPositions;
	QGraphicsPolygonItem* m_pPolygon;
	QVector<QGraphicsLineItem*> m_pTempLines;
	QGraphicsLineItem*  m_pTempLine;
public:
	CCreatePolygon(DiagramScene* pDiagramScene) ;
	~CCreatePolygon() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent);
	int		Cancel() ;
} ;

///////////////////////////////////////////////////////////////
/*	
 *	CCreateRect
 */
class CCreateRect : public CCommand
{
private:
	QPointF m_LeftTop;
	QPointF m_RightBottom;
	QGraphicsRectItem* m_pRect;
public:
	CCreateRect(DiagramScene* pDiagramScene) ;
	~CCreateRect() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;


///////////////////////////////////////////////////////////////
/*	
 *	CCreateText
 */
class CCreateText : public CCommand
{
private:
	QPointF m_LeftTop;
	QPointF m_RightBottom;
public:
	CCreateText() ;
	~CCreateText() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/*	
 *	CCreateBlock
 */
class CCreateBlock : public CCommand
{
private:
	QPointF m_LeftTop;
	QPointF m_RightBottom;
public:
	CCreateBlock() ;
	~CCreateBlock() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;
///////////////////////////////////////////////////////////////
/*	
 *	CCreateArc
 */
class CCreateCircle : public CCommand
{
private:
	QPointF	m_center ;
	QPointF	m_pos ;
	QGraphicsEllipseItem* m_pCircle;
	QGraphicsLineItem* m_pTempLine;
public:
	CCreateCircle(DiagramScene* pDiagramScene) ;
	~CCreateCircle() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;

///////////////////////////////////////////////////////////////
/*	
 *	CCreateEllipse
 */
class CCreateEllipse : public CCommand
{
private:
	QPointF	m_LeftTop ;
	QPointF	m_RightBottum ;
	QGraphicsEllipseItem* m_pEllipse;
public:
	CCreateEllipse(DiagramScene* pDiagramScene) ;
	~CCreateEllipse() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/*	
 *	CCreateArc
 */
class CCreateArc : public CCommand
{
private:
	QPointF m_center;
	QPointF m_begin;
	QPointF m_end ;
	QGraphicsPathItem* m_pArc;
	QGraphicsLineItem* m_pLineItem;

public:
	CCreateArc(DiagramScene* pDiagramScene) ;
	~CCreateArc() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;

///////////////////////////////////////////////////////////////
/*	
 *	CCreateFitCurve
 */
class CCreateFitCurve : public CCommand
{
private:
	QPointF m_begin;	// 直线的起点
	QPointF m_end;		// 直线的终点 
	QPointF *m_pPositions;

	int m_nCmd;
public:
	CCreateFitCurve(int nCmd) ;
	~CCreateFitCurve() ;

	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent);
	int		Cancel() ;
} ;

#endif // #ifdef __cplusplus

#endif