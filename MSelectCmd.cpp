#include "MSelectCmd.h"

#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"

#include <QMouseEvent>
#include <QRubberband>

MSelectCmd::MSelectCmd(QCADView* pDiagramScene)
	: m_begin(0, 0), m_end(0, 0)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDC = pDiagramScene;

	rubberband = NULL;
}

MSelectCmd::~MSelectCmd()
{
	delete rubberband;
}

int MSelectCmd::GetType()
{
	return ctSelect;
}

//如果点选到实体就结束
//否则，拉开矩形选择。。。
int	MSelectCmd::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	//开始要处理是否有shift键按下，？？？否则清空选择集
	m_pDC->ClearSelections();

	m_nStep++; // 每次单击鼠标左键时操作步加 1 

	switch (m_nStep) // 根据操作步执行相应的操作
	{
	case 1:
	{
		m_begin = m_end = mouseEvent->pos();

		//检查有没有选中实体
		bool bSelected = false;
		QList<MEntity*> lstEntitys = m_pDC->GetEntityList();
		foreach(MEntity * pEnt, lstEntitys)
		{
			QPointF pickPt = m_pDC->ScreentoWorld(m_begin);
			if (pEnt->Pick(pickPt, PICK_RADIUS))
			{
				m_pDC->AddSelection(pEnt);
				bSelected = true;
			}
		}
		if (bSelected)
		{
			m_pDC->update(); //刷新显示选择集

			m_nStep = 0;
			break;
		}
		
		rubberband = new QRubberBand(QRubberBand::Line, m_pDC);
		QString sPrmt = QStringLiteral("请选择框的下一点：");
		Prompt(sPrmt);
		break;
	}
	case 2:
	{
		m_end = mouseEvent->pos();

		QList<MEntity*> lstEntitys = m_pDC->GetEntityList();
		foreach(MEntity * pEnt, lstEntitys)
		{
			QPointF pickP1 = m_pDC->ScreentoWorld(m_begin);
			QPointF pickP2 = m_pDC->ScreentoWorld(m_end);
			//此函数需要实现.......
			if (0) //pEnt->IsInRect(m_begin, m_end))
			{
				m_pDC->AddSelection(pEnt);
			}
		}
		if (rubberband)
		{
			rubberband->hide();
			delete rubberband;
			rubberband = NULL;
		}
		m_pDC->update();

		m_nStep = 0;
		break;
	}
	default:
		break;
	}

	return 0;
}

//鼠标左键双击创建多线使用，此处只创建两点直线
int MSelectCmd::OnLButtonDblClk(QMouseEvent* mouseEvent)
{
	
	return 0;
}

int	MSelectCmd::OnMouseMove(QMouseEvent* mouseEvent)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_LINE));
	QPoint curPos = mouseEvent->pos();
	QPointF  wldPos = m_pDC->ScreentoWorld(curPos);

	MCommand::OnMouseMove(mouseEvent);

	switch (m_nStep)
	{
	case 0:
		::Prompt(QString::asprintf("Select: %6.3f,%6.3f", wldPos.x(), wldPos.y()));
		break;
	default:
	{
		int aLeft = m_begin.x();
		int aTop = m_begin.y();
		int aWide = curPos.x() - m_begin.x();
		int aHeight = curPos.y() - m_begin.y();
		QRect rect(aLeft, aTop, aWide, aHeight);

		if (rubberband)
		{
			rubberband->setGeometry(rect);
			rubberband->show();
		}
		break;
	}
	}

	return 0;
}

// 单击鼠标右键取消当前的操作
int	MSelectCmd::OnRButtonDown(QMouseEvent* mouseEvent)
{
	Cancel();
	//// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线

	return 0;
}
// 调用Cancel 函数取消本次操作
int MSelectCmd::Cancel()
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if (m_nStep == 1) {
		//m_pDiagramScene->removeItem(m_pLine);
	}

	m_nStep = 0; // 将操作步重置为 0 
	return 0;
}

