#ifndef DIALOGADDDEVICE_H
#define DIALOGADDDEVICE_H

#include <QWidget>

namespace Ui {
class DialogAddDevice;
}

class DialogAddDevice : public QWidget
{
    Q_OBJECT

public:
    explicit DialogAddDevice(QWidget *parent = nullptr);
    ~DialogAddDevice();

private:
    Ui::DialogAddDevice *ui;
};

#endif // DIALOGADDDEVICE_H
