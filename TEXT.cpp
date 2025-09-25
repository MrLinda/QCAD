#include "ENTITY.H"

#include <Windows.h>

MText::MText()
{
	Init();
}

MText::MText(const MText& text)
	: MEntity(text)
{
	Init();
	m_Text = text.m_Text;
	m_LeftTop = text.m_LeftTop;
	m_RightBottom = text.m_RightBottom;
}

MText::MText(const QPointF& leftTop, const QPointF& rightBottom, const char* text)
{
	Init();
	m_LeftTop = leftTop;
	m_RightBottom = rightBottom;
	m_Text = text;
}

void MText::Init()
{
	MEntity::Init();
	m_type = etText;
	m_LeftTop = QPointF(0, 0);
	m_RightBottom = QPointF(0, 0);
	m_Text = "";
}

MText::~MText()
{
}

MText& MText::operator = (const MText& text)
{
	// 处理特殊情况：text1 = text1
	if (this == &text)
		return *this;
	// 一般情形：text2 = text1
	MEntity::operator = (text); // 调用基类的重载“=”操作
	m_Text = text.m_Text;
	m_LeftTop = text.m_LeftTop;
	m_RightBottom = text.m_RightBottom;

	return *this;
}

MEntity* MText::Copy()
{
	return new MText(m_LeftTop, m_RightBottom, m_Text.toLocal8Bit().data());
}

int MText::GetType()
{
	return m_type;
}

void MText::SetAttrib(QPen* pPen, QBrush* pBrush)
{
	m_pen = *pPen;
	m_brush = *pBrush;
}

QPointF MText::GetLeftTopPos()
{
	return m_LeftTop;
}

void MText::SetLeftTopPos(QPointF pos)
{
	m_LeftTop = pos;
}

QPointF MText::GetRightBottomPos()
{
	return m_RightBottom;
}

void MText::SetRightBottomPos(QPointF pos)
{
	m_RightBottom = pos;
}

QString MText::GetText()
{
	return m_Text;
}

void MText::SetText(const char* text)
{
	m_Text = text;
}

void MText::SetFont(QFont font)
{
	m_Font = font;
}

QColor MText::GetTextColor()
{
	return m_TextColor;
}

void MText::SetTextColor(QColor color)
{
	m_TextColor = color;
}


void MText::Draw(QCADView* pView, int drawMode)
{
	QPainter pDC(pView);
	QPen pOldPen = pDC.pen();
	QBrush pOldBrush = pDC.brush();
	pDC.setPen(m_pen);
	pDC.setBrush(m_brush);
	pDC.setFont(m_Font);

	QPoint lt = pView->WorldtoScreen(m_LeftTop);
	QPoint rb = pView->WorldtoScreen(m_RightBottom);
	
	OutputDebugString(L"%%%%%%%% TEXT\n");
	OutputDebugString(L"Draw Text\n");
	OutputDebugString(L"mText: ");
	OutputDebugString(m_Text.toStdWString().c_str());
	OutputDebugString(L"\n");
	if (drawMode == dmSelect)
	{
		OutputDebugString(L"Draw Border\n");
		OutputDebugString(L"\n");
		QPen pen(Qt::darkGray, 1, Qt::DashLine);
		pDC.setPen(pen);
		pDC.drawRect(QRect(lt, rb));
		pDC.setPen(m_pen);
	}
	pDC.setPen(m_TextColor);
	pDC.drawText(QRect(lt, rb), Qt::AlignCenter, m_Text);

	pDC.setPen(pOldPen);
	pDC.setBrush(pOldBrush);
}

bool MText::Pick(const QPointF& pos, const double pick_radius)
{
	if (pos.x() >= m_LeftTop.x() && pos.x() <= m_RightBottom.x() &&
		pos.y() >= m_LeftTop.y() && pos.y() <= m_RightBottom.y())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MText::GetBox(QRectF& pBox)
{
	pBox.setTopLeft(m_LeftTop.toPoint());
	pBox.setBottomRight(m_RightBottom.toPoint());
}

void MText::Move(const QPointF& basePos, const QPointF& desPos, bool bTemp)
{
	::Offset(m_LeftTop, desPos - basePos);
	::Offset(m_RightBottom, desPos - basePos);
}

void MText::Rotate(const QPointF& basePos, const double angle)
{
	// TODO
}

void MText::Mirror(const QPointF& pos1, const QPointF& pos2)
{
	::Mirror(m_LeftTop, pos1, pos2);
	::Mirror(m_RightBottom, pos1, pos2);
}

bool MText::GetSnapPos(QPointF& pos)
{
	bool res = false;

	QVector<QPointF> pPositions;
	pPositions.push_back(m_LeftTop);
	pPositions.push_back(m_RightBottom);

	for (auto& point : pPositions) {
		if (::Distance(pos, point) < SNAP_DIS) {
			pos = point;
			res = true;
			break;
		}
	}

	return res;
}

void MText::LoadPmtCursor()
{
	// TODO
}

void MText::Serialize(QDataStream& ar, bool bSave)
{
	MEntity::Serialize(ar, bSave);

	if (bSave) {
		ar << m_LeftTop << m_RightBottom << m_Text << m_Font;
	}
	else {
		ar >> m_LeftTop >> m_RightBottom >> m_Text >> m_Font;
	}
}