#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogadddevice.h"
#include "device.h"
#include "polygonmodel.h"
#include "delegate.h"
#include "buttonzone.h"
#include "login.h"
#include "dbservice.h"

#include <QQmlContext>
#include <QAbstractItemModel>
#include <QQuickWidget>
#include <QItemSelectionModel>
#include <QProcess>
#include <QMessageBox>
#include <QHeaderView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    seye::DBService* dblink;
    QString role;

    login user(dblink, &role, this);
    user.setModal(true);
    user.exec();

    //
    ui->setupUi(this);
    // Создаём 'гис'-виджет
    gisWidget = new QQuickWidget(this);
    // Создаём боковые представления
    polygonView = new QTableView(this);
    polygonView->setSelectionBehavior(QAbstractItemView::SelectRows);

    objectView = new QTableView(this);
    objectView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Здесь создание виджета для уведомлений
    noticeService = new seye::Notice(ui->listWidget);

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
    // модель полигона
    if (name.contains("poly"))
    {
        ButtonZone* infozone = new ButtonZone(this);
        ComboBoxDelegate* box = new ComboBoxDelegate(this);
        polygonView->setItemDelegateForColumn(3, infozone);// кнопка открытия паспорта
        polygonView->setItemDelegateForColumn(1, box);// цвет поля
        polygonView->setItemDelegateForColumn(2, box);// цвет края поля

        polygonView->setModel(model);
        QItemSelectionModel* selectionModel = polygonView->selectionModel();
        context->setContextProperty("polygonSelection", selectionModel);

        auto header = polygonView->horizontalHeader();
        header->setSectionsClickable(false);
        header->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(polygonView, SIGNAL(doubleClicked(const QModelIndex&)),
                model, SLOT(polygonLook(const QModelIndex&)));
    }
    // модель объекта
    if (name.contains("obj"))
    {
        // Создаём прокис для объектов
        objectProxy = new seye::ObjectProxy(this);
        objectProxy->setSourceModel(model);
        connect(this, SIGNAL(resort()),
                objectProxy, SLOT(invalidate()));

        // устанавливаем нашу прокси модель вместо модели
        objectView->setModel(objectProxy);

        // Ставим сортировку (по статусу).
        objectView->setSortingEnabled(true);
        objectView->sortByColumn(1, Qt::DescendingOrder);
        // Отключаем клик у хедера таблицы
        auto header = objectView->horizontalHeader();
        header->setSectionsClickable(false);

        // Ставим делегат
        MyDelegate* delegate = new MyDelegate(this);
        objectView->setItemDelegateForColumn(2, delegate);// кнопка открытия паспорта

        // вьюхи с моделью для перемещения карты на объект
        connect(objectView, SIGNAL(doubleClicked(const QModelIndex&)),
                model, SLOT(objectSelected(const QModelIndex&)));

        // модели с уведомлениями
        connect(model, SIGNAL(noticePushed(int, QString, State)),
                noticeService, SLOT(NoticeAlarm(int, QString, State)));
    }
}

QItemSelectionModel *MainWindow::getPolygonSelection()
{
    return polygonView->selectionModel();
}

void MainWindow::onObjectsUpdated()
{
    emit resort();
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
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Смена пользователя", "Вы уверены? Все несохраненые данные будут удалены", QMessageBox::Yes | QMessageBox::No);
    // restart:
    if(reply==QMessageBox::Yes){
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
    else {}
}
void MainWindow::on_pushButton_5_clicked()
{
    ui->smallStackedWidget->setCurrentWidget(polygonView);
}

void MainWindow::on_pushButton_4_clicked()
{
//    window = new Mainwindow2(this);
//    window->show();
    Device dia(this);
    dia.setModal(true);
    dia.exec();
}
