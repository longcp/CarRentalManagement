#include "clientmanagermentwidget.h"
#include "ui_clientmanagermentwidget.h"
#include <QToolBar>

#define LOG_TAG                 "CLIENT_MANAGERMENT_WIDGET"
#include "utils/Log.h"

ClientManagermentWidget::ClientManagermentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientManagermentWidget)
{
    ui->setupUi(this);

    mToolBar = new QToolBar(tr("clientToolBar"), this);
    mToolBar->setStyleSheet("background-color: rgb(255,197,49);color:rgb(0,0,0);");
    mToolBar->setLayoutDirection(Qt::LeftToRight);
    mToolBar->setIconSize(QSize(24,24));
    mToolBar->setOrientation(Qt::Horizontal);
    mToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    mToolBar->setFloatable(true);
    mToolBar->setMovable(true);
    mToolBar->setEnabled(true);
    mToolBar->setGeometry(0, 0, 0, 0);
    mToolBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    mToolBar->setFocusPolicy(Qt::NoFocus);
    mToolBar->setContextMenuPolicy(Qt::DefaultContextMenu);
    mToolBar->setInputMethodHints(Qt::ImhNone);
    mToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QAction *actionFrontView    = new QAction(QIcon("images\3D\front view.ico"),tr("前视图"), this);
    QAction *actionRearView     = new QAction(QIcon("images\3D\rear view.ico"),tr("后视图"), this);
    QAction *actionLeftView     = new QAction(QIcon("images\3D\left view.ico"),tr("左视图"), this);
    QAction *actionRightView    = new QAction(QIcon("images\3D\right view.ico"),tr("右视图"), this);
    QAction *actionTopView      = new QAction(QIcon("images\3D\top view.ico"),tr("俯视图"), this);
    QAction *actionBottomView   = new QAction(QIcon("images\3D\bottom view.ico"),tr("仰视图"), this);

    QAction *actionSWView       = new QAction(QIcon("images\3D\sw iso.ico"),tr("西南轴测图"), this);
    QAction *actionNWView       = new QAction(QIcon("images\3D\nw iso.ico"),tr("西北轴测图"), this);
    QAction *actionSEView       = new QAction(QIcon("images\3D\se iso.ico"),tr("东南轴测图"), this);
    QAction *actionNEView       = new QAction(QIcon("images\3D\ne iso.ico"),tr("东北轴测图"), this);

    QAction *actionZoomAll      = new QAction(QIcon("images\3D\zoomAll.png"),tr("全局缩放"), this);
    QAction *actionZoomIn       = new QAction(QIcon("images\3D\zoomIn.png"),tr("放大"), this);
    QAction *actionZoomOut      = new QAction(QIcon("images\3D\zoomOut.png"),tr("缩小"), this);
    QAction *actionZoomTwoPoint = new QAction(QIcon("images\3D\zoomTwoPoint.png"),tr("局部缩放"), this);
    QAction *actionMove         = new QAction(QIcon("images\3D\cursor_drag_arrow.png"),tr("移动"), this);
    QAction *actionPLTopen      = new QAction(QIcon("images\3D\plt.png"),tr("打开plt文件"), this);
    //    QAction *actionNEView = new QAction(QIcon("images\3D\rotateCCW"),tr("旋转"), this);
    mToolBar->addAction(actionFrontView);
    mToolBar->addAction(actionRearView);
    mToolBar->addAction(actionLeftView);
    mToolBar->addAction(actionRightView);
    mToolBar->addAction(actionTopView);
    mToolBar->addAction(actionBottomView);
    mToolBar->addSeparator();
    mToolBar->addAction(actionSWView);
    mToolBar->addAction(actionNWView);
    mToolBar->addAction(actionSEView);
    mToolBar->addAction(actionNEView);
    mToolBar->addSeparator();
    mToolBar->addAction(actionZoomAll);
    mToolBar->addAction(actionZoomIn);
    mToolBar->addAction(actionZoomOut);
    mToolBar->addAction(actionZoomTwoPoint);
    mToolBar->addAction(actionMove);
    mToolBar->addAction(actionPLTopen);
    ui->clientVerticalLayout->addWidget(mToolBar);
}

ClientManagermentWidget::~ClientManagermentWidget()
{
    delete ui;
}

void
ClientManagermentWidget::openClientWidgetSlot()
{
    ALOGD("ClientManagermentWidget::openClientWidgetSlot enter\n");
    this->show();
}
