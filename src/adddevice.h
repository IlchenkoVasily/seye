#ifndef ADDDEVICE_H
#define ADDDEVICE_H

#include <QDialog>

#include "dbservice.h"

namespace Ui {
class AddDevice;
}

class AddDevice : public QDialog
{
    Q_OBJECT

public:
    explicit AddDevice(seye::DBService *db, QWidget *parent = nullptr);
    ~AddDevice();

private slots:
    void on_comboBox_activated(int index);

    void on_buttonBox_accepted();

    void on_radioButtonPilot_clicked(bool checked);

    void on_radioButtonWorker_clicked(bool checked);

    void on_radioButtonFuel_clicked(bool checked);

    void on_radioButtonSecurity_clicked(bool checked);

    void on_radioButtonCar_clicked(bool checked);

private:
    Ui::AddDevice *ui;
    seye::DBService *dblink;
    qint16 speedLimit;
    qint16 role;
};

#endif // ADDDEVICE_H
