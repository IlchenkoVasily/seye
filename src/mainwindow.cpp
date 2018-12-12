#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polygonmodel.h"
#include "delegate.h"
#include "buttonzone.h"
#include "login.h"
#include "object.h"
#include "objectsmodel.h"
#include "groups.h"
#include "scenario.h"
#include "users.h"
#include "addpassport.h"
#include "adddevice.h"

#include <QQmlContext>
#include <QAbstractItemModel>
#include <QQuickWidget>
#include <QItemSelectionModel>
#include <QProcess>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableView>


MainWindow::MainWindow(seye::DBService* db, QString userRole, QWidget *parent) :
    QMainWindow(parent),
    db(db),
    ui(new Ui::MainWindow),
    userRole(userRole),
    onEditing(false)
{
    ui->setupUi(this);

    // Создаём 'гис'-виджет
    gisWidget = new QQuickWidget(this);

    // Create table for rules
    ruleView = new QTableView(this);
    ruleView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Создаём таблицу для пасспортов
    passportView = new QTableView(this);
    passportView->setSelectionBehavior(QAbstractItemView::SelectRows);

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
    ui->mainStackedWidget->addWidget(passportView);
    ui->mainStackedWidget->addWidget(ruleView);

    // ставим корректный виджет на отображение
    ui->mainStackedWidget->setCurrentWidget(gisWidget);

    // Добавляем мини вьюхи в боковой стэк
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

    if(userRole == "operator") //для роли Оператор
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
        ui->pushButton_2->show();
        ui->pushButton_13->show();
        ui->listWidget->show();
    }

    if(userRole == "admin") //для роли Админ
    {
        ui->pushButton_6->show();
        ui->pushButton_7->show();
        ui->pushButton_8->show();
        ui->pushButton_9->show();
        ui->pushButton_11->show();
        ui->pushButton_12->show();
        ui->pushButton_14->show();
        ui->pushButton_15->show();
        ui->pushButton_16->setIcon(QIcon(":/icons/createZone1.png"));
        ui->pushButton_16->show();
        ui->pushButton_17->show();
        ui->pushButton_5->setText("Зоны (ГИС)");
        ui->buttonBox->show();
        ui->pushButton_2->hide();
        ui->pushButton_13->show();
        ui->listWidget->hide();
    }

    // Timeline
    timelineModel = new QStandardItemModel;
    QList<seye::AccessLine> timeline = db->getAllAccessesForTimeline();

    QStandardItem *item;
    for(int i = 0; i < timeline.size(); ++i)
    {
        item = new QStandardItem(QString(timeline[i].name));
        item->setBackground(QBrush(timeline[i].color));
        timelineModel->setItem(0, i, item);
    }
    ui->tableViewTimeline->setModel(timelineModel);
    ui->tableViewTimeline->resizeColumnsToContents();
    ui->tableViewTimeline->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
        polygonModel = qobject_cast<seye::PolygonModel*>(model);

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
        ui->pushButton_16->setCheckable(true);
        ui->pushButton_16->setChecked(false);
        connect(ui->pushButton_16, SIGNAL(clicked()),
                model, SLOT(beginCreatePolygon()));
        connect(model, SIGNAL(onCreateChanged(bool)),
                ui->pushButton_16, SLOT(toggle()));
        connect(this, SIGNAL(startUpdateData()),
                model, SLOT(updateStarted()));

        (qobject_cast<seye::PolygonModel*>(model))->setDatabase(db);
    }
    // модель объекта
    if (name.contains("obj"))
    {
        objectModel = qobject_cast<seye::ObjectModel*>(model);

        // Создаём прокис для объектов
        objectProxy = new seye::ObjectProxy(this);
        objectProxy->setSourceModel(model);
        // set filter for operator
        bool sortFlag = userRole == "operator" ? true : false;
        objectProxy->setSortingState(sortFlag);
        objectProxy->setFilteringState(sortFlag);

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
        objectView->setItemDelegateForColumn(3, delegate);// кнопка открытия паспорта

        // Устанавливаем моделе селекшен модель для сохранения выделения
        objectModel->setSelectionModel(objectView->selectionModel());

        // вьюхи с моделью для перемещения карты на объект
        connect(objectView, SIGNAL(doubleClicked(const QModelIndex&)),
                objectProxy, SLOT(objectSelected(const QModelIndex&)));

        // модели с уведомлениями
        connect(model, SIGNAL(noticePushed(QString, QString, State)),
                noticeService, SLOT(NoticeAlarm(QString, QString, State)));
    }

    if (name.contains("passport"))
    {
        // save model
        passportModel = qobject_cast<QStandardItemModel*>(model);

        // set model
        passportView->setModel(model);

        // disable vertical numbers on every row
        auto vertHeader = passportView->verticalHeader();
        vertHeader->setVisible(false);

        // centilize every cell
        auto horHeader = passportView->horizontalHeader();
        horHeader->setSectionResizeMode(QHeaderView::Stretch);
        horHeader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    }

    if (name.contains("rule"))
    {
        // save model
        ruleModel = qobject_cast<QStandardItemModel*>(model);

        // set model
        ruleView->setModel(model);

        // disable vertical numbers on every row
        auto vertHeader = ruleView->verticalHeader();
        vertHeader->setVisible(false);

        // centilize every cell
        auto horHeader = ruleView->horizontalHeader();
        horHeader->setSectionResizeMode(QHeaderView::Stretch);
        horHeader->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    }
}


QItemSelectionModel *MainWindow::getPolygonSelection()
{
    return polygonView->selectionModel();
}

seye::PolygonModel *MainWindow::getPolygonModel()
{
    return polygonModel;
}

seye::ObjectModel *MainWindow::getObjectModel()
{
    return objectModel;
}

seye::ObjectProxy *MainWindow::getObjectProxyModel()
{
    return objectProxy;
}

void MainWindow::onObjectsUpdated()
{
    emit resort();
}

void MainWindow::on_pushButton_released()
{
    // TODO Если это не юзается, удали
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->smallStackedWidget->setCurrentWidget(objectView);
}

// Смена пользователя
void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Смена пользователя", "Все несохраненые данные будут потеряны, продолжить?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        delete db;
        userRole = "";
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->smallStackedWidget->setCurrentWidget(polygonView);
    ui->mainStackedWidget->setCurrentWidget(gisWidget);
}

void MainWindow::on_pushButton_4_clicked()
{
    // TODO Если это не юзается, удали
}


void MainWindow::on_searchButton_clicked()
{
    auto table = qobject_cast<QTableView*>(ui->smallStackedWidget->currentWidget());
    auto model = table->model();

    if (model == objectProxy)
    {
        // re-cast to proxy model
        auto proxy = qobject_cast<seye::ObjectProxy*>(model);
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
    if (userRole == "operator")
    {
        if(ui->smallStackedWidget->isVisible())
            ui->pushButton_13->setText("↓");
        else
            ui->pushButton_13->setText("↑");
        ui->smallStackedWidget->setVisible(!ui->smallStackedWidget->isVisible());
    }
    else
    {
        if(ui->listWidget->isVisible())
            ui->pushButton_13->setText("↑");
        else
            ui->pushButton_13->setText("↓");
        ui->listWidget->setVisible(!ui->listWidget->isVisible());
    }
}


// Добавление объекта
void MainWindow::on_pushButton_15_clicked()
{
    AddDevice form(this);
    form.setModal(true);
    form.exec();
}

// Добавление паспорта
void MainWindow::on_pushButton_14_clicked()
{
    AddPassport form(database(), this);
    form.setModal(true);
    form.exec();

    // check for new passport
    seye::Passport* pass;
    if ((pass = form.getPassport()))
    {
        auto number    = new QStandardItem(QString::number(passportModel->rowCount()));
        auto lastName  = new QStandardItem(pass->lastName);
        auto firstName = new QStandardItem(pass->firstName);
        auto birth     = new QStandardItem(pass->birthday.toString(QString("dd-MM-yyyy")));
        auto link      = new QStandardItem(pass->callSign);
        auto device    = new QStandardItem(pass->device);

        passportModel->appendRow(QList<QStandardItem*>()
                                  << number << firstName << lastName
                                  << birth  << link      << device);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(passportView);
    ui->smallStackedWidget->setCurrentWidget(objectView);
}

void MainWindow::on_pushButton_12_clicked()
{
    auto table = qobject_cast<QTableView*>(ui->smallStackedWidget->currentWidget());
    auto model = table->model();

    if (model == polygonView->model())
    {
        auto poly = qobject_cast<seye::PolygonModel*>(model);

        int count = table->selectionModel()->selectedRows().count();

        QString message;
        message += "Вы уверены, что хотите удалить ";
        message += QString::number(count);
        message += " зон?";

        QMessageBox::StandardButton reply = QMessageBox::warning(this,
            "Удаление зон", message, QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            poly->deleteSelected();
        }
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    if (onEditing)
    {
        onEditing = false;
        ui->pushButton_11->setText("Редактирование");
        emit startUpdateData();
    }
    else
    {
        onEditing = true;
        ui->pushButton_11->setText("Сохранить");
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    Groups dia(this);
    dia.setModal(true);
    dia.exec();
}

void MainWindow::on_pushButton_17_clicked()
{
    Scenario dia(this);
    dia.setModal(true);
    dia.exec();
}

// управление пользователями
void MainWindow::on_pushButton_9_clicked()
{
    Users form(userRole, database(), this);
    form.setModal(true);
    form.exec();
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ruleView);
}
