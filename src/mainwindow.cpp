#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogadddevice.h"
#include "device.h"
#include "polygonmodel.h"
#include "delegate.h"
#include "buttonzone.h"
#include "login.h"

#include <QQmlContext>
#include <QAbstractItemModel>
#include <QQuickWidget>
#include <QItemSelectionModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    login user(this);
//    user.setModal(true);
//    user.exec();

    //
    ui->setupUi(this);
    // Создаём 'гис'-виджет
    gisWidget = new QQuickWidget(this);
    // Создаём боковые представления
    polygonView = new QTableView(this);
    polygonView->setSelectionBehavior(QAbstractItemView::SelectRows);
    objectView = new QTableView(this);
    objectView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // ! Здесь будет создание виджета для уведомлений

    // инит 'гис'-виджета
    gisWidget->setSource(QUrl("qrc:/qml/main.qml"));
    gisWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Добавление в стек виджет побочных виджетов
    ui->mainStackedWidget->addWidget(gisWidget);

    // ставим корректный виджет на отображение
    ui->mainStackedWidget->setCurrentWidget(gisWidget);

    ui->smallStackedWidget->addWidget(polygonView);
    ui->smallStackedWidget->addWidget(objectView);

    // Ставим корректный виджет на отображение (а должен быть уведомлений)
    ui->smallStackedWidget->setCurrentWidget(polygonView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addModel(QString name, QAbstractItemModel *model)
{
    // Для заброса модели в qml нужно получить контекст
    QQmlContext* context = gisWidget->rootContext();
    // и закинуть)0
    context->setContextProperty(name, model);

    // Сразу забрасываем модель в боковое представление
    if (name.contains("poly"))
    {
//        ButtonZone* infozone = new ButtonZone(this);
//        polygonView->setItemDelegateForColumn(4, infozone);// кнопка открытия паспорта
//        polygonView->setModel(model);
//        QItemSelectionModel* selectionModel = polygonView->selectionModel();
//        context->setContextProperty("polygonSelection", selectionModel);

        ButtonZone* infozone = new ButtonZone(this);
        ComboBoxDelegate* box = new ComboBoxDelegate(this);
        polygonView->setItemDelegateForColumn(3, infozone);// кнопка открытия паспорта
        polygonView->setItemDelegateForColumn(1, box);// цвет поля
        polygonView->setItemDelegateForColumn(2, box);// цвет края поля

        polygonView->setModel(model);
        QItemSelectionModel* selectionModel = polygonView->selectionModel();
        context->setContextProperty("polygonSelection", selectionModel);
    }
    if (name.contains("obj"))
    {
        objectView->setModel(model);
        MyDelegate* delegate = new MyDelegate(this);

        objectView->setItemDelegateForColumn(2, delegate);// кнопка открытия паспорта
    }
}

QItemSelectionModel *MainWindow::getPolygonSelection()
{
    return polygonView->selectionModel();
}

void MainWindow::on_pushButton_released()
{
    DialogAddDevice dia(this);
    dia.setModal(true);
    dia.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->smallStackedWidget->setCurrentWidget(objectView);
}

void MainWindow::on_pushButton_3_clicked()
{
    Device dev(this);
    dev.setModal(true);
    dev.exec();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->smallStackedWidget->setCurrentWidget(polygonView);
}
