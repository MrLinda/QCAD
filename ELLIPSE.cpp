#include "math.h"
#include "Entity.h"
#include <iostream>

// ���캯��
MEllipse::MEllipse() {
    Init();
}

MEllipse::MEllipse(const MEllipse& elps) {
    Init();
    m_LeftTop = elps.m_LeftTop;
    m_RightBottom = elps.m_RightBottom;
}

MEllipse::MEllipse(const QPointF& lt, const QPointF& rb) {
    Init();
    m_LeftTop = lt;
    m_RightBottom = rb;
    std::cout << "MEllipse created with points: (" << m_LeftTop.x() << ", " << m_LeftTop.y() << ") and ("
        << m_RightBottom.x() << ", " << m_RightBottom.y() << ")" << std::endl;
}

// ��������
MEllipse::~MEllipse() {}

// ��ʼ����Ա����
void MEllipse::Init() {
    MEntity::Init();
    m_type = etEllipse;
    m_LeftTop = QPointF(0,0);
    m_RightBottom = QPointF(0, 0);
}

// ��������
int MEllipse::GetType() {
    return m_type;
}

// ��������
void MEllipse::SetAttrib(QPen* pPen, QBrush* pBrush) {
    if (pPen) {
        m_pen = *pPen;
    }
    if (pBrush) {
        m_brush = *pBrush;
    }
    /*if (pPen == nullptr) {
        // ���� pPen Ϊ�յ����
        return;
    }
    m_pen = *pPen;
    m_brush = *pBrush;*/
}

// �������Ͻ�����
QPointF MEllipse::GetLTPos() {
    return m_LeftTop;
}

// �������½�����
QPointF MEllipse::GetRBPos() {
    return m_RightBottom;
}

// ������Բ
void MEllipse::Draw(QCADView* pView, int drawMode)
{
    QPainter pDC(pView);
    //if (drawMode == dmInvalid) return;

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

    // ������Բ�ľ��α߽粢������Բ
    QPoint lt = pView->WorldtoScreen(m_LeftTop);
    QPoint rb = pView->WorldtoScreen(m_RightBottom);
    QRectF ellipseRect(lt, rb);
    pDC.drawEllipse(ellipseRect);

    //QPen penToUse;
    //if (drawMode == dmNormal) {
    //    penToUse = m_pen;
    //    pDC.setBrush(m_brush);
    //}
    //else {
    //    penToUse.setColor(Qt::black);
    //    penToUse.setWidthF(1.0);
    //}

    //// ���û���  
    //pDC.setPen(penToUse);

    //// ������Բ  
    //pDC.drawEllipse(ellipseRect);

    //// �ָ�֮ǰ�Ļ��ʺͻ�ˢ  
    //pDC.setPen(oldPen);
    //pDC.setBrush(oldBrush);
}

// ѡȡ��Բ�߽�
bool MEllipse::Pick(const QPointF& pos, const double pick_radius) {
    double pick_adjusted = pick_radius + 2;

    // �Ȼ�ȡ��Χ�в������������ڳ����ж�
    QRectF sourceBox;
    GetBox(sourceBox);
    QPointF objPos = pos;
    QRectF desBox = sourceBox;
    desBox.adjust(-pick_adjusted, -pick_adjusted, pick_adjusted, pick_adjusted);

    // �ж�ʰȡ���Ƿ��ڲ��԰�Χ���У�������ǣ���ͼԪδ��ѡ��
    if (objPos.x() < desBox.left() || objPos.x() > desBox.right() || \
        objPos.y() > desBox.bottom() || objPos.y() < desBox.top())
        return false;

    // ������Բ�����ĵ�
    QPointF center((m_LeftTop.x() + m_RightBottom.x()) / 2, (m_LeftTop.y() + m_RightBottom.y()) / 2);

    // ����ԭʼ��Բ�ĳ�����Ͷ̰���
    double halfWidth = fabs(m_RightBottom.x() - center.x());
    double halfHeight = fabs(m_RightBottom.y() - center.y());

    // ��������Բ�ĳ�����Ͷ̰���
    double inflatedHalfWidth = halfWidth + pick_radius;
    double inflatedHalfHeight = halfHeight + pick_radius;

    // ��������Բ�ĳ�����Ͷ̰���
    double deflatedHalfWidth = halfWidth - pick_radius;
    double deflatedHalfHeight = halfHeight - pick_radius;

    // ����㵽��Բ���ĵ�ƫ��
    double dx = pos.x() - center.x();
    double dy = pos.y() - center.y();

    // �жϵ��Ƿ�������Բ��
    double outerEllipseEquation = (dx * dx) / (inflatedHalfWidth * inflatedHalfWidth) + (dy * dy) / (inflatedHalfHeight * inflatedHalfHeight);

    // �жϵ��Ƿ�������Բ��
    double innerEllipseEquation = (dx * dx) / (deflatedHalfWidth * deflatedHalfWidth) + (dy * dy) / (deflatedHalfHeight * deflatedHalfHeight);

    // �����������Բ�ڣ�����������Բ�ڣ�����Ϊѡ������Բ�ı߽�
    if (outerEllipseEquation <= 1.0 && innerEllipseEquation > 1.0) {
        return true;
    }

    return false;
}

// ��ȡ��Χ��
void MEllipse::GetBox(QRectF& pBox) {
    pBox = QRect(m_LeftTop.toPoint(), m_RightBottom.toPoint());
}

// ƽ�Ʋ���
void MEllipse::Move(const QPointF& basePos, const QPointF& desPos, bool bTemp) {
    if (m_LeftTop != basePos && m_RightBottom != desPos) {
        /*QPointF offset = desPos - basePos;
        m_LeftTop += offset;
        m_RightBottom += offset;*/
        ::Offset(m_LeftTop, desPos - basePos);
        ::Offset(m_RightBottom, desPos - basePos);
    }
    else if(m_LeftTop == basePos && m_RightBottom != desPos){
        m_LeftTop = basePos;
        m_RightBottom = desPos;
    }
    else {
        
    }

}

// ��ת����
void MEllipse::Rotate(const QPointF& basePos, const double angle) {
    // ��Բ��ת���߼�����ͨ�������ĵ���תʵ��
    QPointF center = (m_LeftTop + m_RightBottom) / 2;
    ::Rotate(m_LeftTop, center, angle);
    ::Rotate(m_RightBottom, center, angle);
}

// �������
void MEllipse::Mirror(const QPointF& pos1, const QPointF& pos2) {
    ::Mirror(m_LeftTop, pos1, pos2);
    ::Mirror(m_RightBottom, pos1, pos2);
}

// ���л�
void MEllipse::Serialize(QDataStream& ar, bool bSave) {
    
    MEntity::Serialize(ar, bSave);

    if (bSave) {
        ar << m_LeftTop << m_RightBottom;
    }
    else {
        ar >> m_LeftTop >> m_RightBottom;
    }
}

MEntity* MEllipse::Copy() {
    MEllipse* pEntity = new MEllipse(m_LeftTop, m_RightBottom);
    return pEntity;
}

bool MEllipse::GetSnapPos(QPointF& pos) {
    // ��Բ�����ĵ�
    QPointF center = (m_LeftTop + m_RightBottom) / 2;

    // �ж� pos �Ƿ�ӽ���Բ������
    if (::Distance(pos, center) < SNAP_DIS) {
        pos = center;
        return true;
    }

    // �����㲶׽�߼����Ը���������ӣ�������Բ�Ķ���
    return false;
}

void MEllipse::LoadPmtCursor() {
    // �������Ƕ�����һ����Բ�Ĺ����Դ IDC_PROMPT_ELLIPSE
    //::SetCursor(AfxGetApp()->LoadCursor(IDC_PROMPT_ELLIPSE));
}

//bool MEllipse::IsInRect(const QPointF& rectBegin, const QPointF& rectEnd){
//    QRect rect(rectBegin.toPoint(), rectEnd.toPoint()); // �������淶��QRect  
//    QRect ellipseBox;
//    GetBox(ellipseBox); // ��ȡ��Բ�ı߽��  
//    return rect.intersects(ellipseBox); // ������������Ƿ��ཻ  
//}