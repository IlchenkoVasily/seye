#include "adddevice.h"
#include "ui_adddevice.h"
#include "mainwindow.h"

#include <QMessageBox>

AddDevice::AddDevice(seye::DBService *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDevice)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    dblink = db;
    ui->radioButtonCar->setIcon(QIcon(":/icons/car.svg"));
    ui->radioButtonFuel->setIcon(QIcon(":/icons/fuel.svg"));
    ui->radioButtonPilot->setIcon(QIcon(":/icons/pilot.svg"));
    ui->radioButtonSecurity->setIcon(QIcon(":/icons/security.svg"));
    ui->radioButtonWorker->setIcon(QIcon(":/icons/worker.svg"));
    ui->radioButtonCar->hide();
    ui->radioButtonFuel->hide();
    ui->radioButtonPilot->hide();
    ui->radioButtonSecurity->hide();
    ui->radioButtonWorker->hide();
}

AddDevice::~AddDevice()
{
    delete ui;
}

void AddDevice::on_comboBox_activated(int index)
{
    if (index == 2)
    {
        speedLimit = 25;
        ui->radioButtonPilot->hide();
        ui->radioButtonSecurity->hide();
        ui->radioButtonWorker->hide();
        ui->radioButtonCar->show();
        ui->radioButtonFuel->show();
    }
    else if (index == 1)
    {
        speedLimit = 5;
        ui->radioButtonCar->hide();
        ui->radioButtonFuel->hide();
        ui->radioButtonPilot->show();
        ui->radioButtonSecurity->show();
        ui->radioButtonWorker->show();
    }
        else
    {
        speedLimit = 0;
        ui->radioButtonCar->hide();
        ui->radioButtonFuel->hide();
        ui->radioButtonPilot->hide();
        ui->radioButtonSecurity->hide();
        ui->radioButtonWorker->hide();
    }
}

void AddDevice::on_buttonBox_accepted()
{
    seye::ObjectDev object;
    object.id = ui->lineEditDevice->text();
    object.speedLimit = speedLimit;
    object.role = role;
    object.link = ui->lineEditLink->text();

    if (object.link.isEmpty()) QMessageBox::warning(this, "Ошибка", "Поле связи должно быть заполнено");
    else
        if (object.id.size() < 16) QMessageBox::warning(this, "Ошибка", "Неверный ID, " + QString::number(object.id.size()) + " < 16");
        else
            if (speedLimit)
                if (dblink->add(object))
                {
                    // sorry for parent->parent
                    auto model = qobject_cast<MainWindow*>(parent()->parent())->getObjectModel();
                    seye::Object obj(object, QString(""));
                    model->addObject(obj);
                    accept(); // Тут надо слать данные в модель списка объектов
                }
                else QMessageBox::warning(this, "Неудачное добавление", "Возможно id повторяется");
            else QMessageBox::warning(this, "Ошибка", "Выберите тип объекта");
}

void AddDevice::on_radioButtonPilot_clicked(bool checked)
{
    if (checked) role = 3;
}

void AddDevice::on_radioButtonWorker_clicked(bool checked)
{
    if (checked) role = 0;
}

void AddDevice::on_radioButtonFuel_clicked(bool checked)
{
    if (checked) role = 2;
}

void AddDevice::on_radioButtonSecurity_clicked(bool checked)
{
    if (checked) role = 4;
}

void AddDevice::on_radioButtonCar_clicked(bool checked)
{
    if (checked) role = 1;
}
