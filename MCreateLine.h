#pragma once
#include "MCommand.h"

class MLine;

class MCreateLine :
    public MCommand
{
private:
	QPointF m_begin;	// 直线的起点
	QPointF m_end;		// 直线的终点 
	QPointF m_tempPos; //临时点
	MLine*  m_pLine;

public:
	MCreateLine(QCADView* pDiagramScene);
	~MCreateLine();

	int		GetType();
	int		OnLButtonDown(QMouseEvent* mouseEvent);
	int		OnMouseMove(QMouseEvent* mouseEvent);
	int		OnRButtonDown(QMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QMouseEvent* mouseEvent);

	int		Cancel();
};

