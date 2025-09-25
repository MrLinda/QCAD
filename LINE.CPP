#include "math.h"
#include "Entity.h"

MLine::MLine()
{
	Init();
}

MLine::MLine(const MLine& line)
		: MEntity(line)
{
	Init();
	m_begin = line.m_begin;
	m_end	 = line.m_end;
}

MLine::MLine(const QPointF& begin,const QPointF& end)
{
	Init();
	m_begin = begin ;
	m_end = end ;
}

MLine::~MLine()
{
}

MLine& MLine::operator = (const MLine& line)
{
	// 处理特殊情况：line1 = line1
	if(this == &line)
		return *this;
	// 一般情形：line2 = line1
	MEntity::operator = (line); // 调用基类的重载“=”操作
	m_begin = line.m_begin;
	m_end = line.m_end;
	
	return *this;
}

MEntity* MLine::Copy()
{
	MLine*	pEntity = new MLine(m_begin, m_end);
	return pEntity;
}

void MLine::Init()
{
	MEntity::Init();
	m_type = etLine;
	m_begin = QPointF(0,0);
	m_end = QPointF(0, 0);
}

void MLine::SetAttrib(QPen* pPen, QBrush* pBrush)
{
	m_pen = *pPen;
	//直线不需要画刷
}


int	MLine::GetType()
{
	return etLine;
}

QPointF MLine::GetBeginPos()
{
	return m_begin;
}

QPointF MLine::GetEndPos()
{
	return m_end;
}

void MLine::Draw(QCADView * pView, int drawMode /* = dmNormal */)
{
	QPoint pt_begin, pt_end; // 屏幕坐标的起点和终点

	pt_begin = pView->WorldtoScreen(m_begin); // 将世界坐标转化为屏幕坐标
	pt_end = pView->WorldtoScreen(m_end) ;

	QPainter pDC(pView);
	//int		n = GetROP2(pDC->GetSafeHdc()); // 得到原来的绘图模式
	//::SetDrawEnvir(pDC, drawMode);
	
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
	pDC.drawLine(pt_begin, pt_end);
	
	//pDC->SelectObject(pOldPen); // 恢复原来的画笔 
	//pDC->SetROP2(n); // 恢复原来的绘图模式
}

bool MLine::Pick(const QPointF& pos, const double pick_radius)
{
	int m_numpts = 2;
	QPointF objPos = pos;
	QRectF sourceBox,desBox;
	GetBox(sourceBox); // 得到直线段的最小包围盒
	// 将最小包围盒向四周放大，得到测试包围盒
	desBox.setLeft(sourceBox.left() - pick_radius);
	desBox.setBottom ( sourceBox.bottom() - pick_radius);
	desBox.setRight( sourceBox.right() + pick_radius);
	desBox.setTop ( sourceBox.top() + pick_radius);

	// 判断拾取点是否在测试包围盒中，如果不是，则图元未被选中
	if( objPos.x() < desBox.left() || objPos.x() > desBox.right() || \
		objPos.y() < desBox.bottom() || objPos.y() > desBox.top())
		return false;

	double angle = ::GetAngleToXAxis(m_begin, m_end);
		// DIST = fabs(X * cos(a) + Y * sin(a) - P)
	QPointF dp = objPos - m_begin;
	double dist = fabs(dp.x()*cos(angle) + dp.y()*sin(angle) - Distance(objPos,m_begin));
	if(dist <= pick_radius)
		return true;

	return false;
}

void MLine::GetBox(QRectF& pBox)
{
	double min0 = 1E20;
	double min1 = 1E20;
	double max0 = -1E20;
	double max1 = -1E20;
	QVector<QPointF> pPositions;
	pPositions.push_back(m_begin);
	pPositions.push_back(m_end);

	int m_numpts = pPositions.size();

	for(int i=0; i<m_numpts; i++) {
		min0 = min0 <= pPositions[i].x()? min0 : pPositions[i].x();
		min1 = min1 <= pPositions[i].y() ? min1 : pPositions[i].y();
		max0 = max0 >= pPositions[i].x() ? max0 : pPositions[i].x();
		max1 = max1 >= pPositions[i].y() ? max1 : pPositions[i].y();
	}
	pBox.setLeft(min0); //min( m_begin.x, m_end.x );
	pBox.setBottom(min1); //min( m_begin.y, m_end.y );
	pBox.setRight( max0); //max( m_begin.x, m_end.x );
	pBox.setTop( max1); //max( m_begin.y, m_end.y );
}

void MLine::Move(const QPointF& basePos,const QPointF& desPos, bool bTemp)
{
	::Offset(m_begin, desPos - basePos);
	::Offset(m_end,  desPos - basePos);
}

void MLine::Rotate(const QPointF& bp, const double angle)
{
	QPointF basePos(bp);
	::Rotate(m_begin, basePos, angle) ;
    ::Rotate(m_end, basePos, angle) ;
}

void MLine::Mirror(const QPointF& pos1, const QPointF& pos2)
{
	::Mirror(m_begin, pos1, pos2) ;
	::Mirror(m_end ,pos1, pos2) ;
	//int m_numpts = m_pPositions.size();
	//for(int i=0; i<m_numpts; i++)
	//	m_pPositions[i] = m_pPositions[i].Mirror(pos1, pos2);
}

void MLine::LoadPmtCursor() 
{
	//设置鼠标样式，需要传入窗口的指针
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_PROMPT_LINE));
}


bool MLine::GetSnapPos(QPointF& pos)
{
	bool ret = false;

	QVector<QPointF> pPositions;
	pPositions.push_back(m_begin);
	pPositions.push_back(m_end);
	int m_numpts = pPositions.size();

	for( int i=0; i<m_numpts; i++ ){
		if( ::Distance(pos, pPositions[i]) < SNAP_DIS ){
			pos = pPositions[i];
			ret = true;
			break;
		}
	}

	return ret;
}

void MLine::Serialize(QDataStream& ar, bool bSave) 
{
	MEntity::Serialize(ar, bSave);

	if(bSave) {
		ar<<m_begin<<m_end;
	}
	else {
		ar>>m_begin>>m_end;
	}
}


