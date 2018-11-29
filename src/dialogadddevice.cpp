#include "dialogadddevice.h"
#include "ui_dialogadddevice.h"
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

//    QDate date = QDate::fromString(str,"dd/MM/yyyy");

    QString host = "31.211.74.221";
    QString login = "pradlol";
    QString password = "g1e6111213";
    seye::DBService dblink(host, login, password);

    seye::Passport passport;
    passport.callSign = callSign;
    passport.firstName = firstname;
    passport.lastName = lastname;
    passport.birthday = date;

    if(lastname.isEmpty() || firstname.isEmpty() || callSign.isEmpty() /*|| str.isEmpty()*/){
        QMessageBox::warning(this,"Ошибка", "Заполните все поля");

    }
    else{
        QMessageBox::information(this,"Успех", "Вы успешно добавили объект");

        ui->lastname->clear();
        ui->firstname->clear();
        ui->callSign->clear();
        ui->date1->clear();

           dblink.add(passport); //отправка инфы в бд
//        qDebug() << date ;

//           dblink.isOpen(passport);
//           QVector<seye::passport> passports = dblink.getAllPassports();
//           for(int i = 0; i < passports.size(); ++i)
//               qDebug() << seye::toString(passports[i]) << endl;
//           seye::object object;
//           object.id = "9999999999999990";
//           dblink.add(object, "");
    }
}
