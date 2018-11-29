#ifndef DEVICE_H
#define DEVICE_H
#include <QDialog>
#include <QWidget>

namespace Ui {
class Device;
}

class Device : public QDialog
{
    Q_OBJECT

public:
    explicit Device(QWidget *parent = nullptr);
    ~Device();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Device *ui;
};

#endif // DEVICE_H
