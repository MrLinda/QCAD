#include "MMoveCmd.h"

#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"

#include <QMouseEvent>
#include <QRubberband>

MMoveCmd::MMoveCmd(QCADView* pDiagramScene)
	: m_begin(0, 0), m_end(0, 0)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDC = pDiagramScene;
	m_promptPrefix = "请指定移动起点：";

	m_pTmpLine = nullptr;
}

MMoveCmd::~MMoveCmd()
{
	if (m_pTmpLine != nullptr)
	{
		delete m_pTmpLine;
		m_pTmpLine = nullptr;
	}
}

int MMoveCmd::GetType()
{
	return ctMove;
}

int MMoveCmd::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	m_nStep++; // 每次单击鼠标左键时操作步加 1

	switch (m_nStep) // 根据操作步执行相应的操作
	{
	case 1:
	{
		if (m_pDC->GetSelectedEntityList().count() == 0)
		{
			Prompt("pls select entity");
			m_nStep = 0;
			return 0;
		}

		m_begin = m_end = m_pDC->ScreentoWorld(mouseEvent->pos());
		m_pTmpLine = new MLine(m_begin, m_end);
		m_pDC->addEntity(m_pTmpLine);

		m_promptPrefix = "请指定移动终点：";
		break;
	}
	case 2:
	{
		m_promptPrefix = "请指定移动起点：";
		m_pDC->removeLastEntity();
		delete m_pTmpLine;
		m_pTmpLine = nullptr;
		m_nStep = 0;

		break;
	}
	}

	m_pDC->update();
	return 0;
}

int MMoveCmd::OnRButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::RightButton)
		return 0;

	Cancel();
	return 0;
}

int MMoveCmd::OnLButtonDblClk(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	//Cancel();
	return 0;
}

int MMoveCmd::OnMouseMove(QMouseEvent* mouseEvent)
{
	auto pos = m_pDC->ScreentoWorld(mouseEvent->pos());
	auto x = pos.x();
	auto y = pos.y();

	switch (m_nStep)
	{
	case 0:
	{
		m_promptPrefix = "请指定移动起点：";
		break;
	}
	case 1:
	{
		for (auto pEnt : m_pDC->GetSelectedEntityList())
		{
			pEnt->Move(m_end, m_begin);
		}
		m_end = pos;
		for (auto pEnt : m_pDC->GetSelectedEntityList())
		{
			pEnt->Move(m_begin, m_end);
		}
		m_pTmpLine->SetEndPos(m_end);
		break;
	}
	}

	Prompt(m_promptPrefix + QString::asprintf("(%d, %d)", x, y));
	m_pDC->update();
	return 0;
}

int MMoveCmd::Cancel()
{
	if (m_nStep != 0)
	{
		for (auto pEnt : m_pDC->GetSelectedEntityList())
		{
			pEnt->Move(m_end, m_begin);
		}
		m_pDC->removeLastEntity();
		delete m_pTmpLine;
		m_pTmpLine = nullptr;
	}
	m_nStep = 0; // 将操作步重置为 0
	m_promptPrefix = "请指定移动起点：";
	m_pDC->update();

	return 0;
}