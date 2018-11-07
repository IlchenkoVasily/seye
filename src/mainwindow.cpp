#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogadddevice.h"

#include <QQmlContext>
#include <QAbstractItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->quickWidget->setSource(QUrl("qrc:/qml/main.qml"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addModel(QString name, QAbstractItemModel *model)
{
    QQmlContext* context = ui->quickWidget->rootContext();

    context->setContextProperty(name, model);
}

void MainWindow::on_pushButton_released()
{

}
