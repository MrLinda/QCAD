#include "MCreateEllipse.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"  // 包含 MEllipse 的定义

#include <QMouseEvent>

MCreateEllipse::MCreateEllipse(QCADView* pDiagramScene)
	: m_LeftTop(0, 0), m_RightBottom(0, 0), m_pEllipse(nullptr)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDC = pDiagramScene;
	//m_pEllipse = nullptr; // 确保 m_pEllipse 初始化为 nullptr
}

MCreateEllipse::~MCreateEllipse()
{
	delete m_pEllipse;
	/*// 确保析构时释放未完成的椭圆对象
	if (m_pEllipse != nullptr) {
		delete m_pEllipse;
		m_pEllipse = nullptr;
	}*/
}

int MCreateEllipse::GetType()
{
	return ctCreateEllipse; // 自定义的类型，用于标识绘制椭圆
}

int MCreateEllipse::OnLButtonDown(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	// 检查 m_pDC 是否已初始化，防止空指针
	if (m_pDC == nullptr) {
		return 0;  // 如果绘图上下文为空，提前返回
	}

	QPen pen(m_pDC->penStyle());
	pen.setColor(m_pDC->lineColor());
	pen.setWidthF(m_pDC->penWidth());

	QBrush brush(m_pDC->brushColor());

	m_nStep++; // 每次单击鼠标左键时操作步加 1 

	switch (m_nStep)
	{
	case 1:
	{
		// 第一次单击，记录椭圆的左上角点
		m_LeftTop = m_pDC->ScreentoWorld(mouseEvent->pos());
		m_RightBottom = m_LeftTop; // 初始时右下角点与左上角点相同

		//QBrush brush(m_pDC->brushColor());

		// 创建椭圆实体
		m_pEllipse = new MEllipse(m_LeftTop, m_RightBottom);
		m_pEllipse->SetPen(pen);
		m_pEllipse->SetBrush(brush);
		//QBrush brush(m_pDC->brushColor());  // 使用 QCADView 的 brushColor 初始化画刷
		m_pEllipse->SetAttrib(&pen, &brush); // 设置椭圆的属性
		m_pDC->addEntity(m_pEllipse); // 将椭圆添加到场景

		QString sPrmt = QStringLiteral("请输入椭圆的右下角点：");
		Prompt(sPrmt);
		break;
	}
	case 2:
	{
		if (m_pEllipse == nullptr) {
			// 防止空指针访问
			return 0;
		}

		// 第二次单击，确定右下角点，完成椭圆的绘制
		m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

		// 更新椭圆的右下角点并刷新场景
		//m_pEllipse->Move(m_LeftTop, m_RightBottom);
		//m_pDC->update();
		//m_pDC->repaint();

		// 提示绘制完成
		Prompt(QStringLiteral("椭圆绘制完成"));

		m_nStep = 0; // 完成椭圆绘制后重置步骤

		// 清除椭圆对象，以便后续操作
		//delete m_pEllipse;  // 确保释放内存
		//m_pEllipse = nullptr;
		break;
	}
	default:
		break;
	}

	return 0;
}

int MCreateEllipse::OnMouseMove(QMouseEvent* mouseEvent)
{
	MCommand::OnMouseMove(mouseEvent);

	if (m_nStep == 1 && m_pEllipse != nullptr)
	{
		// 当用户移动鼠标时，更新右下角点
		m_RightBottom = m_pDC->ScreentoWorld(mouseEvent->pos());

		// 动态更新椭圆的形状
		m_pEllipse->Move(m_LeftTop, m_RightBottom);
		m_pDC->update();
	}

	return 0;
}

int MCreateEllipse::OnRButtonDown(QMouseEvent* mouseEvent)
{
	// 右键点击取消当前操作
	Cancel();
	return 0;
}

int MCreateEllipse::Cancel()
{
	if (m_nStep == 1 && m_pEllipse != nullptr) {
		// 在取消时，可以删除未完成的椭圆
		m_pDC->removeLastEntity();  // 移除最后一个实体
		m_pEllipse = nullptr;  // 避免再次访问空指针
	}

	m_nStep = 0;
	Prompt(QStringLiteral("请输入椭圆的左上角点...")); // 提示新的操作
	return 0;
}