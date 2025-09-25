#pragma once
#include "MCommand.h"
#include <QVector>

class MLine;
class MPolygon;

class MCreatePolygon :
    public MCommand
{
private:
	QPointF m_begin;	// ֱ�ߵ����
	QPointF m_end;		// ֱ�ߵ��յ� 
	QPointF m_tempPos; //��ʱ��
	MPolygon* m_pPolygon;
	QVector<QPointF>    m_pPositions;
	QVector<MLine*> m_pTempLines;
	MLine* m_pTempLine;

public:
	MCreatePolygon(QCADView* pDiagramScene);
	~MCreatePolygon();

	int		GetType();
	int		OnLButtonDown(QMouseEvent* mouseEvent);
	int		OnMouseMove(QMouseEvent* mouseEvent);
	int		OnRButtonDown(QMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QMouseEvent* mouseEvent);

	int		Cancel();
};

