#include "math.h"
#include "Entity.h"
#include <iostream>

MRectangle::MRectangle()
{
	Init();
}

MRectangle::MRectangle(const MRectangle& rect)
	: MEntity(rect)
{
	Init();
	m_LeftTop = rect.m_LeftTop;
	m_RightBottom = rect.m_RightBottom;
}

MRectangle::MRectangle(const QPointF& LeftTop, const QPointF& RightBottom)
{
	Init();
	m_LeftTop = LeftTop;
	m_RightBottom = RightBottom;
}

MRectangle::~MRectangle() {}

MRectangle& MRectangle::operator = (const MRectangle& rect)
{
	// �������������line1 = line1
	if (this == &rect)
		return *this;
	// һ�����Σ�line2 = line1
	MEntity::operator = (rect); // ���û�������ء�=������
	m_LeftTop = rect.m_LeftTop;
	m_RightBottom = rect.m_RightBottom;

	return *this;
}

MEntity* MRectangle::Copy()
{
	MRectangle* pEntity = new MRectangle(m_LeftTop, m_RightBottom);
	return pEntity;
}

void MRectangle::Init()
{
	MEntity::Init();
	m_type = etRectangle;
	m_LeftTop = QPointF(0, 0);
	m_RightBottom = QPointF(0, 0);
}

void MRectangle::SetAttrib(QPen* pPen, QBrush* pBrush)
{
	if (pPen) {
		m_pen = *pPen;
	}
	if (pBrush) {
		m_brush = *pBrush;
	}
}

int	MRectangle::GetType()
{
	return etRectangle;
}

QPointF MRectangle::GetLeftTopPos()
{
	return m_LeftTop;
}

QPointF MRectangle::GetRightBottomPos()
{
	return m_RightBottom;
}

void MRectangle::Draw(QCADView* pView, int drawMode)
{
	QPainter pDC(pView);
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
	QPoint pt_begin, pt_end; // ��Ļ����������յ�
	pt_begin = pView->WorldtoScreen(m_LeftTop);
	pt_end = pView->WorldtoScreen(m_RightBottom);

	pDC.drawRect(QRect(pt_begin, pt_end));
}

bool MRectangle::Pick(const QPointF& pos, const double pick_radius)
{
	int m_numpts = 2;
	double Xmax, Xmin, Ymax, Ymin;
	QPointF objPos = pos;
	QRectF sourceBox, desBox;
	GetBox(sourceBox); // �õ�ֱ�߶ε���С��Χ��
	//// ����С��Χ�������ܷŴ󣬵õ����԰�Χ��
	desBox.setLeft(sourceBox.left() - pick_radius);
	desBox.setBottom(sourceBox.bottom() - pick_radius);
	desBox.setRight(sourceBox.right() + pick_radius);
	desBox.setTop(sourceBox.top() + pick_radius);

	//// �ж�ʰȡ���Ƿ��ڲ��԰�Χ���У�������ǣ���ͼԪδ��ѡ��
//if (objPos.x() < desBox.left() || objPos.x() > desBox.right() || \
		//objPos.y() < desBox.bottom() || objPos.y() > desBox.top())
		  //return false;

	if (m_RightBottom.x() < m_LeftTop.x())
	{
		Xmax = m_LeftTop.x();
		Xmin = m_RightBottom.x();
	}
	else
	{
		Xmax = m_RightBottom.x();
		Xmin = m_LeftTop.x();
	}
	if (m_RightBottom.y() < m_LeftTop.y())
	{
		Ymax = m_LeftTop.y();
		Ymin = m_RightBottom.y();
	}
	else
	{
		Ymax = m_RightBottom.y();
		Ymin = m_LeftTop.y();
	}


	if (objPos.x() < Xmax + pick_radius && objPos.x() > Xmin - pick_radius && objPos.y() < Ymax + pick_radius && objPos.y() > Ymin - pick_radius)
	{
		return true;
	}

	return false;
}

void MRectangle::Move(const QPointF& basePos, const QPointF& desPos, bool bTemp)
{
	if (m_LeftTop != basePos && m_RightBottom != desPos) {
	
		::Offset(m_LeftTop, desPos - basePos);
		::Offset(m_RightBottom, desPos - basePos);
	}
	else if (m_LeftTop == basePos && m_RightBottom != desPos) {
		m_LeftTop = basePos;
		m_RightBottom = desPos;
	}
	else {

	}

}

void MRectangle::Mirror(const QPointF& pos1, const QPointF& pos2) {
	::Mirror(m_LeftTop, pos1, pos2);
	::Mirror(m_RightBottom, pos1, pos2);
}

void MRectangle::Serialize(QDataStream& ar, bool bSave) {

	MEntity::Serialize(ar, bSave);

	if (bSave) {
		ar << m_LeftTop << m_RightBottom;
	}
	else {
		ar >> m_LeftTop >> m_RightBottom;
	}
}


bool MRectangle::GetSnapPos(QPointF& pos) {
	
	QPointF center = (m_LeftTop + m_RightBottom) / 2;

	
	if (::Distance(pos, center) < SNAP_DIS) {
		pos = center;
		return true;
	}

	
	return false;
}

void MRectangle::LoadPmtCursor() {
	
}

void MRectangle::GetBox(QRectF& pBox) {
	pBox = QRect(m_LeftTop.toPoint(), m_RightBottom.toPoint());
}

void MRectangle::Rotate(const QPointF& basePos, const double angle) {

	QPointF center = (m_LeftTop + m_RightBottom) / 2;
	::Rotate(m_LeftTop, center, angle);
	::Rotate(m_RightBottom, center, angle);
}
