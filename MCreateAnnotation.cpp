#include "ENTITY.H"
#include "mainwindow.h"
#include "MCreateAnnotation.h"
#include "QCADView.h"
#include <QMouseEvent>

MCreateAnnotation::MCreateAnnotation(QCADView* pDiagramScene, MAnnotation::AnnotationType annoType)
	: m_begin(0, 0), m_middle(0, 0), m_end(0, 0), m_pAnnotation(NULL), m_createAnnoType(annoType)
{
	m_nStep = 0; // ��ʼ��������Ϊ 0
	m_pDC = pDiagramScene;
	m_annoData.type = annoType;
	if (annoType == MAnnotation::atRoughness)
	{
		m_promptPrefix = QStringLiteral("��ָ������ֲڶȱ�ע����㣺");
	}
	else if (annoType == MAnnotation::atStandard)
	{
		m_promptPrefix = QStringLiteral("��ָ����׼��ע����㣺");
	}
	else if (annoType == MAnnotation::atWelding)
	{
		m_promptPrefix = QStringLiteral("��ָ�����ӱ�ע����㣺");
	}
}

MCreateAnnotation::~MCreateAnnotation()
{
	if (m_pAnnotation)
	{
		delete m_pAnnotation;
		m_pAnnotation = NULL;
	}
}

int MCreateAnnotation::GetType()
{
	return ctCreateAnnotation;
}

int MCreateAnnotation::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	m_nStep++;

	switch (m_nStep)
	{
	case 1:
		m_begin = m_pDC->ScreentoWorld(mouseEvent->pos());
		m_middle = m_end = m_begin;
		m_pAnnotation = new MAnnotation(m_begin, m_middle, m_end, m_annoData);
		m_pDC->addEntity(m_pAnnotation);
		m_promptPrefix = QStringLiteral("��ָ����ע���м�㣺");
		break;
	case 2:
		m_middle = m_pDC->ScreentoWorld(mouseEvent->pos());
		m_pAnnotation->SetMiddlePos(m_middle);
		m_promptPrefix = QStringLiteral("��ָ����ע���յ㣺");
		break;
	case 3:
		m_end = m_pDC->ScreentoWorld(mouseEvent->pos());
		// endλ��yֵ��middleλ��yֵ��ͬ
		m_pAnnotation->SetEndPos(QPointF(m_end.x(), m_middle.y()));
		m_nStep = 0;
		switch (m_createAnnoType)
		{
		case MAnnotation::atRoughness:
			m_promptPrefix = QStringLiteral("��ָ������ֲڶȱ�ע����㣺");
			break;
		case MAnnotation::atStandard:
			m_promptPrefix = QStringLiteral("��ָ����׼��ע����㣺");
			break;
		case MAnnotation::atWelding:
			m_promptPrefix = QStringLiteral("��ָ�����ӱ�ע����㣺");
			break;
		default:
			break;
		}
		break;
	}

	m_pDC->update();
	return 0;
}

int MCreateAnnotation::OnMouseMove(QMouseEvent* mouseEvent)
{
	QPointF pos = m_pDC->ScreentoWorld(mouseEvent->pos());

	switch (m_nStep)
	{
	case 1:
		m_middle = pos;
		m_pAnnotation->SetMiddlePos(m_middle);
		break;
	case 2:
		m_end = pos;
		// endλ��yֵ��middleλ��yֵ��ͬ
		m_pAnnotation->SetEndPos(QPointF(m_end.x(), m_middle.y()));
		break;
	default:
		break;
	}

	if (m_nStep > 0)
	{
		m_pDC->update();
	}
	
	Prompt(m_promptPrefix + QString::asprintf("(%d, %d)", static_cast<int>(pos.x()), static_cast<int>(pos.y())));
	return 0;
}

int MCreateAnnotation::OnRButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::RightButton)
		return 0;

	Cancel();
	return 0;
}

int MCreateAnnotation::Cancel()
{
	if (m_nStep > 0 && m_pAnnotation)
	{
		m_pDC->removeLastEntity();
		delete m_pAnnotation;
		m_pAnnotation = NULL;
	}

	m_nStep = 0;
	switch (m_createAnnoType)
	{
	case MAnnotation::atRoughness:
		m_promptPrefix = QStringLiteral("��ָ������ֲڶȱ�ע����㣺");
		break;
	case MAnnotation::atStandard:
		m_promptPrefix = QStringLiteral("��ָ����׼��ע����㣺");
		break;
	case MAnnotation::atWelding:
		m_promptPrefix = QStringLiteral("��ָ�����ӱ�ע����㣺");
		break;
	default:
		break;
	}
	m_pDC->update();
	return 0;
}

int MCreateAnnotation::OnLButtonDblClk(QMouseEvent* mouseEvent)
{
	return 0;
}
