#include "device.h"
#include "ui_device.h"
#include <QMessageBox>
#include <QString>


Device::Device(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Device)
{
    ui->setupUi(this);
}

Device::~Device()
{
    delete ui;
}

void Device::on_buttonBox_accepted()
{
    QString id = ui->id->text();      //получение данных из строк
    QString role = ui->role->text();
    QString phone = ui->phone->text();
    QString nik = ui->nik->text();//
    int k = ui->tipe->currentIndex();

    if(id.isEmpty() || role.isEmpty() || phone.isEmpty() || nik.isEmpty()){  //проверка на заполненость
        QMessageBox::warning(this,"Ошибка", "Заполните все поля");
    }
    else{
        QMessageBox::information(this,"Успех", "Вы успешно добавили объект");
//        qDebug() << speed;

            ui->id->clear();   // очищение строк
            ui->role->clear();
            ui->nik->clear();
            ui->phone->clear();
            if(k ==0){
              int speed=5;
            }
            if(k ==1){
              int speed=27;
            }

    //        dblink.addPassport(callSign, firstName, lastName, date); //отправка инфы в бд
        }
}

void Device::on_buttonBox_rejected()
{
    QMessageBox::about(this, "Отмена","Операция отменена");
    Device::close();
}
