#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStackedWidget>

#include "notice.h"
#include "objectproxy.h"
#include "dbservice.h"
#include "popup.h"
#include "polygonmodel.h"
#include "objectsmodel.h"


class QAbstractItemModel;
class QQuickWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(seye::DBService* db, QString userRole, QWidget *parent = nullptr);
    ~MainWindow();

    seye::DBService* database() const { return db; }
    void setDatabase(seye::DBService* newDb) { db = newDb; }

    void addModel(QString modelName, QAbstractItemModel* model);

    void setupUi();

    bool isEditEnabled() { return onEditing; }

    QItemSelectionModel* getPolygonSelection();

private slots:
    void onObjectsUpdated();

    void on_pushButton_released();

    // Объекты кнопка
    void on_pushButton_2_clicked();

    //
    void on_pushButton_3_clicked();

    // Зоны кнопка
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    // Кнопка поиска
    void on_searchButton_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    // Кнопка "Объекты и пасспорта"
    void on_pushButton_6_clicked();

    // Кнопка "Удалить"
    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_17_clicked();

signals:
    // Ресорт
    void resort();
    // Сигнал для апдейта всех моделей.
    void startUpdateData();

private:
    Ui::MainWindow *ui;             // юи
    // Виджеты
    QQuickWidget* gisWidget;        // Гис виджет
    QTableView* polygonView;        // Таблица зон
    QTableView* objectView;         // Таблица объектов
    QTableView* passportView;       // Таблица паспортов
    // Данные
    seye::PolygonModel* polygonModel;
    seye::ObjectModel* objectModel; //
    seye::ObjectProxy* objectProxy; // Прокси для объектов
    seye::DBService* db;            // База данных
    // Формы
    PopUp *popUp;                   // попап паспортов
    // Разное
    seye::Notice* noticeService;    // Сервис уведомлений
    QString userRole;               // Роль пользователя
    bool onEditing;                 // Включено редактирование
};


#endif // MAINWINDOW_H
