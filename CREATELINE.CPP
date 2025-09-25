#include "math.h"
#include "Command.h"
#include "CreateCmd.h"

#include "diagramscene.h"
#include <QGraphicsLineItem>

CCreateLine::CCreateLine(DiagramScene* pDiagramScene)
	: m_begin(0,0), m_end(0,0)
{
	m_nStep = 0; // 初始化操作步为 0
	m_pDiagramScene = pDiagramScene;
}

CCreateLine::~CCreateLine()
{
	m_pPositions.clear();
}

int CCreateLine::GetType()
{
	return ctCreateLine; 
}

int	CCreateLine::OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	if (mouseEvent->button() != Qt::LeftButton)
		return 0;

	QPen pen(m_pDiagramScene->penStyle());   
	pen.setColor(m_pDiagramScene->lineColor());  
	pen.setWidthF(m_pDiagramScene->penWidth());

	m_nStep ++; // 每次单击鼠标左键时操作步加 1 

	switch(m_nStep) // 根据操作步执行相应的操作
	{
		case 1: 
		{
			m_begin = m_end = mouseEvent->scenePos();
			m_pLine = new QGraphicsLineItem(QLineF(m_begin, m_end));
			m_pLine->setPen(pen);
			m_pDiagramScene->addItem(m_pLine);

			QString sPrmt = QStringLiteral("请输入直线的下一点：");
			Prompt(sPrmt);
			break;
		}
		case 2:
		{
			m_end = mouseEvent->scenePos();
			QLineF newLine(m_pLine->line().p1(), m_end);
			m_pLine->setLine(newLine);
			m_nStep = 0;

			//// 如果在按鼠标左键的过程中同时按下了Shift键，
			////	那么根据鼠标单击位置绘制水平线或竖直线
			//if( nFlags & MK_SHIFT ){ 
			//	double dx = pos.x - m_begin.x;
			//	double dy = pos.y - m_begin.y;
			//	if(fabs(dx) <= fabs(dy)) // 如果鼠标单击位置在X方向靠近起点
			//		m_end.Set(m_begin.x, pos.y); // 那么终点的x坐标与起点的相同
			//	else
			//		m_end.Set(pos.x,m_begin.y);
			//}
			//else if (m_bCaptured)
			//{
			//	m_end = m_ptCapture;
			//}
			//else {
			//	m_end = pos; // 如果未按下Shift键, 则终点为鼠标单击位置			
			//}
			break;
		}	
		default:
			break;
	}

	m_pPositions.push_back(m_end);

	return 0;
}

//鼠标左键双击创建多线
int CCreateLine::OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent)
{
	//// 擦除在拖动状态时显示的最后一条线
	//MLines*	pTempLine = new MLines(m_begin,m_end); 
	//pTempLine->Draw(pDC, dmDrag);
	//delete pTempLine;
	//
	//// 如果在按鼠标左键的过程中同时按下了Shift键，
	////	那么根据鼠标单击位置绘制水平线或竖直线
	//if( nFlags & MK_SHIFT ){ 
	//	double dx = pos.x - m_begin.x;
	//	double dy = pos.y - m_begin.y;
	//	if(fabs(dx) <= fabs(dy)) // 如果鼠标单击位置在X方向靠近起点
	//		m_end.Set(m_begin.x, pos.y); // 那么终点的x坐标与起点的相同
	//	else
	//		m_end.Set(pos.x,m_begin.y);
	//}
	//else
	//	m_end = pos; // 如果未按下Shift键, 则终点为鼠标单击位置			
	//
	//if(m_nStep >= 2) {
	//	MLines*	pNewLine = new MLines(m_pPositions);// 根据起点和终点创建直线
	//	pNewLine->Draw(pDC,dmNormal); // 绘制直线
	//	g_pDoc->m_EntityList.AddTail(pNewLine); // 将直线指针添加到图元链表
	//	g_pDoc->SetModifiedFlag(TRUE);// set modified flag ;
	//	pNewLine->m_nOperationNum = g_pView->m_nCurrentOperation;
	//	m_pPositions.clear();
	//}
	//
	//g_pView->ReleaseDC(pDC); // 释放设备环境指针
	//
	//m_nStep = 0;  // 将操作步重置为 0
	//Prompt(QStringLiteral(""请输入直线的起点：");
	
	return 0;
}

int	CCreateLine::OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent)
{
	//::SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW_LINE));

	CCommand::OnMouseMove(mouseEvent);

	switch(m_nStep)
	{
		case 0:
			Prompt(QStringLiteral("请输入直线的起点："));
			break;
		default:
		{
			//// 如果在按鼠标左键的过程中同时按下了Shift键，
			////	那么根据鼠标单击位置绘制水平线或竖直线
			//if( nFlags & MK_SHIFT ){
			//	double dx = pos.x - m_begin.x;
			//	double dy = pos.y - m_begin.y;
			//	if(fabs(dx)>=fabs(dy))
			//		curPos.Set(pos.x,m_begin.y);
			//	else
			//		curPos.Set(m_begin.x, pos.y);
			//}
			//else if (m_bCaptured)
			//{
			//	curPos = m_ptCapture;
			//}
			//else {
			//	curPos = pos; // 如果未按下Shift键, 则终点为鼠标单击位置			
			//}
			QLineF newLine(m_pLine->line().p1(), mouseEvent->scenePos());
			m_pLine->setLine(newLine);

			break;
		}
	}

	return 0;
}

// 单击鼠标右键取消当前的操作
int	CCreateLine::OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) 
{
	Cancel();
	//// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	//if(m_nStep == 1){ 
	//	CDC*	pDC = g_pView->GetDC(); // 得到设备环境指针
	//	QPointF	prePos = m_end; // 获得鼠标所在的前一个位置
	//	MLines*	pTempLine = new MLines(m_begin, prePos); 
	//	pTempLine->Draw(pDC, dmDrag); // 擦除上一次绘制的橡皮线
	//	delete pTempLine;
	//	g_pView->ReleaseDC(pDC); // 释放设备环境指针
	//}
	//m_nStep = 0; // 将操作步重置为 0 
	//Prompt(QStringLiteral(""请输入直线的起点：");
	return 0;
}
// 调用Cancel 函数取消本次操作
int CCreateLine::Cancel()
{
	// 如果当前的操作步为 1 ，那么要在结束本次操作前擦除上次鼠标移动时绘制的橡皮线
	if(m_nStep == 1){ 
		m_pDiagramScene->removeItem(m_pLine);
	}

	m_nStep = 0; // 将操作步重置为 0 
	Prompt(QStringLiteral("请输入直线起点...")); // 等待提示新类型的命令操作
	return 0 ;
}

