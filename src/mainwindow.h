#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_pushButton_released();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    QQuickWidget* gisWidget;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
