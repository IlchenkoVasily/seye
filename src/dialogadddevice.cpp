#include "dialogadddevice.h"
#include "ui_dialogadddevice.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QString>
//#include "structs.h"
#include "dbservice.h" // класс связи с бд
#include <QDate>

//using namespace seye;

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
    QDate date = ui->date1->date();//

    auto dblink = ((MainWindow*)parent())->database();

    seye::Passport passport;
    passport.callSign = callSign;
    passport.firstName = firstname;
    passport.lastName = lastname;
    passport.birthday = date;

    if(lastname.isEmpty() || firstname.isEmpty() || callSign.isEmpty() /*|| str.isEmpty()*/){
        QMessageBox::warning(this,"Ошибка", "Заполните все поля");

    }

        ui->lastname->clear();
        ui->firstname->clear();
        ui->callSign->clear();
        ui->date1->clear();

        try {
            dblink->add(passport);
        } catch (...) {
            qDebug() << "Drop it out";
            delete dblink;
        }

//        if (dblink->add(passport)) //отправка инфы в бд
//            QMessageBox::information(this,"Успех", "Вы успешно добавили объект");
//        else
//            QMessageBox::critical(this,"Ошибка", "Не удалось добавить паспорт");
}
