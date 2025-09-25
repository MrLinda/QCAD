#include "COMMAND.H"
#include "CCommandQrue.h"

CCommandQrue::CCommandQrue()
{
	m_curPos = 0;
}

CCommandQrue::~CCommandQrue()
{
	DeleteAll();
}

//每次操作完成后调用该函数
void CCommandQrue::AddCommands(CCommand* pCmd)
{
	assert(pCmd);

	AdjustCommands();
	m_vCommands.push_back(pCmd);
	m_curPos++;
}

//每次操作之前调用该函数，调整序列，删去可能的内存
void CCommandQrue::AdjustCommands()
{
	if (m_curPos >= m_vCommands.count())
	{
		return;
	}

	//倒序，逐个检查pcmd，如果是创建，则清楚内存；并删去该pCmd
	for (int i = m_vCommands.count(); i > m_curPos ; i--)
	{
		CCommand* pCmd = m_vCommands.at(i);
		//如果是创建
		//if (pCmd->IsCreateCmd())
		//{
		//	QList<QGraphicsItem*> lstItems = pCmd->GetItems();
		//	foreach(QGraphicsItem * pItem, lstItems)
		//	{
		//		delete pItem;
		//	}
		//}
		m_vCommands.remove(i);
		delete pCmd;
	}
}

//调用当前的curPos的操作，执行其 Undo
void CCommandQrue::Undo()
{
	CCommand* pCurCmd = m_vCommands.at(m_curPos);

	//pCurCmd->Undo();
	m_curPos--;
}

/*
*  命令操作类型      Undo                        Redo
*  创建实体          移除实体出显示列表          将实体再加入显示列表
*  移动/旋转/镜像    反向操作，回原来位置        执行操作（需记录操作参数和实体集）
*  删除              将实体再加入显示            移除实体出显示列表
*  其他（如字体、线型、颜色）  都需要定义其操作命令，参数和实体集，对应实现Undo/redo
*/
void CCommandQrue::Redo()
{
	CCommand* pCurCmd = m_vCommands.at(m_curPos+1);

	//pCurCmd->Redo();
	m_curPos++;
}

//创建的实体，在最后被删除，过程中除非调整，否则为了Undo/Redo操作，不删除实体
void CCommandQrue::DeleteAll()
{
	foreach(CCommand * pCmd, m_vCommands)
	{
		//如果是创建
		//if (pCmd->IsCreateCmd())
		//{
		//	QList<QGraphicsItem*> lstItems = pCmd->GetItems();
		//	foreach(QGraphicsItem * pItem, lstItems)
		//	{
		//		delete pItem;
		//	}
		//}
		delete pCmd;
	}

	m_vCommands.clear();
}
