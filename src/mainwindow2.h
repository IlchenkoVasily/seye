#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QWidget>

namespace Ui {
class Mainwindow2;
}

class Mainwindow2 : public QWidget
{
    Q_OBJECT

public:
    explicit Mainwindow2(QWidget *parent = nullptr);
    ~Mainwindow2();

private:
    Ui::Mainwindow2 *ui;
};

#endif // MAINWINDOW2_H
