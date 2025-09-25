#pragma once
#include "MCommand.h"

class  QRubberBand;

class MSelectCmd :
    public MCommand
{
private:
	QPointF m_begin;	// Æðµã
	QPointF m_end;		// ÖÕµã 
	QRubberBand* rubberband;

public:
	MSelectCmd(QCADView* pDiagramScene);
	~MSelectCmd();

	int		GetType();
	int		OnLButtonDown(QMouseEvent* mouseEvent);
	int		OnMouseMove(QMouseEvent* mouseEvent);
	int		OnRButtonDown(QMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QMouseEvent* mouseEvent);

	int		Cancel();
};


