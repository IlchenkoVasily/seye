#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "notice.h"
#include "mainwindow_admin.h"
#include "objectproxy.h"


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

    void addModel(QString modelName, QAbstractItemModel* model);

    void setupUi();

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

signals:
    void resort();

private:
    QQuickWidget* gisWidget;
    QTableView* polygonView;
    QTableView* objectView;
    seye::Notice* noticeService;
    seye::ObjectProxy* objectProxy;
    Ui::MainWindow *ui;
    Mainwindow2* window;
};

#endif // MAINWINDOW_H
