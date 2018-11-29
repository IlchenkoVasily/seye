#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "mainwindow_admin.h"
#include <popup.h>


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
//    MainWindow* mainwindow = 0;

private slots:
    void on_pushButton_released();

    // Объекты кнопка
    void on_pushButton_2_clicked();

    //
    void on_pushButton_3_clicked();

    // Зоны кнопка
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

private:
    QQuickWidget* gisWidget;
    QTableView* polygonView;
    QTableView* objectView;
    Ui::MainWindow *ui;
    Mainwindow2* window;
    PopUp *popUp;       // попап паспортов
  //  MainWindow* mainwindow = 0;
};


#endif // MAINWINDOW_H
