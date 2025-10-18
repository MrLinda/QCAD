#include "QCADView.h"
#include "ENTITY.H"
#include "MCommand.H"
#include "mainwindow.h"
#include "MCreateLine.h"
#include "MCreateRectangle.h"
#include "MCreatePolygon.h"
#include "MCreateArc.h"
#include "MCreateCircle.h"
#include "MCreateEllipse.h"
#include "MCreateText.h"
#include "MCreateAnnotation.h"
#include "MSelectCmd.h"
#include "MMoveCmd.h"
#include "MZoomPan.h"
#include "MZoomWindow.h"

#include <QPainter>
#include <QStatusBar>

QCADView::QCADView()
{
	m_pCmd = NULL;
	m_lineColor = QColor(0, 0, 0);
	m_penStyle = Qt::SolidLine;
	m_penWidth = 1;
	m_brushColor = QColor(255, 255, 255);

	m_scale = 1.0;
	m_Xorg = this->rect().width() / 2;
	m_Yorg = this->rect().height() / 2;
}

QCADView::~QCADView()
{
	//清楚实体链表！！！
	
	//清楚命令，可能重复
	delete m_pCmd;
}

void QCADView::resizeEvent(QResizeEvent* event)
{
	// 获取窗口的宽度和高度
	m_Xorg = this->width()/2;
	m_Yorg = this->height()/2;

	update();
}

void QCADView::paintEvent(QPaintEvent* event)
{
	QPainter dc(this);

	//绘制画布
	/* // 将原点移动到缩放中心点
	dc.translate(m_offsetX, m_offsetY);
	// 设置缩放比例
	dc.scale(m_scale, m_scale);
	dc.translate(-m_offsetX, -m_offsetY);

	dc.translate(Xorg, Yorg);
	*/

	//坐标系统
	dc.setPen(QPen(Qt::black, 1));


	// 获取窗口的宽度和高度
	int width = this->width();
	int height = this->height();

	// 计算中心位置
	int centerX = m_Xorg;// width / 2;
	int centerY = m_Yorg;// height / 2;
	//m_Xorg = centerX;
	//m_Yorg = centerY;

	// 清除背景
	dc.fillRect(event->rect(), Qt::white);

	// 设置坐标轴长度（可以根据需求调整比例）
	int axisLength = std::min(width, height) * 0.2; // 坐标轴长度为窗口宽度或高度的20%

	// 绘制坐标轴
	dc.setPen(Qt::black);
	// 绘制向上的 Y 轴
	dc.drawLine(centerX, centerY, centerX, centerY - axisLength); // Y 轴

	// 绘制向左的 X 轴
	dc.drawLine(centerX, centerY, centerX + axisLength, centerY); // X 轴

	// 绘制原点
	dc.setBrush(Qt::red);
	dc.drawEllipse(centerX - 5, centerY - 5, 10, 10); // 原点

	// 绘制坐标值
	dc.drawText(centerX - 15, centerY + 2, "O");
	// 绘制标志文本
	dc.drawText(centerX + 5, centerY - axisLength + 10, "Y"); // Y轴标志
	dc.drawText(centerX + axisLength + 10, centerY - 5, "X"); // X轴标志

	//绘制全部实体
	foreach(MEntity * pEnt, m_EntityList)
	{
		//如果实体在选择集中，则显示状态
		if (!m_SelectEntityList.contains(pEnt))
		{
			pEnt->Draw(this, dmNormal);// dc.setPen(pEnt->GetPen());
			//dc.setBrush(pEnt->GetBrush());
		}
		else {
			pEnt->Draw(this, dmSelect);
		}
	}
}

void QCADView::mousePressEvent(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton && m_pCmd)
	{
		m_pCmd->OnLButtonDown(mouseEvent);
	}
	if (mouseEvent->button() == Qt::RightButton && m_pCmd)
	{
		m_pCmd->OnRButtonDown(mouseEvent);
	}
}

void QCADView::mouseMoveEvent(QMouseEvent* mouseEvent)
{
	if (m_pCmd)
	{
		m_pCmd->OnMouseMove(mouseEvent);
		return;
	}

	//显示屏幕坐标和世界坐标
	QPoint pos = mouseEvent->pos();
	//QPointF sPos = mouseEvent->screenPos();
	//QPointF lPos = mouseEvent->localPos();
	//QPointF gPos = mouseEvent->globalPos();

	MainWindow* pMain = g_pMainWnd;
	QPointF scnPos = this->ScreentoWorld(pos);
	QString sScreenPosX = QString::number(scnPos.x(), 'f', 2);
	QString sScreenPosY = QString::number(scnPos.y(), 'f', 2);
	QString sScnPos = QStringLiteral("当前坐标：");
	sScnPos += sScreenPosX;
	sScnPos += ", ";
	sScnPos += sScreenPosY;

	if (m_pCmd == NULL)
		pMain->statusBar()->showMessage(sScnPos);
}

void QCADView::mouseReleaseEvent(QMouseEvent* mouseEvent)
{

}

void QCADView::mouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
	{
		return;
	}

	if (m_pCmd)
	{
		m_pCmd->OnLButtonDblClk(mouseEvent);
	}
}

void QCADView::addEntity(MEntity* pEnt)
{
	m_EntityList.push_back(pEnt);
}

void QCADView::removeEntity(MEntity* pEnt)
{
	m_EntityList.removeOne(pEnt);
}

void QCADView::removeLastEntity()
{
	m_EntityList.pop_back();
}

void QCADView::remove()
{
	foreach(MEntity * pEnt, m_SelectEntityList) {
		Addremove(pEnt);
	}
	m_SelectEntityList.clear();

}

void QCADView::selectEntity()
{
	if (m_pCmd && m_pCmd->GetType() == ctSelect)
		return;

	delete m_pCmd;
	m_pCmd = new MSelectCmd(this);
}

void QCADView::moveEntity()
{
	if (m_pCmd && m_pCmd->GetType() == ctMove)
		return;

	delete m_pCmd;
	m_pCmd = new MMoveCmd(this);
}

void QCADView::AddSelection(MEntity* pEnt)
{
	m_SelectEntityList.push_back(pEnt);
}

void QCADView::Addremove(MEntity* pEnt)
{
	m_removeEntityList.push_back(pEnt);
}

void QCADView::RemoveSelection(MEntity* pEnt)
{
	m_SelectEntityList.removeOne(pEnt);
}

void QCADView::drawLine()
{
	//QPainter pDC(this);

	if (m_pCmd && m_pCmd->GetType() == ctCreateLine)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateLine(this);
}

void QCADView::drawCircle()
{
	if (m_pCmd && m_pCmd->GetType() == ctCreateCircle)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateCircle(this);
}

void QCADView::drawArc()
{
	if (m_pCmd && m_pCmd->GetType() == ctCreateArc)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateArc(this);
}

void QCADView::drawRectangle()
{
	if (m_pCmd && m_pCmd->GetType() == ctCreateRectangle)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateRectangle(this);
}

void QCADView::drawPolygon()
{
	if (m_pCmd && m_pCmd->GetType() == ctCreatePolygon)
		return;

	delete m_pCmd;
	m_pCmd = new MCreatePolygon(this);
}

void QCADView::drawEllipse()
{
	//QPainter pDC(this);

	if (m_pCmd && m_pCmd->GetType() == ctCreateEllipse)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateEllipse(this);
}

void QCADView::drawText()
{
	//QPainter pDC(this);

	if (m_pCmd && m_pCmd->GetType() == ctCreateText)
		return;

	delete m_pCmd;
	m_pCmd = new MCreateText(this);
}

void QCADView::drawAnnotation()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreateAnnotation)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atRoughness, MAnnotation::FilletWeld);//默认值;
}

void QCADView::filletWeld()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreateFilletWeld)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atWelding, MAnnotation::FilletWeld);//角焊;
}

void QCADView::IWeld()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreateIWeld)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atWelding, MAnnotation::IWeld);//I焊;
}
void QCADView::VWeld()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreateVWeld)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atWelding, MAnnotation::VWeld);//V焊;
}
void QCADView::PlugWeld()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreatePlugWeld)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atWelding, MAnnotation::PlugWeld);//塞焊;
}
void QCADView::spotWeld()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreatespotWeld)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atWelding, MAnnotation::SpotWeld);//点焊;
}

void QCADView::standardA()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreateStandardA)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atStandard, MAnnotation::IWeld); // 基准标注A
}

void QCADView::standardB()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreateStandardB)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atStandard, MAnnotation::VWeld); // 基准标注B
}

void QCADView::standardC()
{
	//QPainter pDC(this);
	if (m_pCmd && m_pCmd->GetType() == ctCreateStandardC)
		return;
	delete m_pCmd;
	m_pCmd = new MCreateAnnotation(this, MAnnotation::atStandard, MAnnotation::FilletWeld); // 基准标注C
}

void QCADView::setScale(double scale)
{
	m_scale = scale;
}

//窗口功能
void QCADView::Scale(double scale)
{
	qreal scale_new = m_scale * scale;
	m_scale = scale_new;
	update();
}

void QCADView::zoomRect(QRectF rect)
{
	qreal dX = rect.width();
	qreal dY = rect.height();
	int wid = this->rect().width();
	int hit = this->rect().height();
	qreal dx = wid / dX;
	qreal dy = hit / dY;
	qreal scale = dx < dy ? dx : dy;
	setScale(scale);
	QPointF center = rect.center();//世界坐标系
	QPoint cps = WorldtoScreen(center);
	QPoint Scn_Center(wid / 2, hit / 2);
	MoveOrg(-cps.x() + Scn_Center.x(), -cps.y() + Scn_Center.y());

	update();
}
//center是屏幕坐标系
void QCADView::ScaleCenter(double scale, QPoint center)
{
	int X_Cen = this->width() / 2;
	int Y_cen = this->height() / 2;

	m_Xorg -= center.x() - X_Cen;
	m_Yorg -= center.y() - Y_cen;
	m_scale *= scale;
	update();
}

void QCADView::MoveOrg(int delX, int delY)
{
	m_Xorg += delX;
	m_Yorg += delY;
}

void QCADView::setOrg(QPoint cps)
{
	m_Xorg = cps.x();
	m_Yorg = cps.y();
}

double QCADView::GetScale()
{
	return m_scale;
}

QPoint QCADView::WorldtoScreen(QPointF pos)
{
	qreal x = pos.x();
	qreal y = pos.y();
	QPoint ScreenPt;
	ScreenPt.setX( x * m_scale + m_Xorg );
	ScreenPt.setY( -y * m_scale + m_Yorg );
	
	return ScreenPt;
}

QPointF QCADView::ScreentoWorld(QPointF pos)
{
	QPointF WorldPt;
	qreal x = pos.x();
	qreal y = pos.y();

	WorldPt.setX((x - m_Xorg) / m_scale);
	WorldPt.setY((-y + m_Yorg) / m_scale);

	return WorldPt;
}

QPointF QCADView::ScreentoWorld(QPoint pos)
{
	QPointF WorldPt;	
	int x = pos.x();
	int y = pos.y();

	/*double scale = this->GetScale();
	WorldPt.setX((x - m_offsetX) / scale + m_offsetX - Xorg);
	WorldPt.setY((y - m_offsetY) / scale + m_offsetY - Yorg);*/
	
	WorldPt.setX((x - m_Xorg) / m_scale);
	WorldPt.setY((-y + m_Yorg) / m_scale);

	return WorldPt;
}

void QCADView::zoomPan()
{
	if (m_pCmd && m_pCmd->GetType() == ctZoomPan)
		return;

	delete m_pCmd;
	m_pCmd = new MZoomPan(this);
}
//窗显
void QCADView::zoomWindow()
{
	if (m_pCmd && m_pCmd->GetType() == ctZoomWindow)
		return;

	delete m_pCmd;
	m_pCmd = new MZoomWindow(this);
}

void QCADView::Save(QDataStream& out)
{
	out << (qint32)m_EntityList.size();
	foreach(MEntity * pEnt, m_EntityList)
	{
		out << (qint32)pEnt->GetType();
		pEnt->Serialize(out, true);
	}
}

void QCADView::Read(QDataStream& in)
{
	// 清空现有实体
	foreach(MEntity * pEnt, m_EntityList)
	{
		delete pEnt;
	}
	m_EntityList.clear();

	qint32 count;
	in >> count;
	for (int i = 0; i < count; ++i)
	{
		qint32 type;
		in >> type;
		MEntity* pEnt = nullptr;
		switch ((EEntityType)type)
		{
		case EEntityType::etLine:
			pEnt = new MLine();
			break;
		case EEntityType::etRectangle:
			pEnt = new MRectangle();
			break;
		case EEntityType::etCircle:
			pEnt = new CCircle();
			break;
		case EEntityType::etArc:
			pEnt = new CArc();
			break;
		case EEntityType::etEllipse:
			pEnt = new MEllipse();
			break;
		case EEntityType::etPolygon:
			pEnt = new MPolygon();
			break;
		case EEntityType::etText:
			pEnt = new MText();
			break;
		case EEntityType::etAnnotation:
			pEnt = new MAnnotation();
			break;
			// 根据需要为其他实体类型添加 case
		}
		if (pEnt)
		{
			pEnt->Serialize(in, false);
			m_EntityList.append(pEnt);
		}
	}
	update();
}
