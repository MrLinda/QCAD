/****************************************************************************
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "diagramitem.h"

#include <QMainWindow>

class DiagramScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
QT_END_NAMESPACE

class QCADView;
//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    //选择命令...
    void selectEntity();
private slots:
    void backgroundButtonGroupClicked(QAbstractButton* button);
    void buttonGroupClicked(int id);
    void deleteItem();
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem* item);
    void textInserted(QGraphicsTextItem* item);
    void currentFontChanged(const QFont& font);
    void fontSizeChanged(const QString& size);
    void sceneScaleChanged(const QString& scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem* item);
    void about();

    //file
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();

    //Zoom
    void zoomIn();
    void zoomOut();
    void zoomAll();

    //创建
    void drawLine();
    void drawCircle();
    void drawArc();
    void drawRect();
    void drawEllipse();
    void drawPolygon();
    void drawText();
    void drawAnnotation();
    void filletWeld();
    void IWeld();
    void VWeld();
    void PlugWeld();
    void spotWeld();
    void standardAnnotation();
    //修改
    void moveEntity();
    void rotateEntity();
    void symetryEntity();
    void removeEntity();

    //视窗
    void zoomWindow();
    void zoomPan();

    //线型
    void lineTypeChanged(QString sType);
    void lineWidChanged(QString sType);

private:
    void unCheckAllCommand();
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    QWidget* createBackgroundCellWidget(const QString& text,
        const QString& image);
    QWidget* createCellWidget(const QString& text,
        DiagramItem::DiagramType type);
    QMenu* createColorMenu(const char* slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString& image, QColor color);
    QIcon createColorIcon(QColor color);

    DiagramScene* scene;

#ifdef QT_PAINTER
    QCADView* view;
#else
    QGraphicsView* view;
#endif // QT_PAINTER

    QAction* exitAction;
    QAction* addAction;
    QAction* deleteAction;

    QAction* toFrontAction;
    QAction* sendBackAction;
    QAction* aboutAction;

    QMenu* fileMenu;
    QMenu* itemMenu;
    QMenu* aboutMenu;
    QMenu* SymbolAnnotationMenu;

    QToolBar* textToolBar;
    QToolBar* editToolBar;
    QToolBar* colorToolBar;
    QToolBar* pointerToolbar;
    //new tool bar
    QToolBar* fileToolBar;
    QToolBar* drawToolBar;
    QToolBar* modifyToolBar;
    QToolBar* zoomToolBar;
    QToolBar* attribToolbar;
    QToolBar* annotationToolbar;

    QComboBox* sceneScaleCombo;
    QComboBox* itemColorCombo;
    QComboBox* textColorCombo;
    QComboBox* fontSizeCombo;
    QFontComboBox* fontCombo;
    QComboBox* lineTypeCombo;//
    QComboBox* lineWidCombo;//

    QToolBox* toolBox;
    QButtonGroup* buttonGroup;
    QButtonGroup* pointerTypeGroup;
    QButtonGroup* backgroundButtonGroup;
    QToolButton* fontColorToolButton;
    QToolButton* fillColorToolButton;
    QToolButton* lineColorToolButton;
    QAction* boldAction;
    QAction* underlineAction;
    QAction* italicAction;
    QAction* textAction;
    QAction* fillAction;
    QAction* lineAction;
    //File Action
    QAction* newFileAction;
    QAction* openFileAction;
    QAction* saveFileAction;
    QAction* saveAsFileAction;
    //Draw Action
    QAction* drawLineAction;
    QAction* drawCircleAction;
    QAction* drawArcAction;
    QAction* drawRectAction;
    QAction* drawEllipseAction;
    QAction* drawPolygonAction;
    QAction* drawTextAction;
    //Modify Action
    QAction* selectEntityAction;
    QAction* regenEntityAction;
    QAction* moveEntityAction;
    QAction* symetryEntityAction;
    QAction* rotateEntityAction;
    QAction* removeEntityAction;
    //Zoom Actions
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* zoomAllAction;
    QAction* zoomWindowAction;
    QAction* zoomPanAction;
    //annotation Actions
    QAction* drawAnnotationAction;
    QAction* FilletweldAction;
    QAction* IweldAction;
    QAction* VweldAction;
    QAction* PlugweldAction;
    QAction* spotweldAction;
    QAction* standardAnnotationAction;
private:
    QString m_sFileName;
};

extern MainWindow* g_pMainWnd;
extern void Prompt(QString sInfo);

//! [0]

#endif // MAINWINDOW_H
