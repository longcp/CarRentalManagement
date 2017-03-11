#include "versiondialog.h"
#include "ui_versiondialog.h"
#include <QDate>
#include <QTime>
#include <QDebug>

#define APP_NAME    "CarRentalManagement"
#define VERSION_CODE    " v0.11"

#define TITLE_TEXT  tr(APP_NAME).append(VERSION_CODE)

VersionDialog::VersionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("关于");
    this->setFixedSize(500, 120);
    QDate buildDate = QLocale(QLocale::English)
            .toDate(QString(__DATE__), "MMM dd yyyy");
    QTime buildTime = QTime::fromString(__TIME__, "hh:mm:ss");

    ui->titleLabel->setText(TITLE_TEXT);
    ui->buildDateLabel->setText("Build on "
                                + tr(__DATE__) + "  "
                                + buildTime.toString("hh:mm:ss.zzz"));
    qDebug() << "buildDate = " << buildDate << ", "
             << "buildTime = " << buildTime ;

}

VersionDialog::~VersionDialog()
{
    delete ui;
}

void
VersionDialog::openWindow()
{
    this->exec();
}

void
VersionDialog::mousePressEvent(QMouseEvent *ev)
{
    this->close();
}
