#include "math.h"
#include "Entity.h"
#include <QDebug>

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
			QPointF textLowerRightBottom;
			QPointF textUpperRightBottom;
			if (m_annoData.obtainMethod == 0 || m_annoData.obtainMethod == 1)
			{
				// 未注或去除材料
				textLowerRightBottom = (signP1 + 3 * (signP2 + signP3) / 2) / 4 + QPointF(signHeight / tan(PI/3), signHeight * 0.1);
				textUpperRightBottom = textLowerRightBottom + QPointF(0, textHeight);
			}
			else if (m_annoData.obtainMethod == 2)
			{
				// 不去除材料
				textLowerRightBottom = circleCenter + QPointF(signHeight / tan(PI / 3), radius + signHeight * 0.1);
				textUpperRightBottom = textLowerRightBottom + QPointF(0, textHeight);
			}
			MText* pTextMin = new MText();
			MText* pTextMax = new MText();
			pTextMin->SetFont(realFont);
			pTextMax->SetFont(realFont);
			if(!m_annoData.roughnessMin.isEmpty() && !m_annoData.roughnessMax.isEmpty())
			{
				// 最小值和最大值都不为空
				int textWidth = metrics.horizontalAdvance(m_annoData.roughnessMin) / scale;
				pTextMin->SetText(m_annoData.roughnessMin);
				pTextMin->SetRightBottomPos(textLowerRightBottom);
				pTextMin->SetLeftTopPos(QPointF(textLowerRightBottom.x() - textWidth, textLowerRightBottom.y() + textHeight));
				

				textWidth = metrics.horizontalAdvance(m_annoData.roughnessMax) / scale;
				pTextMax->SetText(m_annoData.roughnessMax);
				pTextMax->SetRightBottomPos(textUpperRightBottom);
				pTextMax->SetLeftTopPos(QPointF(textUpperRightBottom.x() - textWidth, textUpperRightBottom.y() + textHeight));

				entities.push_back(pTextMin);
				entities.push_back(pTextMax);
			}
			else if (!m_annoData.roughnessMin.isEmpty())
			{
				// 只有最小值不为空
				int textWidth = metrics.horizontalAdvance(m_annoData.roughnessMin) / scale;
				pTextMin->SetText(m_annoData.roughnessMin);
				pTextMin->SetRightBottomPos(textLowerRightBottom);
				pTextMin->SetLeftTopPos(QPointF(textLowerRightBottom.x() - textWidth, textLowerRightBottom.y() + textHeight));

				entities.push_back(pTextMin);
			}
			else if (!m_annoData.roughnessMax.isEmpty())
			{
				// 只有最大值不为空
				int textWidth = metrics.horizontalAdvance(m_annoData.roughnessMax) / scale;
				pTextMax->SetText(m_annoData.roughnessMax);
				pTextMax->SetRightBottomPos(textUpperRightBottom);
				pTextMax->SetLeftTopPos(QPointF(textUpperRightBottom.x() - textWidth, textUpperRightBottom.y() + textHeight));

				entities.push_back(pTextMax);
			}
			qDebug() << "begin:" << m_begin << "middle:" << m_middle << "end:" << m_end;
			qDebug() << "signP2:" << signP2;
		}
		else if (m_annoData.type == atStandard)
		{
			// 画基准标注
		}
		else if (m_annoData.type == atWelding)
		{
			// 画焊接标注
		}
		foreach (MEntity* entity, entities) {
			entity->SetAttrib(&m_pen, &m_brush);
			entity->Draw(pView, drawMode);
		}
	}
	else if (drawMode == dmSelect)
	{
		foreach (MEntity* entity, entities) {
		entity->Draw(pView, drawMode);
		}
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