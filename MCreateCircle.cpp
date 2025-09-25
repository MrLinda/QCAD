#include "MCreateCircle.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"
#include <QMouseEvent>


MCreateCircle::MCreateCircle(QCADView* pDiagramScene)
    : m_center(0, 0), m_radius(0)
{
    m_nStep = 0;  // ��ʼ��������Ϊ 0
    m_pDC = pDiagramScene;
}
MCreateCircle::~MCreateCircle()
{
}

int MCreateCircle::GetType()
{
    return ctCreateCircle;  
}
//������������������
int MCreateCircle::OnLButtonDown(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return 0;

    QPen pen(m_pDC->penStyle());
    pen.setColor(m_pDC->lineColor());
    pen.setWidthF(m_pDC->penWidth());
    QBrush brush = m_pDC->brushColor();

    m_nStep++;  // ÿ�ε���������ʱ�������� 1

    switch (m_nStep)  // ���ݲ�����ִ����Ӧ�Ĳ���
    {
    case 1:
    {
        m_center = m_pDC->ScreentoWorld(mouseEvent->pos());  // ��¼Բ��λ��
        QString sPrmt = QStringLiteral("������Բ���ϵ�����㣺");
        Prompt(sPrmt);

        m_radius = 0; // ����뾶
        m_pCircle = new CCircle(m_center, m_radius);  // ���� MCircle ʵ��
        m_pCircle->SetPen(pen);
        m_pCircle->SetBrush(brush);
        m_pDC->addEntity(m_pCircle);  // ��ӵ���ͼ��

        break;
    }
    case 2:
    {
        QString sPrmt = QStringLiteral("������Բ�ĵ㣺");
        Prompt(sPrmt);

        m_nStep = 0;  // ���ò�������
        break;
    }
    default:
        break;
    }

    return 0;
}

int MCreateCircle::OnMouseMove(QMouseEvent* mouseEvent)
{
    MCommand::OnMouseMove(mouseEvent);

    if (m_nStep == 1 && m_pCircle != nullptr)
    {
        // ���û��ƶ����ʱ���������½ǵ�
        QPointF curPos = m_pDC->ScreentoWorld(mouseEvent->pos());
        QPointF dp = curPos - m_center;
        double r_del = sqrt(dp.x()*dp.x() + dp.y()*dp.y());
        m_pCircle->SetRadius(r_del);
        m_pDC->update();
    }
    else
    {
        QString sPrmt = QStringLiteral("������Բ�ĵ㣺");
        Prompt(sPrmt);
    }

    return 0;
}

int	MCreateCircle::OnRButtonDown(QMouseEvent* mouseEvent) {
    Cancel();
    return 0;
}

int MCreateCircle::Cancel()
{
    m_nStep = 0;  // ���ò�������
    Prompt(QStringLiteral("������Բ�ģ�"));  // ��ʾ��������
    return 0;
}
