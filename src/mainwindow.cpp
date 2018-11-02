#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea(this);  // инициализируем QMdiArea
    // Настраиваем скроллбары
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Устанавливаем Mdi Area в качестве центрального виджета
    setCentralWidget(mdiArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAddWindow_triggered()
{
    // Создаём виджет, который будет окном
    QWidget *widget = new QWidget(mdiArea);
    // Добавляем в него размещение
    QGridLayout *gridLayout = new QGridLayout(widget);
    widget->setLayout(gridLayout);
    // Добавляем на виджет надпись
    QLabel *label = new QLabel("Hello, I am sub window!!!", widget);
    gridLayout->addWidget(label);

    // Добавляем виджет в качестве подокна в Mdi Area
    mdiArea->addSubWindow(widget);
    // Устанавливаем заголовок окна
    widget->setWindowTitle("Sub Window");
    // и показываем виджет
    widget->show();
}
