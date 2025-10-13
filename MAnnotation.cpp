#include "math.h"
#include "Entity.h"
#include <QDebug>

#define pointHorizVertTolerance 3 // ����ˮƽ��ֱ�ߵ��ݲ��λ����

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
	m_annoData.roughnessMin = 0;
	m_annoData.roughnessMax = 0;
	m_annoData.obtainMethod = 0;
	m_annoData.standardName = "";
	m_annoData.weldingName = "";
}

MAnnotation::~MAnnotation()
{
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
QPointF MAnnotation::GetStartPos() // �������ֵ
{
	return m_begin;
}
QPointF MAnnotation::GetMiddlePos() // �����м��ֵ
{
	return m_middle;
}
QPointF MAnnotation::GetEndPos() // �����յ�ֵ
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
		QVector<MEntity*> entities;
		if (m_annoData.type == atRoughness)
		{
			qDebug() << "����ֲڶȻ��Ʒ�֧";
			qDebug() << "begin:" << m_begin << "middle:" << m_middle << "end:" << m_end;
			qDebug() << "obtainMethod:" << m_annoData.obtainMethod;

			// ������ֲڶȱ�ע
			QPoint pt_begin = pView->WorldtoScreen(m_begin);
			QPoint pt_middle = pView->WorldtoScreen(m_middle);
			QPoint pt_end = pView->WorldtoScreen(m_end);

			// �Ƿ�ˮƽ��
			bool drawLeader = true;
			// ������
			if (Distance(pt_begin, pt_middle) < pointHorizVertTolerance)
			{
				// ������
				m_middle = m_begin;
				drawLeader = false;
			}
			else
			{
				if (abs(pt_begin.x() - pt_middle.x()) <= pointHorizVertTolerance)
				{
					// ��ֱ��
					m_middle.setX(m_begin.x());
					pt_middle = pView->WorldtoScreen(m_middle);

					entities.push_back(new MLine(m_begin, m_middle));

				}
				else if (abs(pt_begin.y() - pt_middle.y()) <= pointHorizVertTolerance)
				{
					// ˮƽ��
					m_middle.setY(m_begin.y());
					pt_middle = pView->WorldtoScreen(m_middle);

					entities.push_back(new MLine(m_begin, m_middle));
				}
				else
				{
					// ��ͷ
					entities.push_back(new MLine(m_begin, m_middle));
				}
			}
			// ��ˮƽ��
			if(drawLeader)
			{
				entities.push_back(new MLine(m_middle, m_end));
			}
			// ���ֲڶȷ���
			qreal textHeight = 10;					// ����߶�
			qreal signHeight = textHeight * 1.4;	// ���Ÿ߶�
			qreal signOffset = signHeight * 1.1;	// ����ƫ�� 
			QPointF signP1, signP2, signP3;
			if (pt_end.x() > pt_middle.x())
			{
				// �Ҳ�
				signP1 = QPointF(m_middle.x() + signOffset - signHeight / tan(PI / 3), m_middle.y() + signHeight);
				signP2 = QPointF(m_middle.x() + signOffset, m_end.y());
				signP3 = QPointF(m_middle.x() + signOffset + 2 * signHeight / tan(PI / 3), m_middle.y() + 2 * signHeight);
			}
			else
			{
				// ���
				signP1 = QPointF(m_middle.x() - signOffset + signHeight / tan(PI / 3), m_middle.y() + signHeight);
				signP2 = QPointF(m_middle.x() - signOffset, m_middle.y());
				signP3 = QPointF(m_middle.x() - signOffset - 2 * signHeight / tan(PI / 3), m_middle.y() + 2 * signHeight);
			}
			entities.push_back(new MLine(signP1, signP2));
			entities.push_back(new MLine(signP2, signP3));
			if (m_annoData.obtainMethod == 0)
			{
				// δע
			}
			else if (m_annoData.obtainMethod == 1)
			{
				// ȥ������
				entities.push_back(new MLine(signP2, (signP2 + signP3) / 2));
			}
			else if (m_annoData.obtainMethod == 2)
			{
				// ��ȥ������
				QPointF circleCenter = (signP1 + (signP2 + signP3) / 2) / 2 + QPointF(0, signHeight / tan(PI / 3) / tan(PI / 3));
				qreal radius = signHeight / tan(PI / 3) / sin(PI / 3);
				entities.push_back(new CCircle(circleCenter, radius));
			}
		}
		else if (m_annoData.type == atStandard)
		{
			// ����׼��ע
		}
		else if (m_annoData.type == atWelding)
		{
			// �����ӱ�ע
		}
		foreach (MEntity* entity, entities) {
			entity->SetAttrib(&m_pen, &m_brush);
			entity->Draw(pView, drawMode);
			delete entity; // ���꼰ʱ�ͷ�
		}
	}
}