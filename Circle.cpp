// ���������ļ���ʵ���ඨ��
#include "Entity.h" 
#include <math.h>    

// CCircle���ʵ�֣����ڱ�ʾԲ�μ���ʵ��

// Ĭ�Ϲ��캯������ʼ��Բ�ĺͰ뾶
CCircle::CCircle()
{
    Init();  // ���ó�ʼ������

}

// ����Բ�ĸ���
CCircle::CCircle(const CCircle& circle)
{
    Init();  // ���ó�ʼ������
    MEntity::operator = (circle);
    m_center = circle.m_center;  // ����Բ��
    m_dRadius = circle.m_dRadius;  // ���ư뾶
}

// ���캯����ͨ��Բ�ĺͰ뾶����Բ
CCircle::CCircle(const QPointF& center, const double& radius)
{
    Init();  // ���ó�ʼ������
    m_center = center;  // ����Բ��
    m_dRadius = radius;  // ���ð뾶
}

// ���캯����ͨ��Բ�ĺ�Բ���ϵ�����һ�㴴��Բ��������뾶
CCircle::CCircle(const QPointF& center, const QPointF& around)
{
    Init();  // ���ó�ʼ������
    m_center = center;  // ����Բ��
    m_dRadius = sqrt(pow(center.x() - around.x(), 2) + pow(center.y() - around.y(), 2));
}

CCircle::~CCircle()
{
}

// ��ʼ������������Բ�Ļ�������
void CCircle::Init()
{
    MEntity::Init();  // ���û��� MEntity �ĳ�ʼ������
    m_type = etCircle;  // ����ʵ������ΪԲ��
    m_center = QPointF(0, 0);  // ��ʼ��Բ��Ϊ (0, 0)
    m_dRadius = 0;  // ��ʼ���뾶Ϊ 0
}

// ����Բ�Ļ�ͼ���ԣ��������ʺͻ�ˢ
void CCircle::SetAttrib(QPen* pPen, QBrush* pBrush)
{
    m_pen = *pPen;
    m_brush = *pBrush;
}
// ���ص�ǰʵ�������-Բ��
int CCircle::GetType()
{
    return etCircle;  
}

QPointF CCircle::GetCenterPos() {
    return m_center;
}

double CCircle::GetRadius() {
    return m_dRadius;
}

void CCircle::SetRadius(double r)
{
    m_dRadius = r;
}

MEntity* CCircle::Copy()
{
    CCircle* pEntity = new CCircle(m_center, m_dRadius);
    return pEntity;
}

// ����Բ�εĺ�����ʹ�� QPainter ��Բ���ܽ��л��ƣ����ǹ���߽����Ϊ������
void CCircle::Draw(QCADView* pView, int drawMode)
{
    QPainter pDC(pView);
    QPointF topLeft(m_center.x() - m_dRadius, m_center.y() + m_dRadius);
    QPointF bottom_right(m_center.x() + m_dRadius, m_center.y() - m_dRadius);
    topLeft = pView->WorldtoScreen(topLeft);
    bottom_right = pView->WorldtoScreen(bottom_right);
    QPen pOldPen = pDC.pen();
    QBrush pOldBrush = pDC.brush();
    // ͨ��Բ�ĺͰ뾶�����Բ�ı߽����
    QRectF circleRect(topLeft, bottom_right);// cen_scn.x() - m_dRadius, cen_scn.y() - m_dRadius, 2 * m_dRadius, 2 * m_dRadius);
  
    if (drawMode == dmSelect)
    {
        QPen pen(Qt::DashLine);
        pen.setColor(Qt::green);
        pen.setWidth(1);
        pDC.setPen(pen);
        QBrush brush(Qt::white);
        pDC.setBrush(brush);
    }
    else {
        pDC.setPen(m_pen);  // ���û���
        pDC.setBrush(m_brush);  // ���û�ˢ
    }

    pDC.drawEllipse(circleRect); // �����������������Բ����Բ
    pDC.setPen(pOldPen);
    pDC.setBrush(pOldBrush);
}


// �ж�ĳ�����Ƿ���Բ�ı�Ե����������ʰȡ����
bool CCircle::Pick(const QPointF& pos, const double pick_radius)
{
    // ����õ㵽Բ�ĵľ���
    qreal dist = sqrt(pow(pos.x() - m_center.x(), 2) + pow(pos.y() - m_center.y(), 2));
    // �жϸõ��Ƿ���Բ�ı�Ե�������������ΧΪ pick_radius
    return fabs(dist - m_dRadius) <= pick_radius;
}

// ��ȡԲ����С��Χ�У����ؾ�������
void CCircle::GetBox(QRectF& pBox)
{
    // ͨ��Բ�ĺͰ뾶�������С��Χ�еı߽�
    pBox.setLeft(m_center.x() - m_dRadius);    // ������߽�
    pBox.setTop(m_center.y() - m_dRadius);     // �����ϱ߽�
    pBox.setRight(m_center.x() + m_dRadius);   // �����ұ߽�
    pBox.setBottom(m_center.y() + m_dRadius);  // �����±߽�
}
//�ƶ�ʱ��Բ�ĵ�λ���йأ����뾶�޹�
// ƽ��Բ��λ�ã��� basePos �ƶ��� desPos
void CCircle::Move(const QPointF& basePos, const QPointF& desPos, bool bTemp)
{
    // ����ƫ����
    QPointF offset = desPos - basePos;
    // ����Բ��λ��
    m_center += offset;
}

// ��תԲ�����ĵ�
void CCircle::Rotate(const QPointF& bp, const double angle)
{
    QPointF basePos(bp);  
    // ʹ��Rotate ������תԲ��
    ::Rotate(m_center, basePos, angle);
}

// ��Բ�Ľ��о������
void CCircle::Mirror(const QPointF& pos1, const QPointF& pos2)
{
    // ʹ��ȫ�� Mirror ������Բ�Ľ��о���
    ::Mirror(m_center, pos1, pos2);
}

// ��׽���ܣ��жϸ����ĵ��Ƿ�ӽ�Բ�ģ����ز�׽���
bool CCircle::GetSnapPos(QPointF& pos)
{
    // ����õ㵽Բ�ĵľ���С�� SNAP_DIS����׽������ֵ�����򽫸õ㲶׽��Բ��
    if (fabs(QLineF(pos, m_center).length()) < SNAP_DIS) {
        pos = m_center;  // ��������ΪԲ��
        return true;  // ��׽�ɹ�
    }
    return false;  // δ��׽��
}

// ���л����������ڱ���ͼ���Բ������
void CCircle::Serialize(QDataStream& ar, bool bSave)
{
    // ���û�������л�����
    MEntity::Serialize(ar, bSave);
    if (bSave) {
        // ����Ǳ����������Բ�ĺͰ뾶д��������
        ar << m_center << m_dRadius;
    }
    else {
        // ����Ǽ��ز��������������ж�ȡԲ�ĺͰ뾶
        ar >> m_center >> m_dRadius;
    }
}
