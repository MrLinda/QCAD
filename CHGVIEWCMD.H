#ifndef _ChgViewCmd_h_
#define _ChgViewCmd_h_
#include "COMMAND.H"

class QGraphicsView;
class QGraphicsRectItem;

#ifdef __cplusplus
////////////////
///CZoomPanCmd ¿‡

class CZoomPanCmd : public CCommand
{
private:
	QPointF m_basePos;
	QPointF m_desPos;
	QGraphicsView* m_pView;

public:
	CZoomPanCmd(DiagramScene* pScene) ;
	~CZoomPanCmd() ;
	
	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;
///////////////////////////////////////////////////////////////
/*	
 *	CZoomWindowCmd
 */
class CZoomWindowCmd : public CCommand
{
private:
	QPointF m_basePos;
	QPointF m_desPos;
	QGraphicsView* m_pView;
	QGraphicsRectItem* m_pRect;

public:
	CZoomWindowCmd(DiagramScene* pScene) ;
	~CZoomWindowCmd() ;
	
	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
#endif // __cplusplus

#endif // _ModifyCmd_h_
