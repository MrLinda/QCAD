#pragma once

#include "MCommand.h"
#include "ENTITY.H"

class MAnnotation;

class MCreateAnnotation :
	public MCommand
{
private:
	QPointF m_begin;	// 标注的起点
	QPointF m_middle;	// 标注的中间点
	QPointF m_end;		// 标注的终点 
	MAnnotation* m_pAnnotation;
	QString m_promptPrefix; // 提示信息前缀

	MAnnotation::AnnotationData m_annoData;
	MAnnotation::AnnotationType m_createAnnoType;
public:
	MCreateAnnotation(QCADView* pDiagramScene, MAnnotation::AnnotationType annoType);
	~MCreateAnnotation();
	int		GetType();
	int		OnLButtonDown(QMouseEvent* mouseEvent);
	int		OnMouseMove(QMouseEvent* mouseEvent);
	int		OnRButtonDown(QMouseEvent* mouseEvent);
	int		OnLButtonDblClk(QMouseEvent* mouseEvent);
	int		Cancel();
};