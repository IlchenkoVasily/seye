#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogadddevice.h"
#include "device.h"
#include "polygonmodel.h"
#include "objectsmodel.h"
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
    //
    connect(ui->searchBox, SIGNAL(returnPressed()),
            this, SLOT(on_searchButton_clicked()));

    /*
        Здесь проверяется роль и убирает/добавляет
        кнопки которые не нужны для данной роли
    */
    //Соединение сигнала двойного клика по уведомлению с выделением в таблице объектов
    connect(noticeService, SIGNAL(outNoticeInfo(int)), this, SLOT(showObject(int)));

    if(role == "operator") //для роли Оператор
    {
        ui->pushButton_6->hide();
        ui->pushButton_7->hide();
        ui->pushButton_8->hide();
        ui->pushButton_9->hide();
        ui->pushButton_11->hide();
        ui->pushButton_12->hide();
        ui->pushButton_14->hide();
        ui->pushButton_15->hide();
        ui->pushButton_16->hide();
        ui->pushButton_17->hide();
        ui->pushButton_5->setText("Зоны");
        ui->buttonBox->hide();
        ui->pushButtonTest->hide();
        ui->pushButtonSave->hide();
        ui->pushButton_2->show();
        ui->pushButton_13->show();
        ui->listWidget->show();
        ui->pushButton_18->hide();
    }

    if(role == "admin") //для роли Админ
    {
        ui->pushButton_6->show();
        ui->pushButton_7->show();
        ui->pushButton_8->show();
        ui->pushButton_9->show();
        ui->pushButton_11->show();
        ui->pushButton_12->show();
        ui->pushButton_14->show();
        ui->pushButton_15->show();
        ui->pushButton_16->show();
        ui->pushButton_17->show();
        ui->pushButton_5->setText("Зоны (ГИС)");
        ui->pushButton_18->show();
        ui->pushButtonTest->hide();
        ui->pushButtonSave->show();
        ui->buttonBox->show();
        ui->pushButton_2->show();
        ui->pushButton_13->hide();
        ui->listWidget->hide();
    }

    if(role == "supervisor") //для роли СуперАдмина
    {
        ui->pushButton_6->show();
        ui->pushButton_7->show();
        ui->pushButton_8->show();
        ui->pushButton_9->show();
        ui->pushButton_11->show();
        ui->pushButton_12->show();
        ui->pushButton_14->show();
        ui->pushButton_15->show();
        ui->pushButton_16->show();
        ui->pushButton_17->show();
        ui->pushButton_5->setText("Зоны (ГИС)");
        ui->pushButton_18->show();
        ui->pushButtonTest->show();
        ui->pushButtonSave->show();
        ui->buttonBox->show();
        ui->pushButton_3->show();
        ui->pushButton_4->show();
        ui->pushButton_2->show();
        ui->pushButton_13->show();
        ui->searchButton->show();
        ui->listWidget->show();
    }


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

        // Для центрирования
        connect(polygonView, SIGNAL(doubleClicked(const QModelIndex&)),
                model, SLOT(polygonLook(const QModelIndex&)));

        // Для создания полигона
        connect(ui->pushButton_16, SIGNAL(clicked()),
                model, SLOT(beginCreatePolygon()));
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
                objectProxy, SLOT(objectSelected(const QModelIndex&)));

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


void MainWindow::on_searchButton_clicked()
{
    auto table = (QTableView*)ui->smallStackedWidget->currentWidget();
    auto model = table->model();

    if (model == objectProxy)
    {
        // re-cast to proxy model
        auto proxy = (seye::ObjectProxy*)model;
        QRegExp regular(ui->searchBox->text(), Qt::CaseInsensitive);
        proxy->setFilterRegExp(regular);

    }
}

void MainWindow::on_pushButton_13_clicked()
{
    /*
        Прячет виджет с боковой панелью
        от этого увеличивается окно
        с уведомлениями
    */
    if(ui->smallStackedWidget->isVisible())
        ui->pushButton_13->setText("↓");
    else
        ui->pushButton_13->setText("↑");
    ui->smallStackedWidget->setVisible(!ui->smallStackedWidget->isVisible());
}

void MainWindow::on_pushButton_18_clicked()
{
    /*
        Сева, это не костыль
    */
    if(ui->listWidget->isVisible())
        ui->pushButton_18->setText("↑");
    else
        ui->pushButton_18->setText("↓");
    ui->listWidget->setVisible(!ui->listWidget->isVisible());
}

void MainWindow::on_pushButtonSave_clicked()
{
   QMessageBox::StandardButton saveAsk;
   qDebug() << "Asked for save pushed";
   saveAsk = QMessageBox::question(this, "Сохранить", "Сохранить изменения?", QMessageBox::Yes|QMessageBox::No);
   if(saveAsk == QMessageBox::Yes)
   {
       /*
        Здесь код при нажатии Да
       */qDebug() << "Asked for save -> da";
   }
   else
   {
       /*
        Здесь код при нажатии Нет;
       */qDebug() << "Asked for save -> net";
   }
}

void MainWindow::showObject(int idObject)
{
//   ui->smallStackedWidget->setCurrentWidget(objectView);
//   //TODO
//   foreach(ui->smallStackedWidget->currentWidget()->(/*столбец с id*/));
//    {
//     if(/*элемент столбца id*/ ==idObject)
//           ui->smallStackedWidget->setCurrentIndex(/*тут из цикла берём*/);
//    }
    ui->smallStackedWidget->setCurrentWidget(objectView);
    auto proxymodel = qobject_cast<seye::ObjectProxy*>(objectView->model());
    auto model = qobject_cast<seye::ObjectModel*>(proxymodel->sourceModel());
    auto objects = model->toList();

    int idx = objects.indexOf(seye::Object(idObject, 0, 0));

    auto selection = objectView->selectionModel();
    selection->select(model->index(idx, 0), QItemSelectionModel::SelectionFlag::ClearAndSelect);
    //qDebug() << idObject;
}
