#include "math.h"
#include "Entity.h"
#include <QDebug>
#include <QVector>
#include <QFont>

#define pointHorizVertTolerance 3 // 点与水平或垂直线的容差，单位像素

MAnnotation::MAnnotation()
{
	Init();
}
MAnnotation::MAnnotation(const MAnnotation& ann)
	: MEntity(ann)
{
	Init();
	m_begin = ann.m_begin;
	m_end = ann.m_end;
	m_middle = ann.m_middle;
	m_annoData = ann.m_annoData;
	m_font = ann.m_font;
}
MAnnotation::MAnnotation(const QPointF& startPos, const QPointF& middlePos, const QPointF& endPos, AnnotationData annoData)
{
	Init();
	m_begin = startPos;
	m_end = endPos;
	m_middle = middlePos;
	m_annoData = annoData;
}

void MAnnotation::Init()
{
	MEntity::Init();
	m_type = etAnnotation;
	m_begin = QPointF(0, 0);
	m_end = QPointF(0, 0);
	m_middle = QPointF(0, 0);
	m_annoData.type = atRoughness;
	m_annoData.roughnessMin = "";
	m_annoData.roughnessMax = "";
	m_annoData.obtainMethod = 0;
	m_annoData.standardName = "";
	m_annoData.weldingName = "";
	m_font = QFont("SimSun", 12);
}

MAnnotation::~MAnnotation()
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i])
		{
			delete entities[i];
			entities[i] = NULL;
		}
	}
	entities.clear();
}

MEntity* MAnnotation::Copy()
{
	return new MAnnotation(m_begin, m_middle, m_end, m_annoData);
}

//-----------------------------------------------
// member function
// Attributes
int MAnnotation::GetType()
{
	return m_type;
}
void MAnnotation::SetAttrib(QPen* pPen, QBrush* pBrush)
{
	m_pen = *pPen;
	m_brush = *pBrush;
}
QPointF MAnnotation::GetStartPos() // 返回起点值
{
	return m_begin;
}
QPointF MAnnotation::GetMiddlePos() // 返回中间点值
{
	return m_middle;
}
QPointF MAnnotation::GetEndPos() // 返回终点值
{
	return m_end;
}

bool MAnnotation::GetSnapPos(QPointF& pos)
{
	return false;
}


void MAnnotation::Draw(QCADView* pView, int drawMode)
{
	QPainter pDC(pView);
	if(drawMode == dmNormal)
	{
		// 清理旧的实体
		for (MEntity* entity : entities) {
			if (entity) delete entity;
		}
		entities.clear();

		if (m_annoData.type == atRoughness)
		{
			//qDebug() << "进入粗糙度绘制分支";
			//qDebug() << "begin:" << m_begin << "middle:" << m_middle << "end:" << m_end;
			//qDebug() << "obtainMethod:" << m_annoData.obtainMethod;
			qDebug() << "缩放因子：" << pView->GetScale();
			QFont realFont = QFont(m_font.family(), m_font.pointSize() * pView->GetScale());
			qDebug() << "字体高度：" << realFont.pointSize();
			QFontMetrics metrics(realFont);

			// 画表面粗糙度标注
			QPoint pt_begin = pView->WorldtoScreen(m_begin);
			QPoint pt_middle = pView->WorldtoScreen(m_middle);
			QPoint pt_end = pView->WorldtoScreen(m_end);

			// 是否画水平线
			bool drawLeader = true;
			// 画引线
			if (Distance(pt_begin, pt_middle) < pointHorizVertTolerance)
			{
				// 无引线
				m_middle = m_begin;
				// 固定不动
				m_end = m_middle;
				drawLeader = false;
			}
			else
			{
				if (abs(pt_begin.x() - pt_middle.x()) <= pointHorizVertTolerance)
				{
					// 竖直线
					m_middle.setX(m_begin.x());
					pt_middle = pView->WorldtoScreen(m_middle);

					entities.push_back(new MLine(m_begin, m_middle));

				}
				else if (abs(pt_begin.y() - pt_middle.y()) <= pointHorizVertTolerance)
				{
					// 水平线
					m_middle.setY(m_begin.y());
					pt_middle = pView->WorldtoScreen(m_middle);

					entities.push_back(new MLine(m_begin, m_middle));
				}
				else
				{
					// 引线
					entities.push_back(new MLine(m_begin, m_middle));
				}
			}

			// 画水平线
			if(drawLeader)
			{
				entities.push_back(new MLine(m_middle, m_end));
			}

			// 画粗糙度符号
			qreal scale = pView->GetScale();
			qreal textHeight = metrics.height() / scale;	// 字体高度
			qDebug() << "textHeight:" << textHeight;
			qreal signHeight = textHeight * 1.4;						// 符号高度
			qreal signOffset = signHeight * 1.1;						// 符号偏移 
			QPointF signP1, signP2, signP3;
			if (pt_end.x() >= pt_middle.x())
			{
				// 右侧
				signP1 = QPointF(m_middle.x() + signOffset - signHeight / tan(PI / 3), m_middle.y() + signHeight);
				signP2 = QPointF(m_middle.x() + signOffset, m_middle.y());
				signP3 = QPointF(m_middle.x() + signOffset + 2 * signHeight / tan(PI / 3), m_middle.y() + 2 * signHeight);
			}
			else
			{
				// 左侧
				signP1 = QPointF(m_end.x() + signOffset - signHeight / tan(PI / 3), m_end.y() + signHeight);
				signP2 = QPointF(m_end.x() + signOffset, m_middle.y());
				signP3 = QPointF(m_end.x() + signOffset + 2 * signHeight / tan(PI / 3), m_end.y() + 2 * signHeight);
			}
			entities.push_back(new MLine(signP1, signP2));
			entities.push_back(new MLine(signP2, signP3));

			qreal radius = signHeight / tan(PI / 3) / sin(PI / 3) * 2 / 3;
			QPointF circleCenter = (2 * (signP1 + (signP2 + signP3) / 2) / 2 + signP2) / 3 + QPointF(0, radius * cos(PI / 3));
			if (m_annoData.obtainMethod == 0)
			{
				// 未注
			}
			else if (m_annoData.obtainMethod == 1)
			{
				// 去除材料
				entities.push_back(new MLine(signP1, (signP2 + signP3) / 2));
			}
			else if (m_annoData.obtainMethod == 2)
			{
				// 不去除材料
				entities.push_back(new CCircle(circleCenter, radius));
			}

			// 画文字
			/*QPointF textLowerRightBottom;
			QPointF textUpperRightBottom;*/
			QPointF textLowerLeftTop;
			QPointF textUpperLeftTop;
			textUpperLeftTop = signP3 - QPointF(0, textHeight * 0.1);
			textLowerLeftTop = textUpperLeftTop - QPointF(0, textHeight);
			//if (m_annoData.obtainMethod == 0 || m_annoData.obtainMethod == 1)
			//{
			//	// 未注或去除材料
			//	textLowerRightBottom = (signP1 + 3 * (signP2 + signP3) / 2) / 4 + QPointF(signHeight / tan(PI/3), signHeight * 0.1);
			//	textUpperRightBottom = textLowerRightBottom + QPointF(0, textHeight);
			//}
			//else if (m_annoData.obtainMethod == 2)
			//{
			//	// 不去除材料
			//	textLowerRightBottom = circleCenter + QPointF(signHeight / tan(PI / 3), radius + signHeight * 0.1);
			//	textUpperRightBottom = textLowerRightBottom + QPointF(0, textHeight);
			//}
			MText* pTextMin = new MText();
			MText* pTextMax = new MText();
			pTextMin->SetFont(realFont);
			pTextMax->SetFont(realFont);
			if(!m_annoData.roughnessMin.isEmpty() && !m_annoData.roughnessMax.isEmpty())
			{
				// 最小值和最大值都不为空
				int textWidthMin = metrics.horizontalAdvance(m_annoData.roughnessMin) / scale;
				pTextMin->SetText(m_annoData.roughnessMin);
				/*pTextMin->SetRightBottomPos(textLowerRightBottom);
				pTextMin->SetLeftTopPos(QPointF(textLowerRightBottom.x() - textWidth, textLowerRightBottom.y() + textHeight));*/
				pTextMin->SetLeftTopPos(textLowerLeftTop);
				pTextMin->SetRightBottomPos(textLowerLeftTop + QPointF(textWidthMin, -textHeight));

				int textWidthMax = metrics.horizontalAdvance(m_annoData.roughnessMax) / scale;
				pTextMax->SetText(m_annoData.roughnessMax);
				/*pTextMax->SetRightBottomPos(textUpperRightBottom);
				pTextMax->SetLeftTopPos(QPointF(textUpperRightBottom.x() - textWidth, textUpperRightBottom.y() + textHeight));*/
				pTextMax->SetLeftTopPos(textUpperLeftTop);
				pTextMax->SetRightBottomPos(textUpperLeftTop + QPointF(textWidthMax, -textHeight));

				MLine* pLine = new MLine(signP3, signP3 + QPointF(fmax(textWidthMin, textWidthMax),0));

				entities.push_back(pTextMin);
				entities.push_back(pTextMax);
				entities.push_back(pLine);
			}
			else if (!m_annoData.roughnessMin.isEmpty())
			{
				// 只有最小值不为空
				int textWidth = metrics.horizontalAdvance(m_annoData.roughnessMin) / scale;
				pTextMin->SetText(m_annoData.roughnessMin);
				/*pTextMin->SetRightBottomPos(textLowerRightBottom);
				pTextMin->SetLeftTopPos(QPointF(textLowerRightBottom.x() - textWidth, textLowerRightBottom.y() + textHeight));*/
				pTextMin->SetLeftTopPos(textUpperLeftTop);
				pTextMin->SetRightBottomPos(textUpperLeftTop + QPointF(textWidth, -textHeight));

				MLine* pLine = new MLine(signP3, signP3 + QPointF(textWidth, 0));

				entities.push_back(pTextMin);
				entities.push_back(pLine);
			}
			else if (!m_annoData.roughnessMax.isEmpty())
			{
				// 只有最大值不为空
				int textWidth = metrics.horizontalAdvance(m_annoData.roughnessMax) / scale;
				pTextMax->SetText(m_annoData.roughnessMax);
				/*pTextMax->SetRightBottomPos(textUpperRightBottom);
				pTextMax->SetLeftTopPos(QPointF(textUpperRightBottom.x() - textWidth, textUpperRightBottom.y() + textHeight));*/
				pTextMax->SetLeftTopPos(textUpperLeftTop);
				pTextMax->SetRightBottomPos(textUpperLeftTop + QPointF(textWidth, -textHeight));

				MLine* pLine = new MLine(signP3, signP3 + QPointF(textWidth, 0));

				entities.push_back(pTextMax);
				entities.push_back(pLine);
			}
			qDebug() << "begin:" << m_begin << "middle:" << m_middle << "end:" << m_end;
			qDebug() << "signP2:" << signP2;
		}
		else if (m_annoData.type == atStandard)
		{
			qDebug() << "基准标注功能实现";
			qDebug() << "begin:" << m_begin << "middle:" << m_middle << "end:" << m_end;

			QPoint pt_begin = pView->WorldtoScreen(m_begin);
			QPoint pt_middle = pView->WorldtoScreen(m_middle);
			QPoint pt_end = pView->WorldtoScreen(m_end);

			entities.push_back(new MLine(m_begin, m_middle));
			entities.push_back(new MLine(m_middle, m_end));

			qreal boxSize = 15;
			qreal offset = boxSize * 1.2;

			QPointF lineDirection = m_end - m_middle;
			QPointF perpendicularDir(-lineDirection.y(), lineDirection.x());
			qreal length = sqrt(perpendicularDir.x() * perpendicularDir.x() + perpendicularDir.y() * perpendicularDir.y());
			if (length > 0) {
				perpendicularDir /= length;
			}

			QPointF midPoint = (m_middle + m_end) / 2;
			QPointF boxTopLeft = midPoint + perpendicularDir * offset - QPointF(boxSize / 2, boxSize);

			QPointF boxTopRight = QPointF(boxTopLeft.x() + boxSize, boxTopLeft.y());
			QPointF boxBottomLeft = QPointF(boxTopLeft.x(), boxTopLeft.y() + boxSize);
			QPointF boxBottomRight = QPointF(boxTopLeft.x() + boxSize, boxTopLeft.y() + boxSize);

			entities.push_back(new MLine(boxTopLeft, boxTopRight));
			entities.push_back(new MLine(boxTopRight, boxBottomRight));
			entities.push_back(new MLine(boxBottomRight, boxBottomLeft));
			entities.push_back(new MLine(boxBottomLeft, boxTopLeft));

			qreal aSize = boxSize * 0.6;
			qreal aOffset = (boxSize - aSize) / 2;

			// 注意：这里假设 m_annoData.weldingType 存在且用于基准类型区分
			// 如果实际结构不同，请根据实际情况修改判断逻辑
			// 假设 IWeld, VWeld, FilletWeld 代表基准 A, B, C
			if (m_annoData.weldingType == IWeld) { // 基准A
				QPointF aBottom = QPointF(boxTopLeft.x() + aOffset + aSize / 2, boxTopLeft.y() + aOffset + aSize);
				QPointF aLeft = QPointF(boxTopLeft.x() + aOffset, boxTopLeft.y() + aOffset);
				QPointF aRight = QPointF(boxTopLeft.x() + aOffset + aSize, boxTopLeft.y() + aOffset);
				QPointF aMid = QPointF(boxTopLeft.x() + aOffset + aSize / 3, boxTopLeft.y() + aOffset + aSize / 2);
				QPointF aMid2 = QPointF(boxTopLeft.x() + aOffset + 2 * aSize / 3, boxTopLeft.y() + aOffset + aSize / 2);

				entities.push_back(new MLine(aBottom, aLeft));
				entities.push_back(new MLine(aBottom, aRight));
				entities.push_back(new MLine(aMid, aMid2));
			}
			else if (m_annoData.weldingType == VWeld) { // 基准B
				QPointF leftTop = QPointF(boxTopLeft.x() + aOffset, boxTopLeft.y() + aOffset);
				QPointF leftBottom = QPointF(boxTopLeft.x() + aOffset, boxTopLeft.y() + aOffset + aSize);
				QPointF middle = QPointF(boxTopLeft.x() + aOffset + aSize / 2, boxTopLeft.y() + aOffset + aSize / 2);
				QPointF rightTop = QPointF(boxTopLeft.x() + aOffset + aSize, boxTopLeft.y() + aOffset);
				QPointF rightMiddle = QPointF(boxTopLeft.x() + aOffset + aSize, boxTopLeft.y() + aOffset + aSize / 2);
				QPointF rightBottom = QPointF(boxTopLeft.x() + aOffset + aSize, boxTopLeft.y() + aOffset + aSize);

				entities.push_back(new MLine(leftTop, leftBottom));
				entities.push_back(new MLine(leftTop, rightTop));
				entities.push_back(new MLine(leftTop, middle));
				entities.push_back(new MLine(middle, rightMiddle));
				entities.push_back(new MLine(middle, rightBottom));
				entities.push_back(new MLine(leftBottom, rightBottom));
			}
			else if (m_annoData.weldingType == FilletWeld) { // 基准C
				qreal radius = aSize / 3;
				QPointF centerTop = QPointF(boxTopLeft.x() + aOffset + aSize / 2, boxTopLeft.y() + aOffset + radius);
				QPointF centerBottom = QPointF(boxTopLeft.x() + aOffset + aSize / 2, boxTopLeft.y() + aOffset + aSize - radius);
				QPointF start = QPointF(boxTopLeft.x() + aOffset, boxTopLeft.y() + aOffset + radius);
				QPointF mid1 = QPointF(boxTopLeft.x() + aOffset, boxTopLeft.y() + aOffset + aSize / 2);
				QPointF end = QPointF(boxTopLeft.x() + aOffset, boxTopLeft.y() + aOffset + aSize - radius);

				entities.push_back(new MLine(start, mid1));
				entities.push_back(new MLine(mid1, end));
				entities.push_back(new MLine(start, QPointF(start.x() + radius, start.y() - radius)));
				entities.push_back(new MLine(end, QPointF(end.x() + radius, end.y() + radius)));
			}
		}
		else if (m_annoData.type == atWelding)
		{
			qDebug() << "进入焊接绘制分支"; // 修正了 qDebug 输出内容
			qDebug() << "begin:" << m_begin << "middle:" << m_middle << "end:" << m_end;
			QPoint pt_begin = pView->WorldtoScreen(m_begin);
			QPoint pt_middle = pView->WorldtoScreen(m_middle);
			QPoint pt_end = pView->WorldtoScreen(m_end);

			entities.push_back(new MLine(m_begin, m_middle));
			entities.push_back(new MLine(m_end, m_middle));

			if (m_annoData.weldingType == FilletWeld) // 角焊
			{
				qreal signWidth = 10;
				qreal signHeight = 10;
				QPointF signP1, signP2, signP3;
				if (m_begin.y() > m_middle.y()) signHeight = -signHeight;
				if (pt_middle.x() > pt_end.x()) signWidth = -signWidth;
				qreal signOffset = signWidth * 1.1;
				signP1 = QPointF(m_middle.x() + signOffset, m_middle.y());
				signP2 = QPointF(m_middle.x() + signOffset, m_end.y() + signHeight);
				signP3 = QPointF(m_middle.x() + signOffset + signWidth, m_middle.y());
				entities.push_back(new MLine(signP1, signP2));
				entities.push_back(new MLine(signP1, signP3));
				entities.push_back(new MLine(signP2, signP3));
				qDebug() << "middle.x" << m_middle.x() << "width:" << signWidth << "P3.x" << signP3.x();
			}
			else if (m_annoData.weldingType == IWeld) // I型焊
			{
				qreal signDistance = 7;
				qreal signHeight = 10;
				if (m_begin.y() > m_middle.y()) signHeight = -signHeight;
				if (m_middle.x() > m_end.x()) signDistance = -signDistance;
				qreal signOffset = signDistance * 1.1;
				QPointF signP1, signP2, signP3, signP4;
				signP1 = QPointF(m_middle.x() + signOffset, m_middle.y() + signHeight);
				signP2 = QPointF(m_middle.x() + signOffset, m_end.y());
				signP3 = QPointF(m_middle.x() + signOffset + signDistance, m_middle.y());
				signP4 = QPointF(m_middle.x() + signOffset + signDistance, m_middle.y() + signHeight);
				entities.push_back(new MLine(signP1, signP2));
				entities.push_back(new MLine(signP3, signP4));
			}
			else if (m_annoData.weldingType == VWeld) // V型焊
			{
				qreal signHeight = 10;
				qreal signOffset = signHeight * 1.1;
				QPointF signP1, signP2, signP3;
				if (m_begin.y() > m_middle.y()) signHeight = -signHeight;
				if (m_middle.x() <= m_end.x()) // 在左侧
				{
					signP1 = QPointF(m_middle.x() + signOffset - signHeight / tan(PI / 3), m_middle.y() + signHeight);
					signP2 = QPointF(m_middle.x() + signOffset, m_end.y());
					signP3 = QPointF(m_middle.x() + signOffset + signHeight / tan(PI / 3), m_middle.y() + signHeight);
				}
				else // 在右侧
				{
					signP1 = QPointF(m_middle.x() - signOffset + signHeight / tan(PI / 3), m_middle.y() + signHeight);
					signP2 = QPointF(m_middle.x() - signOffset, m_end.y());
					signP3 = QPointF(m_middle.x() - signOffset - signHeight / tan(PI / 3), m_middle.y() + signHeight);
				}
				entities.push_back(new MLine(signP1, signP2));
				entities.push_back(new MLine(signP2, signP3));
			}
			else if (m_annoData.weldingType == PlugWeld) // 塞焊
			{
				qreal signDistance = 12;
				qreal signHeight = 7;
				if (m_begin.y() > m_middle.y()) signHeight = -signHeight;
				if (m_middle.x() > m_end.x()) signDistance = -signDistance;
				qreal signOffset = signDistance * 1.1;
				QPointF signP1, signP2, signP3, signP4;
				signP1 = QPointF(m_middle.x() + signOffset, m_middle.y() + signHeight);
				signP2 = QPointF(m_middle.x() + signOffset, m_end.y());
				signP3 = QPointF(m_middle.x() + signOffset + signDistance, m_middle.y());
				signP4 = QPointF(m_middle.x() + signOffset + signDistance, m_middle.y() + signHeight);
				entities.push_back(new MLine(signP1, signP2));
				entities.push_back(new MLine(signP1, signP4));
				entities.push_back(new MLine(signP3, signP4));
			}
			else if (m_annoData.weldingType == SpotWeld) // 点焊
			{
				qreal signHeight = 13;
				qreal signOffset = signHeight * 1.1;
				if (m_begin.y() > m_middle.y()) signHeight = -signHeight;
				if (m_middle.x() > m_end.x()) signOffset = -signOffset;
				QPointF signP1 = QPointF(m_middle.x() + signOffset, m_middle.y() + signHeight / 2);
				entities.push_back(new CCircle(signP1, signHeight / 2));
			}

			// 绘制基准线
			qreal Distance = m_end.x() - m_middle.x();
			qreal high = 10;
			if (m_begin.y() > m_middle.y()) high = -high;
			QPointF P1, P2, P3, P4, P5, P6;
			P1 = QPointF(m_middle.x(), m_middle.y() - high);
			P2 = QPointF(P1.x() + Distance / 3, m_middle.y() - high);
			P3 = QPointF(P2.x() + Distance / 12, m_middle.y() - high);
			P4 = QPointF(P3.x() + Distance / 3, m_middle.y() - high);
			P5 = QPointF(P4.x() + Distance / 12, m_middle.y() - high);
			P6 = QPointF(m_end.x(), m_middle.y() - high);
			entities.push_back(new MLine(P1, P2));
			entities.push_back(new MLine(P3, P4));
			entities.push_back(new MLine(P5, P6));
			}
	} // end of dmNormal

	// 在 dmSelect 或其他模式下，绘制已生成的实体
	foreach(MEntity* entity, entities) {
		if (entity) {
			if (drawMode == dmSelect) {
				// 为选中状态设置特殊画笔
				QPen selectPen(Qt::DashLine);
				selectPen.setColor(Qt::green);
				selectPen.setWidth(1);
				QBrush selectBrush; // 通常选中状态不填充
				entity->SetAttrib(&selectPen, &selectBrush);
			}
			else {
				// 使用对象自身的画笔画刷
				entity->SetAttrib(&m_pen, &m_brush);
			}
			entity->Draw(pView, drawMode);
		}
	}

	// dmInvalid 模式处理（恢复初始值）
	if (drawMode == dmInvalid) {
		Init(); // 调用 Init 恢复所有初始值
	}
}

bool MAnnotation::Pick(const QPointF& pos, const double pick_radius)
{
	foreach (MEntity* entity, entities) {
		if (entity->Pick(pos, pick_radius)) {
			return true;
		}
	}
	return false;
}

void  MAnnotation::GetBox(QRectF& pBox)
{
	double min0 = 1E20;
	double min1 = 1E20;
	double max0 = -1E20;
	double max1 = -1E20;

	QVector<QPointF> pPositions;
	pPositions.push_back(m_begin);
	pPositions.push_back(m_end);
	pPositions.push_back(m_middle); // 包含中点

	int m_numpts = pPositions.size();
	for (int i = 0; i < m_numpts; i++) {
		min0 = std::min(min0, pPositions[i].x());
		min1 = std::min(min1, pPositions[i].y());
		max0 = std::max(max0, pPositions[i].x());
		max1 = std::max(max1, pPositions[i].y());
	}

	// 也考虑构成标注的子实体的包围盒
	QRectF subBox;
	for (MEntity* entity : entities) {
		if (entity) {
			entity->GetBox(subBox);
			min0 = std::min(min0, subBox.left());
			min1 = std::min(min1, subBox.top());
			max0 = std::max(max0, subBox.right());
			max1 = std::max(max1, subBox.bottom());
		}
	}

	pBox.setLeft(min0);
	pBox.setTop(min1); // Qt坐标系，Top对应Y的最小值
	pBox.setRight(max0);
	pBox.setBottom(max1); // Qt坐标系，Bottom对应Y的最大值
}

bool  MAnnotation::IsInRect(const QPointF& pos, const QPointF& pos1)
{
	// 获取当前对象的包围盒
	QRectF sourceBox;
	GetBox(sourceBox);

	// 创建选择框
	double Left = std::min(pos.x(), pos1.x());
	double Top = std::min(pos.y(), pos1.y());
	double Right = std::max(pos.x(), pos1.x());
	double Bottom = std::max(pos.y(), pos1.y());

	QRectF selectionBox(Left, Top, Right - Left, Bottom - Top);

	// 检查包围盒是否与选择框相交
	return sourceBox.intersects(selectionBox);
}

void MAnnotation::Serialize(QDataStream& ar, bool bSave)
{
	MEntity::Serialize(ar, bSave);
	if (bSave) {
		// Saving
		ar << m_begin;
		ar << m_middle;
		ar << m_end;
		ar << static_cast<int>(m_annoData.type);
		ar << m_annoData.roughnessMin;
		ar << m_annoData.roughnessMax;
		ar << m_annoData.obtainMethod;
		ar << m_annoData.standardName;
		ar << static_cast<int>(m_annoData.weldingType);
		ar << m_annoData.weldingName;
		ar << m_font;
	}
	else {
		// Loading
		ar >> m_begin;
		ar >> m_middle;
		ar >> m_end;
		int typeInt;
		ar >> typeInt;
		m_annoData.type = static_cast<AnnotationType>(typeInt);
		ar >> m_annoData.roughnessMin;
		ar >> m_annoData.roughnessMax;
		ar >> m_annoData.obtainMethod;
		ar >> m_annoData.standardName;
		int weldingTypeInt;
		ar >> weldingTypeInt;
		m_annoData.weldingType = static_cast<WeldingType>(weldingTypeInt);
		ar >> m_annoData.weldingName;
		ar >> m_font;
	}
}