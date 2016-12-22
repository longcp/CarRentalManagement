#include "clienteditdialog.h"
#include "ui_clienteditdialog.h"
#include <QToolBar>

ClientEditDialog::ClientEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientEditDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()
                         &~Qt::WindowMaximizeButtonHint);               //去掉最大化按钮
    ui->toolBarFrame->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    this->setWindowTitle("客户资料");
    this->setFixedSize(800, 600);

    mToolBar = new QToolBar(tr("clientToolBar"), this);
    mToolBar->setStyleSheet(
                "background-color: rgb(234,234,234);color:rgb(0,0,0);");
    mToolBar->setLayoutDirection(Qt::LayoutDirectionAuto);
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
    mToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mActSaveExit = new QAction(QIcon(":/menu/icon/ok_64.ico"),
                          tr("保存退出"), this);
    mActSave = new QAction(QIcon(":/menu/icon/save_64.ico"),
                          tr("保存"), this);
    mActEdit = new QAction(QIcon(":/menu/icon/edit_64.ico"),
                          tr("修改"), this);
    mActAdd = new QAction(QIcon(":/menu/icon/add_64.ico"),
                          tr("新增"), this);
    mActPrev = new QAction(QIcon(":/menu/icon/arrow_left_64.ico"),
                          tr("上一条"), this);
    mActNext = new QAction(QIcon(":/menu/icon/arrow_right_64.ico"),
                          tr("下一条"), this);
    mActCancel = new QAction(QIcon(":/menu/icon/cancel_64.ico"),
                          tr("取消"), this);
    mActExit = new QAction(QIcon(":/menu/icon/exit_out_64.ico"),
                          tr("取消"), this);

    mToolBar->addAction(mActSaveExit);
    mToolBar->addAction(mActSave);
    mToolBar->addAction(mActEdit);
    mToolBar->addAction(mActAdd);
    mToolBar->addAction(mActPrev);
    mToolBar->addAction(mActNext);
    mToolBar->addAction(mActCancel);
    mToolBar->addAction(mActExit);

    ui->toolBarVerticalLayout->addWidget(mToolBar);
}

ClientEditDialog::~ClientEditDialog()
{
    delete ui;
}

void
ClientEditDialog::openClientEditDialogSlot()
{
    this->exec();
}

