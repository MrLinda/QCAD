#pragma once

#include "MCommand.h"

class MText;
class  QRubberBand;

class MCreateText :
	public MCommand
{
private:
	QPointF m_leftTop;
	QPointF m_rightBottom;
	QPointF m_tempPos;
	MText* m_pText;
	QString m_promptPrefix;
	QRubberBand* rubberband;

public:
	MCreateText(QCADView* pDiagramScene);
	~MCreateText();

	int		GetType();
	int		OnLButtonDown(QMouseEvent* mouseEvent);
	int		OnMouseMove(QMouseEvent* mouseEvent);
	int		OnRButtonDown(QMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QMouseEvent* mouseEvent);

	int		Cancel();
};