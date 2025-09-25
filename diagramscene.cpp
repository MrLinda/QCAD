/****************************************************************************
**
****************************************************************************/

#include "diagramscene.h"
#include "arrow.h"
#include "mainwindow.h"
#include "CREATECMD.H"
#include "CHGVIEWCMD.H"
#include "MODIFYCMD.H"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QStatusBar>

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
    myPenStyle = Qt::PenStyle::SolidLine;

    m_pCmd = NULL;
    //m_pCmd = new CCreateLine(this);
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    //对选中的实体改变
    QList<QGraphicsItem*> lstItems = selectedItems();
    foreach (QGraphicsItem * itm , lstItems)
    {
        //还得分别处理...
        //itm->setColr(myLineColor);
    }

    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]
void DiagramScene::setPenStyle(Qt::PenStyle ps)
{
    myPenStyle = ps;
}

void DiagramScene::setPenWidth(qreal wid)
{
    myPenWidth = wid;
}

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]

void DiagramScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    QPen pen(myPenStyle);   pen.setColor(myLineColor);  pen.setWidthF(myPenWidth);

    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }

    if (m_pCmd)
    {
        m_pCmd->OnLButtonDblClk(mouseEvent);
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPen pen(myPenStyle);   pen.setColor(myLineColor);  pen.setWidthF(myPenWidth);

    if (mouseEvent->button() == Qt::LeftButton && m_pCmd)
    {
        m_pCmd->OnLButtonDown(mouseEvent);
    }
    if (mouseEvent->button() == Qt::RightButton && m_pCmd)
    {
        m_pCmd->OnRButtonDown(mouseEvent);
    }

    DiagramItem *item;
 
    switch (myMode) {
        case InsertItem:
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;
//! [6] //! [7]
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(pen);
            addItem(line);
            break;
//! [7] //! [8]
        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
//! [8] //! [9]
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_pCmd)
    {
        m_pCmd->OnMouseMove(mouseEvent);
    }

    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }

    //显示屏幕坐标和世界坐标
    MainWindow* pMain = (MainWindow*)parent();
    QPointF scnPos = mouseEvent->scenePos();
    QString sScreenPosX = QString::number(scnPos.x(), 'f', 2);
    QString sScreenPosY = QString::number(scnPos.y(), 'f', 2);
    QString sScnPos = QStringLiteral("当前坐标：");
    sScnPos += sScreenPosX;
    sScnPos += ", ";
    sScnPos += sScreenPosY;

    if(m_pCmd == NULL)
        pMain->statusBar()->showMessage(sScnPos);
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    return;

    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
//! [12] //! [13]
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
//! [14]

//保存
void DiagramScene::Save(QDataStream& out)
{
    //遍历
    QList<QGraphicsItem*>  allItems = items();
    out << allItems.count();
    QGraphicsLineItem* lineItem;
    EntityType enType;
    foreach (QGraphicsItem * ent, allItems)
    {
        enType = (EntityType)ent->type();
        out << enType;
        switch (enType)
        {
        case EntityType::GI_LINE :
            lineItem = (QGraphicsLineItem*)ent;
            SaveLine(out, lineItem);
            break;

        case EntityType::GI_ELIPSE :
            break;
        ///...

        default:
            break;
        }
    }
}

//读文件
void DiagramScene::Read(QDataStream& in)
{
    //
    int nCount;
    in >> nCount;

    for(int i=0; i<nCount; i++)
    {
        int enType;
        QGraphicsLineItem* lineItem;
        in >> enType;
        switch (enType)
        {
        case EntityType::GI_LINE:
            lineItem = new QGraphicsLineItem();
            ReadLine(in, lineItem);
            addItem(lineItem);
            break;

        case EntityType::GI_ELIPSE:
            break;
            ///...

        default:
            break;
        }
    }
}

void DiagramScene::SaveLine(QDataStream& out, QGraphicsLineItem* lineItem)
{
    //起终点
    QLineF lineF = lineItem->line();
    out << lineF.x1() << lineF.y1() << lineF.x2() << lineF.y2();

    //画笔
    QPen pen = lineItem->pen();
    QColor clr = pen.color();
    out << clr.rgba();
    out << pen.style();
    out << pen.widthF();
    QBrush brsh = pen.brush();
    //画刷的存储还挺麻烦，...
}

void DiagramScene::ReadLine(QDataStream& in, QGraphicsLineItem* lineItem)
{
    //起终点
    qreal x1, y1, x2, y2;
    in >> x1 >> y1 >> x2 >> y2;
    lineItem->setLine(x1,y1,x2,y2);

    //画笔
    QPen pen;
    QColor clr;
    unsigned int rgba;
    in >> rgba;
    clr.setRgba(rgba);
    pen.setColor(clr);
    int iStyle;
    in >> iStyle;
    pen.setStyle((Qt::PenStyle)iStyle);
    qreal wid;
    in >> wid;
    pen.setWidthF(wid);
    lineItem->setPen(pen);

    QBrush brsh = pen.brush();
    //画刷的存储还挺麻烦，...
}

void DiagramScene::ResetSelectStatus()
{
    MainWindow* pMain = (MainWindow*)parent();
    pMain->selectEntity();
}

//选择实体
void DiagramScene::selectEntity()
{
    delete m_pCmd;
    m_pCmd = NULL;   

    foreach(QGraphicsItem * pItem, items())
    {
        pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
}

void DiagramScene::drawLine()
{
    if (m_pCmd && m_pCmd->GetType() == ctCreateLine)
        return;

    delete m_pCmd;
    m_pCmd = new CCreateLine(this);
}

void DiagramScene::drawCircle()
{
    if (m_pCmd && m_pCmd->GetType() == ctCreateCircle)
        return;

    delete m_pCmd;
    m_pCmd = new CCreateCircle(this);
}

void DiagramScene::drawArc()
{
    if (m_pCmd && m_pCmd->GetType() == ctCreateArc)
        return;

    delete m_pCmd;
    m_pCmd = new CCreateArc(this);
}

void DiagramScene::drawRect()
{
    if (m_pCmd && m_pCmd->GetType() == ctCreateRectangle)
        return;

    delete m_pCmd;
    m_pCmd = new CCreateRect(this);
}

void DiagramScene::drawEllipse()
{
    if (m_pCmd && m_pCmd->GetType() == ctCreateEllipse)
        return;

    delete m_pCmd;
    m_pCmd = new CCreateEllipse(this);
}

void DiagramScene::drawPolygon()
{
    if (m_pCmd && m_pCmd->GetType() == ctCreatePolygon)
        return;

    delete m_pCmd;
    m_pCmd = new CCreatePolygon(this);
}

//zoom
void DiagramScene::zoomPan()
{
    if (m_pCmd && m_pCmd->GetType() == ctZoomPan)
        return;

    delete m_pCmd;
    m_pCmd = new CZoomPanCmd(this);
}

void DiagramScene::zoomWindow()
{
    if (m_pCmd && m_pCmd->GetType() == ctZoomWindow)
        return;

    delete m_pCmd;
    m_pCmd = new CZoomWindowCmd(this);
}

//
void DiagramScene::moveEntity()
{
    if (m_pCmd && m_pCmd->GetType() == ctMove)
        return;

    delete m_pCmd;
    m_pCmd = new CMove(this);
}

void DiagramScene::rotateEntity()
{
    if (m_pCmd && m_pCmd->GetType() == ctRotate)
        return;

    delete m_pCmd;
    m_pCmd = new CRotate(this);
}

void DiagramScene::symetryEntity()
{
    if (m_pCmd && m_pCmd->GetType() == ctMirror)
        return;

    delete m_pCmd;
    m_pCmd = new CMirror(this);
}
