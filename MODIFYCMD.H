/*	
 *	Header File of Class CModifyCmd
 */
#ifndef _ModifyCmd_h_
#define _ModifyCmd_h_

#include "COMMAND.H"
#include <QTransform>

class QPointF;
class QGraphicsLineItem;
class QGraphicsItem;

#ifdef __cplusplus

///////////////////////////////////////////////////////////////
/*	
 *	CMove
 */
class CMove : public CCommand
{
private:
	QPointF m_basePos;
	QPointF m_desPos;
	QPointF tmpPoint;
	QGraphicsLineItem* m_pTempLine;
	QGraphicsItem* m_pSelItem;

public:
	CMove(DiagramScene* pScene) ;
	~CMove() ;
	
	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;
///////////////////////////////////////////////////////////////
/*	
 *	CRotate
 */
class CRotate : public CCommand
{
private:
	QPointF m_basePos;
	QPointF m_desPos;

	QGraphicsLineItem* m_pTempLine;
	QGraphicsItem* m_pSelItem;

public:
	CRotate(DiagramScene* pScene) ;
	~CRotate() ;
	
	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;
///////////////////////////////////////////////////////////////
/*	
 *	CMirror
 */

class CMirror : public CCommand
{
private:
	QPointF m_basePos;
	QPointF m_desPos;
	QGraphicsLineItem* m_pTempLine;
	QGraphicsItem* m_pSelItem;
	QTransform oldTrans;

public:
	CMirror(DiagramScene* pScene) ;
	~CMirror() ;
	
	int		GetType();
	int		OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent) ;
	int		OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) ;

	int		Cancel() ;
} ;

///////////////////////////////////////////////////////////////
#endif // __cplusplus

#endif // _ModifyCmd_h_
