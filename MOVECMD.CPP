#include "math.h"
#include "ModifyCmd.h"
#include "diagramscene.h"

#include <QGraphicsLineItem>

CMove::CMove(DiagramScene* pScene)
	: m_basePos(0,0), m_desPos(0,0)
{
	m_nStep = 0;// 初始化操作步为 0
	m_pDiagramScene = pScene;

	m_pSelItem = NULL;
	if(m_pDiagramScene->selectedItems().count() > 0)
		m_pSelItem = m_pDiagramScene->selectedItems().first();
}

CMove::~CMove()
{
}
int	CMove::GetType()
{
	return ctMove;
}

int	CMove::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	m_nStep ++ ;
	switch(m_nStep)
	{
		case 1:
		{
			m_pTempLine = new QGraphicsLineItem;
			// 第一次单击鼠标左键时得到基点位置，并初步设定目标位置
			m_basePos = m_desPos = tmpPoint = mouseEvent->scenePos();
			QLineF line(m_basePos, m_desPos);
			m_pTempLine->setLine(line); 
			m_pDiagramScene->addItem(m_pTempLine);

			Prompt(QStringLiteral("请输入移动的目标点：单击鼠标右键取消"));
			break;
		}
		case 2:
		{
			Prompt(QStringLiteral("请输入移动的起始点："));
			m_pDiagramScene->removeItem(m_pTempLine);
			delete m_pTempLine;

			if (m_pSelItem)
			{
				m_pSelItem->setSelected(true);
				m_pSelItem->setFocus();
			}
	
			m_nStep = 0; 
			break;
		}
		default:
			break;
	}
	
	return 0;
}

int	CMove::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	
	//if (m_pDiagramScene->selectedItems().count() == 0)
	//	return CCommand::OnMouseMove(mouseEvent);

	//QGraphicsItem* pItemSel = m_pDiagramScene->selectedItems().first();
	if (m_pSelItem == NULL)
	{
		return CCommand::OnMouseMove(mouseEvent);
	}

	switch(m_nStep)
	{
		case 0:
		{
			Prompt(QStringLiteral("请输入移动的起始点："));
			break;
		}
		case 1:
		{
			Prompt(QStringLiteral("请输入移动的终点："));
			m_desPos = mouseEvent->scenePos();
			QLineF line(m_basePos, m_desPos);
			m_pTempLine->setLine(line); 

			qreal dx = m_desPos.x() - tmpPoint.x();
			qreal dy = m_desPos.y() - tmpPoint.y();
			m_pSelItem->moveBy(dx, dy);

			QTransform trans1;
			trans1.translate(dx, dy);
			//m_pSelItem->setTransform(trans1);
			tmpPoint = m_desPos;
		}
		default:
			break;
	}
	return 0;
}
// 单击鼠标右键取消正在进行的操作
int	CMove::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	if(m_nStep == 1){
		
	}

	m_nStep = 0;
	Prompt(QStringLiteral("请输入移动的起始点："));
	m_pDiagramScene->ResetSelectStatus();

	return 0;
}
// 调用Cancel 函数取消本次操作
int CMove::Cancel()
{
	if(m_nStep == 1){
		;
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("就绪")); // 等待提示新类型的命令操作
	return 0;
}

