#include "ENTITY.H"
#include "mainwindow.h"
#include "MCreateText.h"
#include "QCADView.h"

#include <QInputDialog>
#include <QMouseEvent>
#include <Windows.h>
#include <QRubberband>

MCreateText::MCreateText(QCADView* pDiagramScene)
	: m_leftTop(0, 0), m_rightBottom(0, 0)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDC = pDiagramScene;
	m_pText = NULL;
	m_promptPrefix = QStringLiteral("请指定文本的左上角点：");

	rubberband = NULL;
}

MCreateText::~MCreateText()
{
}

int MCreateText::GetType()
{
	return ctCreateText;
}

int MCreateText::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	m_nStep++; // 每次单击鼠标左键时操作步加 1 
	switch (m_nStep) // 根据操作步执行相应的操作
	{
		case 1:
		{
			m_leftTop = m_rightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());
			m_promptPrefix = QStringLiteral("请指定文本的右下角点：");

			m_pText = new MText(m_leftTop, m_rightBottom, ""); // 根据两点创建文本
			m_pText->SetFont(m_pDC->font());
			m_pText->SetTextColor(m_pDC->textColor());
			OutputDebugString(L"%%%%%%%% MCreateText.m_pDC->font()\n");
			OutputDebugString(m_pDC->font().family().toStdWString().c_str());
			OutputDebugString(L"\n");
			OutputDebugString(std::to_wstring(m_pDC->font().pointSize()).c_str());
			OutputDebugString(L"\n");
			m_pDC->addEntity(m_pText);

			rubberband = new QRubberBand(QRubberBand::Line, m_pDC);

			break;
		}
		case 2:
		{
			m_rightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

			QInputDialog inputDialog;
			bool isTextReceived = false;
			QString inputText = inputDialog.getMultiLineText(NULL, QStringLiteral("文本编辑"), QStringLiteral("输入要显示的文本："), "", &isTextReceived);
			if (isTextReceived)
			{
				m_pText->SetText(inputText.toStdString().c_str());
				m_pText->SetRightBottomPos(m_rightBottom);
				m_nStep = 0;  // 将操作步重置为 0
				m_promptPrefix = QStringLiteral("请指定文本的左上角点：");
			}
			else
			{
				Cancel();
			}

			if (rubberband)
			{
				rubberband->hide();
				delete rubberband;
				rubberband = NULL;
			}
			break;
		}

	}
	return 0;
}

int MCreateText::OnMouseMove(QMouseEvent* mouseEvent)
{
	MCommand::OnMouseMove(mouseEvent);

	auto pos = mouseEvent->pos();
	if (m_nStep == 1)
	{
		m_pText->SetRightBottomPos(pos);

		if (rubberband)
		{
			QPoint ltPt = m_pDC->WorldtoScreen(m_leftTop);
			int aLeft = ltPt.x();
			int aTop = ltPt.y();
			int aWide = pos.x() - ltPt.x();
			int aHeight = pos.y() - ltPt.y();
			QRect rect(aLeft, aTop, aWide, aHeight);
			rubberband->setGeometry(rect);
			rubberband->show();
		}
		m_pDC->update();
	}
	auto x = pos.x();
	auto y = pos.y();
	Prompt(m_promptPrefix + QString::asprintf("(%d, %d)", x, y));

	return 0;
}

int MCreateText::OnLButtonDblClk(QMouseEvent* mouseEvent)
{
	return 0;
}

int MCreateText::OnRButtonDown(QMouseEvent* mouseEvent)
{
	Cancel();

	return 0;
}

int MCreateText::Cancel()
{
	if (m_nStep != 0)
	{
		m_pDC->removeLastEntity();
		delete m_pText;
		m_pText = NULL;
	}
	m_nStep = 0; // 将操作步重置为 0
	m_promptPrefix = QStringLiteral("请指定文本的左上角点：");
	m_pDC->update();
	return 0;
}