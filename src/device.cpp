#include "device.h"
#include "ui_device.h"
#include <QMessageBox>
#include <QString>
#include "dbservice.h"
#include "mainwindow.h"


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
    int n = id.size();

    if(n!=16 || role.isEmpty() || phone.isEmpty()){  //проверка на заполненость
        QMessageBox::warning(this,"Ошибка", "Проверьте заполненые поля");
    }
//    qint16 role = ui->role->text().toInt();

    else{
        qint16 role = ui->role->text().toInt();

        auto window = qobject_cast<MainWindow*>(parent());
        auto dblink = window->database();

            ui->id->clear();   // очищение строк
            ui->role->clear();
            ui->phone->clear();
            if(k ==0){
               speed=5;
            }
            if(k ==1){
               speed=27;
            }

            seye::ObjectDev object;
            object.id = id;
            object.role = role;
            object.link = phone;
            object.speedLimit = speed;

            if (true == dblink->add(object)) //отправка инфы в бд
                QMessageBox::information(this, "Успех", "Вы успешно добавили объект");
            else
                QMessageBox::critical(this, "Ошибка", "Не удалось добавить объект");

            // NOTE test adding new object to the model later
            seye::Object okObject(object, QString(""));
            window->getObjectModel()->addObject(okObject);
        }
}

void Device::on_buttonBox_rejected()
{
    QMessageBox::about(this, "Отмена","Операция отменена");
    Device::close();
}
