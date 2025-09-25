#include "MCreateEllipse.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"  // ���� MEllipse �Ķ���

#include <QMouseEvent>

MCreateEllipse::MCreateEllipse(QCADView* pDiagramScene)
	: m_LeftTop(0, 0), m_RightBottom(0, 0), m_pEllipse(nullptr)
{
	m_nStep = 0; // ��ʼ��������Ϊ 0
	m_pDC = pDiagramScene;
	//m_pEllipse = nullptr; // ȷ�� m_pEllipse ��ʼ��Ϊ nullptr
}

MCreateEllipse::~MCreateEllipse()
{
	delete m_pEllipse;
	/*// ȷ������ʱ�ͷ�δ��ɵ���Բ����
	if (m_pEllipse != nullptr) {
		delete m_pEllipse;
		m_pEllipse = nullptr;
	}*/
}

int MCreateEllipse::GetType()
{
	return ctCreateEllipse; // �Զ�������ͣ����ڱ�ʶ������Բ
}

int MCreateEllipse::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	// ��� m_pDC �Ƿ��ѳ�ʼ������ֹ��ָ��
	if (m_pDC == nullptr) {
		return 0;  // �����ͼ������Ϊ�գ���ǰ����
	}

	QPen pen(m_pDC->penStyle());
	pen.setColor(m_pDC->lineColor());
	pen.setWidthF(m_pDC->penWidth());

	QBrush brush(m_pDC->brushColor());

	m_nStep++; // ÿ�ε���������ʱ�������� 1 

	switch (m_nStep)
	{
	case 1:
	{
		// ��һ�ε�������¼��Բ�����Ͻǵ�
		m_LeftTop = m_pDC->ScreentoWorld(mouseEvent->pos());
		m_RightBottom = m_LeftTop; // ��ʼʱ���½ǵ������Ͻǵ���ͬ

		//QBrush brush(m_pDC->brushColor());

		// ������Բʵ��
		m_pEllipse = new MEllipse(m_LeftTop, m_RightBottom);
		m_pEllipse->SetPen(pen);
		m_pEllipse->SetBrush(brush);
		//QBrush brush(m_pDC->brushColor());  // ʹ�� QCADView �� brushColor ��ʼ����ˢ
		m_pEllipse->SetAttrib(&pen, &brush); // ������Բ������
		m_pDC->addEntity(m_pEllipse); // ����Բ��ӵ�����

		QString sPrmt = QStringLiteral("��������Բ�����½ǵ㣺");
		Prompt(sPrmt);
		break;
	}
	case 2:
	{
		if (m_pEllipse == nullptr) {
			// ��ֹ��ָ�����
			return 0;
		}

		// �ڶ��ε�����ȷ�����½ǵ㣬�����Բ�Ļ���
		m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

		// ������Բ�����½ǵ㲢ˢ�³���
		//m_pEllipse->Move(m_LeftTop, m_RightBottom);
		//m_pDC->update();
		//m_pDC->repaint();

		// ��ʾ�������
		Prompt(QStringLiteral("��Բ�������"));

		m_nStep = 0; // �����Բ���ƺ����ò���

		// �����Բ�����Ա��������
		//delete m_pEllipse;  // ȷ���ͷ��ڴ�
		//m_pEllipse = nullptr;
		break;
	}
	default:
		break;
	}

	return 0;
}

int MCreateEllipse::OnMouseMove(QMouseEvent* mouseEvent)
{
	MCommand::OnMouseMove(mouseEvent);

	if (m_nStep == 1 && m_pEllipse != nullptr)
	{
		// ���û��ƶ����ʱ���������½ǵ�
		m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

		// ��̬������Բ����״
		m_pEllipse->Move(m_LeftTop, m_RightBottom);
		m_pDC->update();
	}

	return 0;
}

int MCreateEllipse::OnRButtonDown(QMouseEvent* mouseEvent)
{
	// �Ҽ����ȡ����ǰ����
	Cancel();
	return 0;
}

int MCreateEllipse::Cancel()
{
	if (m_nStep == 1 && m_pEllipse != nullptr) {
		// ��ȡ��ʱ������ɾ��δ��ɵ���Բ
		m_pDC->removeLastEntity();  // �Ƴ����һ��ʵ��
		m_pEllipse = nullptr;  // �����ٴη��ʿ�ָ��
	}

	m_nStep = 0;
	Prompt(QStringLiteral("��������Բ�����Ͻǵ�...")); // ��ʾ�µĲ���
	return 0;
}