#include "dialogadddevice.h"
#include "ui_dialogadddevice.h"
#include <QMessageBox>
#include <QString>
//#include <QtDebug>
//#include <DBService> // класс связи с бд
#include <QDate>

DialogAddDevice::DialogAddDevice(QWidget *parent) :
    QDialog (parent),
    ui(new Ui::DialogAddDevice)
{
    ui->setupUi(this);
}

DialogAddDevice::~DialogAddDevice()
{
    delete ui;
}

void DialogAddDevice::on_buttonBox_2_rejected()
{
    QMessageBox::about(this, "Отмена","Операция отменена");

    DialogAddDevice::close();

}

void DialogAddDevice::on_buttonBox_2_accepted()
{
    QString lastname = ui->lastname->text();
    QString firstname = ui->firstname->text();
    QString callSign = ui->callSign->text();
    QString str = ui->date->text();//не знаю как выводить сразу в date

    QDate date = QDate::fromString(str,"dd/MM/yyyy");

    if(lastname.isEmpty() || firstname.isEmpty() || callSign.isEmpty() || str.isEmpty()){
        QMessageBox::warning(this,"Ошибка", "Заполните все поля");

    }
    else{
        QMessageBox::information(this,"Успех", "Вы успешно добавили объект");

        ui->lastname->clear();
        ui->firstname->clear();
        ui->callSign->clear();
        ui->date->clear();

//        dblink.addPassport(callSign, firstName, lastName, date) //отправка инфы в бд
//        qDebug() << date ;
    }
 //   QWidget::close();
}
