#pragma once

#include "MCommand.h"
#include "ENTITY.H"

#include <QString>

class QRubberBand;

class MMoveCmd :
	public MCommand
{
private:
	QPointF m_begin;	// Æðµã
	QPointF m_end;		// ÖÕµã
	MLine* m_pTmpLine;
	QString m_promptPrefix;

public:
	MMoveCmd(QCADView* pDiagramScene);
	~MMoveCmd();

	int		GetType();
	int		OnLButtonDown(QMouseEvent* mouseEvent);
	int		OnMouseMove(QMouseEvent* mouseEvent);
	int		OnRButtonDown(QMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QMouseEvent* mouseEvent);

	int		Cancel();
};