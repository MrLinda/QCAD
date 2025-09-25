#include "MZoomWindow.h"
#include "QCADView.h"
#include <QMouseEvent>
#include "MCreateLine.h"
#include "mainwindow.h"
#include "ENTITY.H"

MZoomWindow::MZoomWindow(QCADView* pDiagramScene)
	:m_begin(0, 0), m_end(0, 0)
{
	m_nStep = 0;
	m_pDC = pDiagramScene;
	rubberBand = NULL;
}

MZoomWindow::~MZoomWindow()
{

}

int MZoomWindow::GetType()
{
	return ctZoomWindow;
}

int MZoomWindow::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	QPen pen(m_pDC->penStyle());
	pen.setColor(m_pDC->lineColor());
	pen.setWidthF(m_pDC->penWidth());

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

		QString sPrmt = QStringLiteral("���������ž��ε����½ǵ㣺");
		Prompt(sPrmt);
		break;
	}
	case 2:
	{
		m_end = mouseEvent->pos();
		//QPoint center = QPoint((m_begin.x() + m_end.x()) / 2, (m_begin.y() + m_end.y()) / 2);
		//int dx = m_end.x() - m_begin.x();
		//int dy = m_end.y() - m_begin.y();
		//double scale_x = m_pDC->rect().width() / dx;
		//double scale_y = m_pDC->rect().height() / dy;
		//double scale = scale_x < scale_y ? scale_x : scale_y;
		//m_pDC->ScaleCenter(scale, center);
		QPointF lt = m_pDC->ScreentoWorld(m_begin);
		QPointF rb = m_pDC->ScreentoWorld(m_end);
		QRectF rect(lt, rb);
		m_pDC->zoomRect(rect);

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


int	MZoomWindow::OnMouseMove(QMouseEvent* mouseEvent)
{
	MCommand::OnMouseMove(mouseEvent);

	switch (m_nStep)
	{
	case 0:
		::Prompt(QStringLiteral("����"));
		break;
	default:
	{
		QPointF curPos = m_pDC->ScreentoWorld(mouseEvent->pos());
		double dx = curPos.x() - m_begin.x();
		double dy = curPos.y() - m_begin.y();

		if (rubberBand)
			rubberBand->setGeometry(QRect(origin, mouseEvent->pos()).normalized());


		break;
	}
	}

	return 0;
}

// ��������Ҽ�ȡ����ǰ�Ĳ���
int	MZoomWindow::OnRButtonDown(QMouseEvent* mouseEvent)
{
	Cancel();
	return 0;
}
// ����Cancel ����ȡ�����β���

int MZoomWindow::Cancel()
{
	m_nStep = 0; // ������������Ϊ 0 
	Prompt(QStringLiteral("����")); // �ȴ���ʾ�����͵��������
	return 0;
}

