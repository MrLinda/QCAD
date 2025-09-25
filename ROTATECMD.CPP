#include "math.h"
#include "Command.h"
#include "ModifyCmd.h"
#include "diagramscene.h"

#include <QGraphicsLineItem>

CRotate::CRotate(DiagramScene* pScene)
	: m_basePos(0,0), m_desPos(0,0)
{
	m_nStep = 0;// 初始化操作步为 0
	m_pDiagramScene = pScene;

	m_pSelItem = NULL;
	if (m_pDiagramScene->selectedItems().count() > 0)
		m_pSelItem = m_pDiagramScene->selectedItems().first();
}

CRotate::~CRotate()
{
}

int	CRotate::GetType()
{
	return ctRotate;
}

int	CRotate::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	m_nStep ++ ;
	switch(m_nStep)
	{
		case 1:
		{
			// 第一次单击鼠标左键时得到基点位置，并初步设定目标位置
			m_basePos = m_desPos = mouseEvent->scenePos();
			Prompt(QStringLiteral("请输入第二点：单击鼠标右键取消"));
			
			m_pTempLine = new QGraphicsLineItem;
			// 第一次单击鼠标左键时得到基点位置，并初步设定目标位置
			m_basePos = m_desPos = mouseEvent->scenePos();
			QLineF line(m_basePos, m_desPos);
			m_pTempLine->setLine(line);
			m_pDiagramScene->addItem(m_pTempLine);

			break;
		}
		case 2:
		{
			m_pDiagramScene->removeItem(m_pTempLine);
			delete m_pTempLine;

			m_nStep = 0; 
			break;
		}
		default:
			break;
	}
	return 0;
}

int	CRotate::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	switch(m_nStep)
	{
		case 0:
			Prompt(QStringLiteral("请输入旋转的基点：")) ;
			break;
		case 1:
		{
			Prompt(QStringLiteral("请输入旋转的终点："));

			//QPointF	prePos, curPos;
			m_desPos = mouseEvent->scenePos();
			QLineF line(m_basePos, m_desPos);
			m_pTempLine->setLine(line);

			//通过三次复合变换，实现绕任意点旋转
			QTransform trans1, trans2, trans3;
			
			qreal delX = m_desPos.x() - m_basePos.x();
			qreal delY = m_desPos.y() - m_basePos.y();
			qreal r = sqrt(delX * delX + delY * delY);
			qreal endAng = acos(delX / r);
			trans1.translate(-m_basePos.x(), -m_basePos.y());
			trans2.rotate(endAng*57);
			trans3.translate(m_basePos.x(), m_basePos.y());
			m_pSelItem->setTransform(trans1 * trans2*trans3);	
			break;
		}
		default:
			break;
	}
	return 0;
}
// 单击鼠标右键取消正在进行的操作
int	CRotate::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	QPointF	prePos = m_desPos; // 得到上一个鼠标位置
	if(m_nStep == 1){
		;
	}

	m_nStep = 0;
	Prompt(QStringLiteral("请输入旋转的基点："));
	m_pDiagramScene->ResetSelectStatus();

	return 0;
}
// 调用Cancel 函数取消本次操作
int CRotate::Cancel()
{
	QPointF	prePos = m_desPos; // 得到上一个鼠标位置
	if(m_nStep == 1){
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作

	return 0;
}

