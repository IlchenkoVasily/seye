#include "device.h"
#include "ui_device.h"
#include <QMessageBox>
#include <QString>
#include "dbservice.h"


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
    int k = ui->tipe->currentIndex();
    int speed=0;

    if(id.isEmpty() || role.isEmpty() || phone.isEmpty()){  //проверка на заполненость
        QMessageBox::warning(this,"Ошибка", "Заполните все поля");
    }
//    qint16 role = ui->role->text().toInt();

    else{
        qint16 role = ui->role->text().toInt();
        QMessageBox::information(this,"Успех", "Вы успешно добавили объект");
//        qDebug() << speed;

            ui->id->clear();   // очищение строк
            ui->role->clear();
            ui->phone->clear();
            if(k ==0){
               speed=5;
            }
            if(k ==1){
               speed=27;
            }

            QString host = "31.211.74.221";
            QString login = "pradlol";
            QString password = "g1e6111213";
            seye::DBService dblink(host, login, password);

            seye::ObjectDev object;
            object.id = id;
            object.role = role;
            object.link = phone;
            object.speedLimit = speed;

            dblink.add(object); //отправка инфы в бд
        }
}

void Device::on_buttonBox_rejected()
{
    QMessageBox::about(this, "Отмена","Операция отменена");
    Device::close();
}
