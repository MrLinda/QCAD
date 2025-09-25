#include "Entity.h" 
#include <math.h>

// ���캯������ʼ����������
CArc::CArc()
{
    Init();  // ��ʼ����Ա����
}

// ���ƹ��캯����������һ�������������
CArc::CArc(const CArc& arc)
{
    Init();
    m_center = arc.m_center;
    m_begin = arc.m_begin;
    m_end = arc.m_end;
}

// ͨ��Բ�ġ������յ㹹�컡����
CArc::CArc(const QPointF& center, const QPointF& startPos, const QPointF& endPos)
{
    Init();
    m_center = center;
    m_begin = startPos;
    m_end = endPos;
}

// ����������������Դ
CArc::~CArc()
{
}

// ��ʼ�����������û��ĳ�ʼ����
void CArc::Init()
{
    MEntity::Init();  // ��ʼ������
    m_type = etArc;   // ��������Ϊ����
    m_center = QPointF(0, 0);  // ��ʼ�����ĵ�Ϊ (0, 0)
    m_begin = QPointF(0, 0);   // ��ʼ�����
    m_end = QPointF(0, 0);     // ��ʼ���յ�
}

// ����Ϊ etArc
int CArc::GetType()
{
    return etArc;
}

// ���Ƶ�ǰ�����󣬷���һ��ָ���¶����ָ��
MEntity* CArc::Copy()
{
    return new CArc(m_center, m_begin, m_end);
}

// ���û��Ļ������ԣ��������ʺͻ�ˢ
void CArc::SetAttrib(QPen* pPen, QBrush* pBrush)
{
    m_pen = *pPen;
    // ��һ�㲻��Ҫ��ˢ
}

// ���ػ����������
QPointF CArc::GetStartPos()
{
    return m_begin;
}

// ���ػ����յ�����
QPointF CArc::GetEndPos()
{
    return m_end;
}

// ���ػ������ĵ�����
QPointF CArc::GetCenterPos()
{
    return m_center;
}

// �ж�ĳ�����Ƿ�ӽ����Ĺؼ��㣨���ĵ㡢�����յ㣩�����ڲ�׽����
bool CArc::GetSnapPos(QPointF& pos)
{
    // �ж��Ƿ�ӽ����ĵ�
    if (fabs(QLineF(pos, m_center).length()) < SNAP_DIS) {
        pos = m_center;
        return true;
    }
    // �ж��Ƿ�ӽ����
    if (fabs(QLineF(pos, m_begin).length()) < SNAP_DIS) {
        pos = m_begin;
        return true;
    }
    // �ж��Ƿ�ӽ��յ�
    if (fabs(QLineF(pos, m_end).length()) < SNAP_DIS) {
        pos = m_end;
        return true;
    }
    return false;
}

void CArc::SetEndPos(QPointF& pt)
{
    m_end = pt;
}

// ʹ�� `QPainter` ���ƻ���
void CArc::Draw(QCADView* pView, int drawMode)
{
    QPainter pDC(pView);

    // ���㻡�İ뾶
    qreal radius = sqrt(pow(m_center.x() - m_begin.x(), 2) + pow(m_center.y() - m_begin.y(), 2));

    // ����Բ����Ӿ���
    QPoint topLeft(m_center.x() - radius, m_center.y() + radius);
    QPoint bottomRight(m_center.x() + radius, m_center.y() - radius);
    topLeft = pView->WorldtoScreen(topLeft);
    bottomRight = pView->WorldtoScreen(bottomRight);
    QRectF rect(topLeft, bottomRight);

    // ������ʼ�ǶȺ���ֹ�Ƕ�
    qreal startAngle = acos((m_begin.x() - m_center.x()) / radius) * 180 / PI;
    if (m_begin.y() > m_center.y()) startAngle = -startAngle;
    qreal endAngle = acos((m_end.x() - m_center.x()) / radius) * 180 / PI;
    if (m_end.y() > m_center.y()) endAngle = -endAngle;

    // ���û���
    if (drawMode == dmSelect)
    {
        QPen pen(Qt::DashLine);
        pen.setColor(Qt::green);
        pen.setWidth(1);
        pDC.setPen(pen);
    }
    else {
        pDC.setPen(m_pen);
    }
    // �������

    // ���ƻ�
    pDC.drawArc(rect, -startAngle * 16, (-endAngle + startAngle) * 16);  // `* 16` ����Ϊ `drawArc` ʹ�õ��� 1/16 �ȵĵ�λ
}
// �жϵ� `pos` �Ƿ�λ�ڻ��ı�Ե����������ʰȡ���������������ΧΪ `pick_radius`
bool CArc::Pick(const QPointF& pos, const double pick_radius)
{
    // �жϸõ��Ƿ��ڻ��ı�Ե���������Ը���ͶӰ�ж�
    qreal distToCenter = sqrt(pow(pos.x() - m_center.x(), 2) + pow(pos.y() - m_center.y(), 2));
    qreal radius = sqrt(pow(m_center.x() - m_begin.x(), 2) + pow(m_center.y() - m_begin.y(), 2));

    // �������Ƿ���������Χ��
    return fabs(distToCenter - radius) <= pick_radius;
}

// ƽ�ƻ���λ�ã��� basePos �ƶ��� desPos���������ĵ㡢�����յ��λ�ã�����ȷ����������
void CArc::Move(const QPointF& basePos, const QPointF& desPos, bool bTemp)
{
    QPointF offset = desPos - basePos;
    m_center += offset;
    m_begin += offset;
    m_end += offset;
}

// ��ת���Σ��� `basePos` ����תָ���ĽǶ� `angle`
void CArc::Rotate(const QPointF& bp, const double angle)
{
    QPointF basePos(bp);
    ::Rotate(m_center, basePos, angle);
    ::Rotate(m_begin, basePos, angle);
    ::Rotate(m_end, basePos, angle);
}

// �Ի��ν��о���������������� `FstPos` �� `SecPos` ��ֱ�߶���
void CArc::Mirror(const QPointF& FstPos, const QPointF& SecPos)
{
    ::Mirror(m_center, FstPos, SecPos);
    ::Mirror(m_begin, FstPos, SecPos);
    ::Mirror(m_end, FstPos, SecPos);
}

// ��ȡ������С��Χ�У����Σ���ͨ������ `pBox` ���ؾ��α߽�
void CArc::GetBox(QRectF& pBox)
{
    qreal radius = sqrt(pow(m_center.x() - m_begin.x(), 2) + pow(m_center.y() - m_begin.y(), 2));
    pBox.setLeft(m_center.x() - radius);
    pBox.setTop(m_center.y() - radius);
    pBox.setRight(m_center.x() + radius);
    pBox.setBottom(m_center.y() + radius);
}

// ���л����������ڱ���ͼ��ػ��ļ�������
void CArc::Serialize(QDataStream& ar, bool bSave)
{
    MEntity::Serialize(ar, bSave);  // ���û�������л�����
    if (bSave) {
        ar << m_center << m_begin << m_end;  // ���满�����ĵ㡢�����յ�
    }
    else {
        ar >> m_center >> m_begin >> m_end;  // ���ػ������ĵ㡢�����յ�
    }
}

