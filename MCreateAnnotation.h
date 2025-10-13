#pragma once

#include "MCommand.h"
#include "ENTITY.H"

class MAnnotation;

class MCreateAnnotation :
	public MCommand
{
private:
	QPointF m_begin;	// ��ע�����
	QPointF m_middle;	// ��ע���м��
	QPointF m_end;		// ��ע���յ� 
	MAnnotation* m_pAnnotation;
	QString m_promptPrefix; // ��ʾ��Ϣǰ׺

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