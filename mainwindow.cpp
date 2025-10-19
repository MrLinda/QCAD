/****************************************************************************
//wyz
****************************************************************************/
#define QT_PAINTER

#include "arrow.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "mainwindow.h"
#include "QCADView.h"
#include "ENTITY.H"

#include <QtWidgets>

MainWindow* g_pMainWnd;
const int InsertTextButton = 10;

//MainWindow* g_p
void Prompt(QString sInfo)
{
    MainWindow* pMain = g_pMainWnd;
    pMain->statusBar()->showMessage(sInfo);
}

//! [0]
MainWindow::MainWindow()
{
    createActions();
    createToolBox();
    createMenus();

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    scene = NULL;
    view = new QCADView();
    {
        view->setGeometry(0, 0, 1000, 800);
        QPalette pal(view->palette());
        pal.setColor(QPalette::Window, Qt::white);
        view->setAutoFillBackground(true);
        view->setPalette(pal);
    }
#else
    scene = new DiagramScene(itemMenu, this);
    view = new QGraphicsView(scene);
#endif // QT_PAINTER

    /*scene->setSceneRect(QRectF(0, 0, 1000, 800));
   connect(scene, SIGNAL(itemInserted(DiagramItem*)),
           this, SLOT(itemInserted(DiagramItem*)));
   connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
           this, SLOT(textInserted(QGraphicsTextItem*)));
   connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
           this, SLOT(itemSelected(QGraphicsItem*)));*/
    createToolbars();

    //// 初始化状态栏
    //QLabel* labCellIndex = new QLabel("当前坐标: 0.0", this);
    //labCellIndex->setMinimumWidth(150);
    //statusBar()->showMessage("coord: 0,0");

    QString sScnPos = QStringLiteral("就绪");
    statusBar()->showMessage(sScnPos);

    view->setMouseTracking(true);
    //view->fitInView(0, 0, 800, 600);

    QHBoxLayout* layout = new QHBoxLayout;
    //layout->addWidget(toolBox);
    layout->addWidget(view);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("QCAD - untitled"));
    setUnifiedTitleAndToolBarOnMac(true);

    m_sFileName = "untitled";
    g_pMainWnd = this;
}
//! [0]

//! [1]
void MainWindow::backgroundButtonGroupClicked(QAbstractButton* button)
{
    QList<QAbstractButton*> buttons = backgroundButtonGroup->buttons();
    foreach(QAbstractButton * myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap("images/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap("images/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap("images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap("images/background4.png"));

    scene->update();
    view->update();
}
//! [1]

//! [2]
void MainWindow::buttonGroupClicked(int id)
{
    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    foreach(QAbstractButton * button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    }
    else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}
//! [2]

//! [3]
void MainWindow::deleteItem()
{
#ifdef QT_PAINTER
    //QCAD-QPainter版本
    QList<MEntity*> delList = view->GetSelectedEntityList();
    foreach(MEntity * pEnt, delList) {
        view->removeEntity(pEnt);
        delete pEnt;
    }
    view->update();

#elif
    foreach(QGraphicsItem * item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow* arrow = qgraphicsitem_cast<Arrow*>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach(QGraphicsItem * item, scene->selectedItems()) {
        if (item->type() == DiagramItem::Type)
            qgraphicsitem_cast<DiagramItem*>(item)->removeArrows();
        scene->removeItem(item);
        delete item;
    }
#endif // QT_PAINTER
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach(QGraphicsItem * item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach(QGraphicsItem * item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void MainWindow::itemInserted(DiagramItem* item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem*)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont&)
{
    handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString&)
{
    handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString& scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    //QMatrix oldMatrix = view->matrix();
    //view->resetMatrix();
    //view->translate(oldMatrix.dx(), oldMatrix.dy());
    //view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction*>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
        "images/textpointer.png",
        qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction*>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
        "images/floodfill.png",
        qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction*>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
        "images/linecolor.png",
        qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->setTextColor(qvariant_cast<QColor>(textAction->data()));
#else
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
#endif // QT_PAINTER
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->setBrushColor(qvariant_cast<QColor>(fillAction->data()));
#else
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
#endif // QT_PAINTER
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->setLineColor(qvariant_cast<QColor>(lineAction->data()));
#else
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
#endif // QT_PAINTER
}
//! [17]
void MainWindow::lineTypeChanged(QString sType)
{
    Qt::PenStyle pStyle = Qt::PenStyle::SolidLine;
    if (sType == QStringLiteral("实线"))
    {
        pStyle = Qt::PenStyle::SolidLine;
    }
    else if (sType == QStringLiteral("虚线"))
    {
        pStyle = Qt::PenStyle::DashLine;
    }
    else if (sType == QStringLiteral("点划线"))
    {
        pStyle = Qt::PenStyle::DashDotLine;
    }

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->setPenStyle(pStyle);
#else
    scene->setPenStyle(pStyle);
#endif // QT_PAINTER
}

void MainWindow::lineWidChanged(QString sWid)
{
    qreal pWid = 1;
    if (sWid == QStringLiteral("宽度1"))
    {
        pWid = 1;
    }
    else if (sWid == QStringLiteral("宽度2"))
    {
        pWid = 2;
    }
    else if (sWid == QStringLiteral("宽度3"))
    {
        pWid = 3;
    }
    else if (sWid == QStringLiteral("宽度4"))
    {
        pWid = 4;
    }

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->setPenWidth(pWid);
#else
    scene->setPenWidth(pWid);
#endif // QT_PAINTER
}

//! [18]
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->setFont(font);
#else
    scene->setFont(font);
#endif // QT_PAINTER
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem* item)
{
    DiagramTextItem* textItem =
        qgraphicsitem_cast<DiagramTextItem*>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
    QMessageBox::about(this, QStringLiteral("关于QCAD"),
        QStringLiteral("小型二维图形系统--"
            "华中科技大学CAD中心"));
}
//! [20]

//! [21]
void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
        this, SLOT(buttonGroupClicked(int)));
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step), 0, 1);
    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);
    //! [21]

    QToolButton* textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap("images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout* textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget* textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
        this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout* backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
        "images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
        "images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
        "images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
        "images/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget* backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);


    //! [22]
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}
//! [22]

//! [23]
void MainWindow::createActions()
{
    /// <summary>
    /// File Actions
    /// </summary>
    newFileAction = new QAction(QIcon("images/newFile.png"),
        QStringLiteral("新建文件"), this);
    newFileAction->setShortcut(tr("Ctrl+N"));
    newFileAction->setStatusTip(QStringLiteral("新建文件"));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openFileAction = new QAction(QIcon("images/openFile.png"),
        QStringLiteral("打开文件"), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(QStringLiteral("打开文件"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveFileAction = new QAction(QIcon("images/saveFile.png"),
        QStringLiteral("保存文件"), this);
    saveFileAction->setShortcut(tr("Ctrl+S"));
    saveFileAction->setStatusTip(QStringLiteral("保存文件"));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsFileAction = new QAction(QIcon("images/saveAsFile.png"),
        QStringLiteral("另存文件"), this);
    saveAsFileAction->setShortcut(tr("Ctrl+A"));
    saveAsFileAction->setStatusTip(QStringLiteral("另存文件"));
    connect(saveAsFileAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    /// <summary>
    /// CreateActions
    /// </summary>
    drawLineAction = new QAction(QIcon("images/drawLine.png"),
        QStringLiteral("直线"), this);
    drawLineAction->setShortcut(tr("Ctrl+L"));
    drawLineAction->setStatusTip(QStringLiteral("直线"));
    connect(drawLineAction, SIGNAL(triggered()), this, SLOT(drawLine()));

    drawCircleAction = new QAction(QIcon("images/drawCircle.png"),
        QStringLiteral("圆"), this);
    //drawCircleAction->setShortcut(tr("Ctrl+L"));
    drawCircleAction->setStatusTip(QStringLiteral("圆"));
    connect(drawCircleAction, SIGNAL(triggered()), this, SLOT(drawCircle()));

    drawArcAction = new QAction(QIcon("images/drawArc.png"),
        QStringLiteral("圆弧"), this);
    drawArcAction->setStatusTip(QStringLiteral("圆弧"));
    connect(drawArcAction, SIGNAL(triggered()), this, SLOT(drawArc()));

    drawRectAction = new QAction(QIcon("images/drawRect.png"),
        QStringLiteral("矩形"), this);
    drawRectAction->setStatusTip(QStringLiteral("矩形"));
    connect(drawRectAction, SIGNAL(triggered()), this, SLOT(drawRect()));

    drawEllipseAction = new QAction(QIcon("images/drawEllipse.png"),
        QStringLiteral("椭圆"), this);
    drawEllipseAction->setStatusTip(QStringLiteral("椭圆"));
    connect(drawEllipseAction, SIGNAL(triggered()), this, SLOT(drawEllipse()));

    drawPolygonAction = new QAction(QIcon("images/drawPolygon.png"),
        QStringLiteral("多边形"), this);
    drawPolygonAction->setStatusTip(QStringLiteral("多边形"));
    connect(drawPolygonAction, SIGNAL(triggered()), this, SLOT(drawPolygon()));

    drawTextAction = new QAction(QIcon("images/drawText.png"),
        QStringLiteral("文本"), this);
    drawTextAction->setStatusTip(QStringLiteral("文本"));
    connect(drawTextAction, SIGNAL(triggered()), this, SLOT(drawText()));

    drawAnnotationAction = new QAction(QIcon("images/drawAnnotation.png"),
        QStringLiteral("符号标注"), this);
    drawAnnotationAction->setStatusTip(QStringLiteral("符号标注"));
    connect(drawAnnotationAction, SIGNAL(triggered()), this, SLOT(drawAnnotation()));

    FilletweldAction = new QAction(QIcon("images/filletWeld.png"), QStringLiteral("角焊符号"), this);
    FilletweldAction->setStatusTip(QStringLiteral("角焊符号"));
    connect(FilletweldAction, SIGNAL(triggered()), this, SLOT(filletWeld()));

    IweldAction = new QAction(QIcon("images/IWeld.png"), QStringLiteral("I型焊缝符号"), this);
    IweldAction->setStatusTip(QStringLiteral("I型焊缝符号"));
    connect(IweldAction, SIGNAL(triggered()), this, SLOT(IWeld()));

    VweldAction = new QAction(QIcon("images/VWeld.png"), QStringLiteral("V型焊缝符号"), this);
    VweldAction->setStatusTip(QStringLiteral("V型焊缝符号"));
    connect(VweldAction, SIGNAL(triggered()), this, SLOT(VWeld()));

    PlugweldAction = new QAction(QIcon("images/PlugWeld.png"), QStringLiteral("塞焊缝符号"), this);
    PlugweldAction->setStatusTip(QStringLiteral("塞焊缝符号"));
    connect(PlugweldAction, SIGNAL(triggered()), this, SLOT(PlugWeld()));

    spotweldAction = new QAction(QIcon("images/spotWeld.png"), QStringLiteral("点焊缝符号"), this);
    spotweldAction->setStatusTip(QStringLiteral("点焊缝符号"));
    connect(spotweldAction, SIGNAL(triggered()), this, SLOT(spotWeld()));

    spotweldAction = new QAction(QIcon("images/spotWeld.png"), QStringLiteral("点焊缝"), this);
    spotweldAction->setStatusTip(QStringLiteral("点焊缝"));
    connect(spotweldAction, SIGNAL(triggered()), this, SLOT(spotWeld()));

    standardAnnotationAction = new QAction(QIcon("images/standardA.png"), QStringLiteral("基准标注A"), this);
    standardAnnotationAction->setStatusTip(QStringLiteral("基准标注A"));
    connect(standardAnnotationAction, SIGNAL(triggered()), this, SLOT(standardAnnotation()));

    /// <summary>
    /// Modify Actions
    /// </summary>
    selectEntityAction = new QAction(QIcon("images/select.png"),
        QStringLiteral("选择"), this);
    selectEntityAction->setStatusTip(QStringLiteral("选择"));
    selectEntityAction->setCheckable(true);
    connect(selectEntityAction, SIGNAL(triggered()), this, SLOT(selectEntity()));
    //connect(selectEntityAction, &QAction::triggered, [&]() { scene->selectEntity(); });

    regenEntityAction = new QAction(QIcon("images/regen.png"),
        QStringLiteral("重画"), this);
    regenEntityAction->setStatusTip(QStringLiteral("重画"));
    connect(regenEntityAction, SIGNAL(triggered()), this, SLOT(regenEntity()));

    moveEntityAction = new QAction(QIcon("images/move.png"),
        QStringLiteral("移动实体"), this);
    moveEntityAction->setStatusTip(QStringLiteral("移动实体"));
    connect(moveEntityAction, SIGNAL(triggered()), this, SLOT(moveEntity()));

    symetryEntityAction = new QAction(QIcon("images/symetry.png"),
        QStringLiteral("对称实体"), this);
    symetryEntityAction->setStatusTip(QStringLiteral("对称实体"));
    connect(symetryEntityAction, SIGNAL(triggered()), this, SLOT(symetryEntity()));

    rotateEntityAction = new QAction(QIcon("images/rotate.png"),
        QStringLiteral("旋转实体"), this);
    rotateEntityAction->setStatusTip(QStringLiteral("旋转实体"));
    connect(rotateEntityAction, SIGNAL(triggered()), this, SLOT(rotateEntity()));

    removeEntityAction = new QAction(QIcon("images/remove.png"),
        QStringLiteral("删除实体"), this);
    removeEntityAction->setStatusTip(QStringLiteral("删除实体"));
    connect(removeEntityAction, SIGNAL(triggered()), this, SLOT(removeEntity()));

    /// <summary>
    /// Zoom Actions
    /// </summary>
    zoomInAction = new QAction(QIcon("images/zoomIn.png"),
        QStringLiteral("放大"), this);
    zoomInAction->setStatusTip(QStringLiteral("放大"));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomToolBar->addAction(zoomOutAction);
    zoomOutAction = new QAction(QIcon("images/zoomOut.png"),
        QStringLiteral("缩小"), this);
    zoomOutAction->setStatusTip(QStringLiteral("缩小"));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));

    zoomAllAction = new QAction(QIcon("images/zoomAll.png"),
        QStringLiteral("全显"), this);
    zoomAllAction->setStatusTip(QStringLiteral("全显"));
    connect(zoomAllAction, SIGNAL(triggered()), this, SLOT(zoomAll()));

    zoomWindowAction = new QAction(QIcon("images/zoomWindow.png"),
        QStringLiteral("窗显"), this);
    zoomWindowAction->setStatusTip(QStringLiteral("窗显"));
    connect(zoomWindowAction, SIGNAL(triggered()), this, SLOT(zoomWindow()));

    zoomPanAction = new QAction(QIcon("images/zoomPan.png"),
        QStringLiteral("移动视图"), this);
    zoomPanAction->setStatusTip(QStringLiteral("移动视图"));
    connect(zoomPanAction, SIGNAL(triggered()), this, SLOT(zoomPan()));

    /// <summary>
    /// AttribActions
    /// </summary>

    toFrontAction = new QAction(QIcon("images/bringtofront.png"),
        tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
    //! [23]

    sendBackAction = new QAction(QIcon("images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon("images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(QIcon("images/exit.png"), tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap("images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon("images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon("images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(QStringLiteral("关于"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

//! [24]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(QStringLiteral("文件(&F)"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);


}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
    //五个工具条
    // 1.文件：新建，打开，保存，另存，退出
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newFileAction);
    fileToolBar->addAction(openFileAction);
    fileToolBar->addAction(saveFileAction);
    fileToolBar->addAction(saveAsFileAction);
    fileToolBar->addAction(exitAction);

    // 2.绘图：直线，园，圆弧，长方形，椭圆，文本，
    drawToolBar = addToolBar(tr("Draw"));
    drawToolBar->addAction(drawLineAction);
    drawToolBar->addAction(drawCircleAction);
    drawToolBar->addAction(drawArcAction);
    drawToolBar->addAction(drawRectAction);
    drawToolBar->addAction(drawEllipseAction);
    drawToolBar->addAction(drawPolygonAction);
    drawToolBar->addAction(drawTextAction);
    // 3.修改：选择，刷新，平移，对称，旋转，删除
    modifyToolBar = addToolBar(tr("modify"));
    modifyToolBar->addAction(selectEntityAction);
    modifyToolBar->addAction(regenEntityAction);
    modifyToolBar->addAction(moveEntityAction);
    modifyToolBar->addAction(symetryEntityAction);
    modifyToolBar->addAction(rotateEntityAction);
    modifyToolBar->addAction(removeEntityAction);

    // 4.视图：局部放大/缩小，全部显示，窗口显示，平移视图
    zoomToolBar = addToolBar(tr("zoom"));
    zoomToolBar->addAction(zoomInAction);
    zoomToolBar->addAction(zoomOutAction);
    zoomToolBar->addAction(zoomAllAction);
    zoomToolBar->addAction(zoomWindowAction);
    zoomToolBar->addAction(zoomPanAction);

    annotationToolbar = addToolBar(tr("annotation"));
    annotationToolbar->addAction(drawAnnotationAction);
    annotationToolbar->addAction(FilletweldAction);
    annotationToolbar->addAction(IweldAction);
    annotationToolbar->addAction(VweldAction);
    annotationToolbar->addAction(PlugweldAction);
    annotationToolbar->addAction(spotweldAction);
    annotationToolbar->addAction(standardAnnotationAction);

    // 5.属性：线型，颜色，宽度
    //线型：实线、虚线、点划线
    lineTypeCombo = new QComboBox;
    QStringList lineTypes;
    lineTypes << QStringLiteral("实线") << QStringLiteral("虚线") << QStringLiteral("点划线");
    lineTypeCombo->addItems(lineTypes);
    lineTypeCombo->setCurrentIndex(0);
    connect(lineTypeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(lineTypeChanged(QString)));


    //颜色，线条颜色
    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon("images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()), this, SLOT(lineButtonTriggered()));

    //线宽：1~4种吧
    lineWidCombo = new QComboBox;
    QStringList lineWidths;
    lineWidths << QStringLiteral("宽度1") << QStringLiteral("宽度2") << QStringLiteral("宽度3") << QStringLiteral("宽度4");
    lineWidCombo->addItems(lineWidths);
    lineWidCombo->setCurrentIndex(0);
    connect(lineWidCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(lineWidChanged(QString)));

    //! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon("images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()), this, SLOT(fillButtonTriggered()));
    //! [26]
    //! 
    attribToolbar = addToolBar(tr("Attribute"));
    attribToolbar->addWidget(lineTypeCombo);
    attribToolbar->addWidget(lineWidCombo);
    attribToolbar->addWidget(lineColorToolButton);
    attribToolbar->addWidget(fillColorToolButton);
    //! [25]


        //editToolBar = addToolBar(tr("Edit"));
        //editToolBar->addAction(deleteAction);
        //editToolBar->addAction(toFrontAction);
        //editToolBar->addAction(sendBackAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
        this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator* validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
        this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon("images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
        this, SLOT(textButtonTriggered()));



    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    textToolBar->addWidget(fontColorToolButton);

    //colorToolBar = addToolBar(tr("Color"));
    //colorToolBar->addWidget(fontColorToolButton);
    //colorToolBar->addWidget(fillColorToolButton);
    //colorToolBar->addWidget(lineColorToolButton);

    QToolButton* pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon("images/pointer.png"));
    QToolButton* linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon("images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
        this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
        this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    //pointerToolbar->addWidget(pointerButton);
    //pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
}
//! [27]

//! [28]
QWidget* MainWindow::createBackgroundCellWidget(const QString& text, const QString& image)
{
    QToolButton* button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget* MainWindow::createCellWidget(const QString& text, DiagramItem::DiagramType type)
{

    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());

    QToolButton* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [29]

//! [30]
QMenu* MainWindow::createColorMenu(const char* slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::green << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue") << tr("green")
        << tr("yellow");

    QMenu* colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction* action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString& imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
//! [32]

void MainWindow::newFile()
{
    //单文档吧
    //检查当前有没有文档，且发生更改？
    //清空内存
    //设置文档名untitled.cad，标题改变
}

void MainWindow::openFile()
{
    //检查当前有没有文档，且发生更改？
    //清空内存
    //读入文档内容
    QString sName = QFileDialog::getOpenFileName();
    if (sName.isEmpty())
    {
        return;
    }
    m_sFileName = sName;

    //将内存内容保存到文档中
    QFile file(m_sFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file failed";
        return;
    }
    QDataStream in(&file);
#ifdef QT_PAINTER
    view->Read(in);
#else
    scene->Read(in);
#endif // QT_PAINTER
    file.close();

    //更改文档名称，标题改变
    QString stitle = tr("QCAD - ");
    stitle += m_sFileName;
    setWindowTitle(stitle);
}

void MainWindow::saveFile()
{
    //检查文档改变，如果未改变，直接退出
    //检查文档名称，如果untitled，则提示输入文件名
    if (m_sFileName == tr("untitled"))
    {
        QString sName = QFileDialog::getSaveFileName();
        if (sName.isEmpty())
        {
            return;
        }
        m_sFileName = sName;
    }
    //将内存内容保存到文档中
    QFile file(m_sFileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "open file failed";
        return;
    }
    QDataStream out(&file);
#ifdef QT_PAINTER
    if (view) view->Save(out);
#else
    if (scene) scene->Save(out);
#endif // QT_PAINTER
    file.close();
    return;

    //out.setVersion(QDataStream::Qt_6_3);
    const char* w_str = "hello world";
    const char* w_str1 = "模板";
    quint32 w_quint32 = 1234;
    double  w_double = 1.1;
    float   w_float = 2.2f;
    QVector<int> w_vector;//类数组
    w_vector.push_back(1);
    w_vector.push_back(2);
    w_vector.push_back(3);
    //工具名称
    QVector<QString> w_vector_toolsName;//检测工具
    w_vector_toolsName.push_back(QStringLiteral("阈值分割"));
    //
    QMap<int, int> w_map;//key value
    w_map.insert(4, 4);
    w_map.insert(5, 5);
    w_map.insert(6, 6);

    out << w_str;//字符串
    out << w_str1;
    out << w_quint32;//uint类型
    out << w_double;//double类型
    out << w_float;//float类型
    out << w_vector;//
    out << w_vector_toolsName;
    out << w_map;
    //out << w_test;
    out.writeBytes("file end ", qstrlen("file end "));
    out.writeRawData("really end", qstrlen("really end"));
    file.close();

    //测试读入
    QFile file1(m_sFileName);
    if (!file1.open(QIODevice::ReadWrite))
    {
        qDebug() << "open file failed";
        return;
    }
    file1.seek(0);
    QDataStream in(&file1);
    //in.setVersion(QDataStream::Qt_6_3);
    char* r_str1 = new char[100];
    char* r_str2 = new char[100];
    in >> r_str1;
    in >> r_str2;
    quint32 r_quint32 = 1234;
    double  r_double = 1.1;
    float   r_float = 2.2f;
    QVector<int> r_vector;//类数组
    in >> r_quint32 >> r_double >> r_float;
    in >> r_vector;
    QVector<QString> r_vector_toolsName;
    in >> r_vector_toolsName;
    QMap<int, int> r_map;
    in >> r_map;
    char* r_byte = new char[100];
    uint nLen;
    in.readBytes(r_byte, nLen);
    in.readRawData(r_byte, nLen);
    file1.close();
}

void MainWindow::saveAsFile()
{

}


//视窗
void MainWindow::zoomWindow()
{
    zoomWindowAction->setCheckable(true);
    unCheckAllCommand();
    zoomWindowAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->zoomWindow();
#else
    scene->zoomWindow();
#endif // QT_PAINTER
}

void MainWindow::zoomPan()
{
    zoomPanAction->setCheckable(true);
    unCheckAllCommand();
    zoomPanAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->zoomPan();
#else
    scene->zoomPan();
#endif // QT_PAINTER

}
void MainWindow::zoomIn()
{
#ifdef QT_PAINTER
    //放大 //QCAD-QPainter版本
    view->Scale(1 / 0.618);
#endif // QT_PAINTER
}

void MainWindow::zoomOut()
{
#ifdef QT_PAINTER
    //缩小 //QCAD-QPainter版本
    view->Scale(0.618);
#endif // QT_PAINTER

}

void MainWindow::zoomAll()
{
#ifdef QT_PAINTER
    //QCAD-QPainter版本
    qreal xmin = 1E10, ymin = 1E10, xmax = -1E10, ymax = -1E10;
    QList<MEntity*> lstItems = view->GetEntityList();
    if (lstItems.count() == 0)
    {
        xmin = 0;
        ymin = 0;
        xmax = 800;
        ymax = 600;
        return;
    }
    foreach(MEntity * theItem, lstItems)
    {
        if (1)
        {
            QRectF rectEnt;
            theItem->GetBox(rectEnt);
            QPointF beginpos = rectEnt.topLeft();
            QPointF endpos = rectEnt.bottomRight();
            if (xmin > beginpos.x())
                xmin = beginpos.x();
            if (ymin > beginpos.y())
                ymin = beginpos.y();
            if (xmin > endpos.x())
                xmin = endpos.x();
            if (ymin > endpos.y())
                ymin = endpos.y();

            if (xmax < beginpos.x())
                xmax = beginpos.x();
            if (ymax < beginpos.y())
                ymax = beginpos.y();
            if (xmax < endpos.x())
                xmax = endpos.x();
            if (ymax < endpos.y())
                ymax = endpos.y();
        }
    }
    QPointF lt(xmin, ymax);
    QPointF br(xmax, ymin);
    QRectF rect(lt, br);
    view->zoomRect(rect);
#else
    qreal xmin = 1E10, ymin = 1E10, xmax = -1E10, ymax = -1E10;

    QList<QGraphicsItem*> lstItems = scene->items();
    if (lstItems.count() == 0)
    {
        xmin = 0;
        ymin = 0;
        xmax = 800;
        ymax = 600;
    }
    foreach(QGraphicsItem * theItem, lstItems)
    {
        QRectF rect = theItem->boundingRect();
        if (xmin > rect.x())
        {
            xmin = rect.x();
        }
        if (ymin > rect.y())
        {
            ymin = rect.y();
        }
        if (xmax < rect.x() + rect.width())
        {
            xmax = rect.x() + rect.width();
        }
        if (ymax < rect.y() + rect.height())
        {
            ymax = rect.y() + rect.height();
        }
    }
    qreal dX = xmax - xmin;
    qreal dY = ymax - ymin;
#endif // QT_PAINTER

}

void MainWindow::unCheckAllCommand()
{
    selectEntityAction->setChecked(false);
    drawLineAction->setChecked(false);
    drawCircleAction->setChecked(false);
    drawArcAction->setChecked(false);
    drawRectAction->setChecked(false);
    drawEllipseAction->setChecked(false);
    drawPolygonAction->setChecked(false);
    drawTextAction->setChecked(false);
    drawAnnotationAction->setChecked(false);
    FilletweldAction->setChecked(false);
    IweldAction->setChecked(false);
    VweldAction->setChecked(false);
    PlugweldAction->setChecked(false);
    spotweldAction->setChecked(false);
    standardAnnotationAction->setChecked(false);


    symetryEntityAction->setChecked(false);
    moveEntityAction->setChecked(false);
    rotateEntityAction->setChecked(false);
    removeEntityAction->setChecked(false);

    zoomPanAction->setChecked(false);
    zoomWindowAction->setChecked(false);
}
void MainWindow::selectEntity()
{
    unCheckAllCommand();
    selectEntityAction->setChecked(true);// !selectEntityAction->isChecked());

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->selectEntity();
#else
    scene->selectEntity();
#endif // QT_PAINTER
}

void MainWindow::drawLine()
{
    drawLineAction->setCheckable(true);
    unCheckAllCommand();
    drawLineAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawLine();
#else
    scene->drawLine();
#endif // QT_PAINTER

}

void MainWindow::drawCircle()
{
    drawCircleAction->setCheckable(true);
    unCheckAllCommand();
    drawCircleAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawCircle();
#else
    scene->drawCircle();
#endif // QT_PAINTER

}

void MainWindow::drawArc()
{
    drawArcAction->setCheckable(true);
    unCheckAllCommand();
    drawArcAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawArc();
#else
    scene->drawArc();
#endif // QT_PAINTER

}

void MainWindow::drawRect()
{
    drawRectAction->setCheckable(true);

    unCheckAllCommand();
    drawRectAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawRectangle();
#else
    scene->drawRect();
#endif // QT_PAINTER

}

void MainWindow::drawEllipse()
{
    drawEllipseAction->setCheckable(true);
    unCheckAllCommand();
    drawEllipseAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawEllipse();
#else
    scene->drawEllipse();
#endif // QT_PAINTER

}

void MainWindow::drawPolygon()
{
    drawPolygonAction->setCheckable(true);
    unCheckAllCommand();
    drawPolygonAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawPolygon();
#else
    scene->drawPolygon();
#endif // QT_PAINTER

}

void MainWindow::drawText()
{
    drawTextAction->setCheckable(true);
    unCheckAllCommand();
    drawTextAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawText();
#else
    //scene->drawText();  //未实现
    ;
#endif // QT_PAINTER

}

void MainWindow::drawAnnotation()
{
    drawAnnotationAction->setCheckable(true);
    unCheckAllCommand();
    drawAnnotationAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->drawAnnotation();
#else
    //scene->drawAnnotation();
#endif // QT_PAINTER

}

void MainWindow::filletWeld()
{
    FilletweldAction->setCheckable(true);
    unCheckAllCommand();
    FilletweldAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->filletWeld();
#else
    //scene->filletWeld();
#endif // QT_PAINTER

}


void MainWindow::IWeld()
{
    IweldAction->setCheckable(true);
    unCheckAllCommand();
    IweldAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->IWeld();
#else
    //scene->IWeld();
#endif // QT_PAINTER
}

void MainWindow::VWeld()
{
    VweldAction->setCheckable(true);
    unCheckAllCommand();
    VweldAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->VWeld();
#else
    //scene->filletWeld();
#endif // QT_PAINTER
}

void MainWindow::PlugWeld()
{
    PlugweldAction->setCheckable(true);
    unCheckAllCommand();
    PlugweldAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->PlugWeld();
#else
    //scene->filletWeld();
#endif // QT_PAINTER
}

void MainWindow::spotWeld()
{
    spotweldAction->setCheckable(true);
    unCheckAllCommand();
    spotweldAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->spotWeld();
#else
    //scene->filletWeld();
#endif // QT_PAINTER
}
void MainWindow::standardAnnotation()
{
    standardAnnotationAction->setCheckable(true);
    unCheckAllCommand();
    standardAnnotationAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->standardA();
#else
    //scene->standardA();
#endif // QT_PAINTER
}

void MainWindow::moveEntity()
{
    moveEntityAction->setCheckable(true);
    unCheckAllCommand();
    moveEntityAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    ;
#else
    scene->moveEntity();
#endif // QT_PAINTER

}

void MainWindow::rotateEntity()
{
    rotateEntityAction->setCheckable(true);
    unCheckAllCommand();
    rotateEntityAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    ;
#else
    scene->rotateEntity();
#endif // QT_PAINTER

}

void MainWindow::symetryEntity()
{
    symetryEntityAction->setCheckable(true);
    unCheckAllCommand();
    symetryEntityAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    ;
#else
    scene->symetryEntity();
#endif // QT_PAINTER

}

void MainWindow::removeEntity()
{
    removeEntityAction->setCheckable(true);
    unCheckAllCommand();
    removeEntityAction->setChecked(true);

#ifdef QT_PAINTER
    //QCAD-QPainter版本
    view->remove();
#else
    foreach(QGraphicsItem * pItem, scene->selectedItems())
    {
        scene->removeItem(pItem);
        delete pItem; //???
    }

#endif // QT_PAINTER
    //
}
