#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QListWidget>

#include "notice.h"

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
    void on_pushButton_released();

    // Объекты кнопка
    void on_pushButton_2_clicked();

    //
    void on_pushButton_3_clicked();

    // Зоны кнопка
    void on_pushButton_5_clicked();

    void on_noticeList_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_6_clicked();

private:
    QQuickWidget* gisWidget;
    QTableView* polygonView;
    QTableView* objectView;
    seye::notice* noticeService;
    Ui::MainWindow *ui;
signals:
    void clickNot(int idSend, QString strSend, int idNot);

};

#endif // MAINWINDOW_H
