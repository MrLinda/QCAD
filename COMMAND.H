#ifndef _Command_h_
#define _Command_h_

#include <QPoint>
#include <QGraphicsSceneMouseEvent>

#ifdef __cplusplus

enum ECommandType	// 命令类
{
	ctSelect = 0,

//  创建对象命令类
	ctCreateLine = 1,		// 创建直线段
	ctCreateRectangle = 2,	// 创建矩形
	ctCreateCircle = 3,		// 创建圆
	ctCreateArc = 4,		// 创建圆弧
	ctCreateEllipse = 5,    //创建椭圆
	ctCreatePolygon = 6,    //多边形 
	ctCreateText = 7,       //创建文本

//  修改命令类
	ctMove = 11,			// 移动
	ctRotate = 12,			// 旋转
	ctMirror = 13,			// 镜像

//修改视图命令类
	ctZoomPan = 20 ,
	ctZoomWindow = 21
};

class DiagramScene;

class CCommand
{
protected:
	int		m_nStep ; // 命令操作步
	bool    m_bCaptured; //捕捉
	QPointF m_ptCapture; //捕捉点
	
	void     ShowCapturePoint(const QPointF &pos,bool bShow = false);
	bool     GetNearKeyPoint(QPointF &pt);
	DiagramScene* m_pDiagramScene;
public:
	CCommand() {
		m_nStep = 0;
		m_bCaptured = false;
	}
	CCommand(DiagramScene* pScene) {
		m_nStep = 0;
		m_bCaptured = false;
		m_pDiagramScene = pScene;
	}
	~CCommand() {}

	virtual int GetType() = 0; // 返回命令类型 ECommandType
	virtual int OnLButtonDown(QGraphicsSceneMouseEvent* mouseEvent) = 0 ;
	virtual int OnMouseMove(QGraphicsSceneMouseEvent* mouseEvent);
	virtual int OnRButtonDown(QGraphicsSceneMouseEvent* mouseEvent) = 0 ;
	virtual int OnLButtonDblClk(QGraphicsSceneMouseEvent* mouseEvent) {return 0;}
	virtual int Cancel() = 0 ;

public:
	void Prompt(QString sInfo);
} ;

#endif

#endif
