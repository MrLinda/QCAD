#include "MCreateArc.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"
#include <cmath>
#include "qevent.h"


MCreateArc::MCreateArc(QCADView* pDiagramScene)
    : m_center(0, 0), m_begin(0, 0), m_end(0, 0)
{
    m_nStep = 0;  // ��ʼ��������Ϊ 0
    m_pDC = pDiagramScene;

    m_pLine1 = NULL;
    m_pLine2 = NULL;
}

MCreateArc::~MCreateArc()
{
}

int MCreateArc::GetType()
{
    return ctCreateArc;  // ���ش������Ĳ�������
}

int MCreateArc::OnLButtonDown(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return 0;

    QPen pen(m_pDC->penStyle());
    pen.setColor(m_pDC->lineColor());
    pen.setWidthF(m_pDC->penWidth());

    m_nStep++;  // ÿ�ε���������ʱ�������� 1

    switch (m_nStep)  // ���ݲ�����ִ����Ӧ�Ĳ���
    {
    case 1:
    {
        m_center = m_pDC->ScreentoWorld(mouseEvent->pos());  // ��¼��������
        QString sPrmt = QStringLiteral("�����뻡����㣺");
        Prompt(sPrmt);

        //���Ƶ�һ��������
        m_pLine1 = new MLine(m_center, m_center);
        m_pLine1->SetPen(pen);
        m_pDC->addEntity(m_pLine1);
        break;
    }
    case 2:
    {
        m_begin = m_pDC->ScreentoWorld(mouseEvent->pos());  // ��¼�������
        QString sPrmt = QStringLiteral("�����뻡���յ㣺");
        Prompt(sPrmt);
        m_end = m_pDC->ScreentoWorld(mouseEvent->pos());  // ��¼�����յ�
        m_pArc = new CArc(m_center, m_begin, m_end);  // ���� MArc ʵ��
        m_pArc->SetPen(pen);
        m_pDC->addEntity(m_pArc);  // ��ӵ���ͼ��
        //���Ƶڶ���������
        m_pLine2 = new MLine(m_center, m_end);
        m_pLine2->SetPen(pen);
        m_pDC->addEntity(m_pLine2);
        break;
    }
    case 3:
    {
        QString sPrmt = QStringLiteral("�����뻡��Բ�ĵ㣺");
        Prompt(sPrmt);

        //ɾ��������
        m_pDC->removeEntity(m_pLine1);
        m_pDC->removeEntity(m_pLine2);
        delete m_pLine1;
        delete m_pLine2;
        m_pDC->update();

        m_nStep = 0;  // ���ò�������
        break;
    }
    default:
        break;
    }

    return 0;
}

int MCreateArc::OnMouseMove(QMouseEvent* mouseEvent)
{
    if (m_nStep == 1)
    {
        m_end = m_pDC->ScreentoWorld(mouseEvent->pos());
        m_pLine1->SetEndPos(m_end);
        m_pDC->update();
    }
    if (m_nStep == 2 && m_pArc != nullptr)
    {
        m_end = m_pDC->ScreentoWorld(mouseEvent->pos());  // ��¼�����յ�
        m_pArc->SetEndPos(m_end);

        m_pLine2->SetEndPos(m_end);
        m_pDC->update();
    }

    return 0;
}

int MCreateArc::OnRButtonDown(QMouseEvent* mouseEvent)
{
    Cancel();
    return 0;
}

int MCreateArc::Cancel()
{
    m_nStep = 0;  // ���ò�������
    if (m_pLine1) {
        delete m_pLine1;
        m_pLine1 = NULL;
    }
    if (m_pLine2) {
        delete m_pLine2;
        m_pLine2 = NULL;
    }
    Prompt(QStringLiteral("�����뻡�����ĵ㣺"));  // ��ʾ��������
    return 0;
}
