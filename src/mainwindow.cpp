#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogadddevice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_released()
{
    // указатель на окно создания объекта
    DialogAddDevice* dia = new DialogAddDevice(ui->mdiArea);

    // добавляем его в mdi area
    ui->mdiArea->addSubWindow(dia);
    // выводим
    dia->show();
}
