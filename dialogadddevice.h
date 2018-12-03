#ifndef DIALOGADDDEVICE_H
#define DIALOGADDDEVICE_H
#include <QDialog>
#include <QWidget>

namespace Ui {
class DialogAddDevice;
}

class DialogAddDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddDevice(QWidget *parent = nullptr);
    ~DialogAddDevice();

private slots:

    void on_buttonBox_2_rejected();

    void on_buttonBox_2_accepted();

private:
    Ui::DialogAddDevice *ui;
};

#endif // DIALOGADDDEVICE_H
