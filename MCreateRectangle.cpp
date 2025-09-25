#include "MCreateRectangle.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"  // ���� MRectangle �Ķ���

#include <QMouseEvent>

MCreateRectangle::MCreateRectangle(QCADView* pDiagramScene)
	: m_LeftTop(0, 0), m_RightBottom(0, 0), m_pRectangle(nullptr)
{
	m_nStep = 0; // ��ʼ��������Ϊ 0
	m_pDC = pDiagramScene;

	//rubberBand = NULL;
}

MCreateRectangle::~MCreateRectangle()
{
    delete m_pRectangle;
    /*// ȷ������ʱ�ͷ�δ��ɵľ��ζ���
    if (m_pRectangle != nullptr) {
        delete m_pRectangle;
        m_pRectangle = nullptr;
    }*/
}

int MCreateRectangle::GetType()
{
    return ctCreateRectangle; // �Զ�������ͣ����ڱ�ʶ���ƾ���
}

int MCreateRectangle::OnLButtonDown(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return 0;

    // ��� m_pDC �Ƿ��ѳ�ʼ������ֹ��ָ��
    if (m_pDC == nullptr) 
    {
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
        // ��һ�ε�������¼���ε����Ͻǵ�
        m_LeftTop = m_pDC->ScreentoWorld(mouseEvent->pos());
        m_RightBottom = m_LeftTop; // ��ʼʱ���½ǵ������Ͻǵ���ͬ

        //QBrush brush(m_pDC->brushColor());

        // ��������ʵ��
        m_pRectangle = new MRectangle(m_LeftTop, m_RightBottom);
        m_pRectangle->SetPen(pen);
        m_pRectangle->SetBrush(brush);
        //QBrush brush(m_pDC->brushColor());  // ʹ�� QCADView �� brushColor ��ʼ����ˢ
        m_pRectangle->SetAttrib(&pen, &brush); // ���þ��ε�����
        m_pDC->addEntity(m_pRectangle); // ��������ӵ�����

        QString sPrmt = QStringLiteral("��������ε����½ǵ㣺");
        Prompt(sPrmt);
        break;
    }
    case 2:
    {
        if (m_pRectangle == nullptr) {
            // ��ֹ��ָ�����
            return 0;
        }
        // �ڶ��ε�����ȷ�����½ǵ㣬�����Բ�Ļ���
        m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

        // ��ʾ�������
        Prompt(QStringLiteral("���λ������"));

        m_nStep = 0; // ��ɾ��λ��ƺ����ò���
        break;
    }
    default:
        break;
    }

    return 0;
}

int MCreateRectangle::OnMouseMove(QMouseEvent* mouseEvent)
{
    MCommand::OnMouseMove(mouseEvent);

    if (m_nStep == 1 && m_pRectangle != nullptr)
    {
        // ���û��ƶ����ʱ���������½ǵ�
        m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

        // ��̬���¾��ε���״
        m_pRectangle->Move(m_LeftTop, m_RightBottom);
        m_pDC->update();
    }

    return 0;
}

int MCreateRectangle::OnRButtonDown(QMouseEvent* mouseEvent)
{
    // �Ҽ����ȡ����ǰ����
    Cancel();
    return 0;
}

int MCreateRectangle::Cancel()
{
    if (m_nStep == 1 && m_pRectangle != nullptr) {
        // ��ȡ��ʱ������ɾ��δ��ɵľ���
        //m_pDC->removeEntity(m_pRectangle);
        m_pRectangle = nullptr;  // �����ٴη��ʿ�ָ��
    }

    m_nStep = 0;
    Prompt(QStringLiteral("��������ε����Ͻǵ�...")); // ��ʾ�µĲ���
    return 0;
}