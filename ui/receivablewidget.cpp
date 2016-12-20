#include "receivablewidget.h"
#include "ui_receivablewidget.h"

ReceivableWidget::ReceivableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceivableWidget)
{
    ui->setupUi(this);
}

ReceivableWidget::~ReceivableWidget()
{
    delete ui;
}
