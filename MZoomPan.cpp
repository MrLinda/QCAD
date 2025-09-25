#include "MZoomPan.h"
#include "QCADView.h"
#include <QMouseEvent>
#include "MCreateLine.h"
#include "mainwindow.h"
#include "ENTITY.H"

MZoomPan::MZoomPan(QCADView* pDiagramScene)
	:m_begin(0, 0), m_end(0, 0)
{
	m_nStep = 0;
	m_pDC = pDiagramScene;
	rubberBand = NULL;
}

MZoomPan::~MZoomPan()
{

}

int MZoomPan::GetType()
{
	return ctZoomPan;
}

int MZoomPan::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	m_nStep++; // ÿ�ε���������ʱ�������� 1 

	switch (m_nStep) // ���ݲ�����ִ����Ӧ�Ĳ���
	{
	case 1:
	{
		m_begin = m_end = mouseEvent->pos();
        origin = mouseEvent->pos();
		if (!rubberBand)
			rubberBand = new QRubberBand(QRubberBand::Line, m_pDC);
		rubberBand->setGeometry(QRect(origin, QSize()));
		rubberBand->show();

		QString sPrmt = QStringLiteral("������ƽ�ƻ����Ŀ��㣺");
		Prompt(sPrmt);
		break;
	}
	case 2:
	{
		m_end = mouseEvent->pos();
		//double dx = m_end.x() - m_begin.x();
		//double dy = m_end.y() - m_begin.y();
		//m_pDC->MoveOrg(dx, dy);
		if (rubberBand)
			rubberBand->hide();

		m_nStep = 0;
		break;
	}
	default:
		break;
	}

	return 0;
}


int	MZoomPan::OnMouseMove(QMouseEvent* mouseEvent)
{
	MCommand::OnMouseMove(mouseEvent);

	switch (m_nStep)
	{
	case 0:
		::Prompt(QStringLiteral("����"));
		break;
	default:
	{
		QPointF curPos = mouseEvent->pos();
		double dx = curPos.x() - m_begin.x();
		double dy = curPos.y() - m_begin.y();

		if (rubberBand)
			rubberBand->setGeometry(QRect(origin, mouseEvent->pos()).normalized());

		m_pDC->MoveOrg(dx, dy);
		m_pDC->update();
		m_begin = curPos;

		break;
	}
	}

	return 0;
}

// ��������Ҽ�ȡ����ǰ�Ĳ���
int	MZoomPan::OnRButtonDown(QMouseEvent* mouseEvent)
{
	Cancel();
	return 0;
}
// ����Cancel ����ȡ�����β���

int MZoomPan::Cancel()
{
	m_nStep = 0; // ������������Ϊ 0 
	Prompt(QStringLiteral("����")); // �ȴ���ʾ�����͵��������
	return 0;
}

