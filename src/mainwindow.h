#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStackedWidget>

#include "notice.h"
#include "mainwindow_admin.h"
#include "objectproxy.h"
#include "dbservice.h"
#include "popup.h"


class QAbstractItemModel;
class QQuickWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    seye::DBService* database() const { return db; }
    void setDatabase(seye::DBService* newDb) { db = newDb; }

    void addModel(QString modelName, QAbstractItemModel* model);

    void setupUi();

    QItemSelectionModel* getPolygonSelection();
//    MainWindow* mainwindow = 0;

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

    void on_pushButton_8_clicked();

signals:
    void resort();

private:
    QQuickWidget* gisWidget;        // Гис виджет
    QTableView* polygonView;        // Таблица зон
    QTableView* objectView;         // Таблица объектов
    seye::Notice* noticeService;    // Сервис уведомлений
    seye::ObjectProxy* objectProxy; // Прокси для объектов
    seye::DBService* db;            // База данных
    Ui::MainWindow *ui;             // юи
    Mainwindow2* window;            // ?
    PopUp *popUp;                   // попап паспортов
    QString userRole;               // Роль пользователя
  //  MainWindow* mainwindow = 0;
};


#endif // MAINWINDOW_H
