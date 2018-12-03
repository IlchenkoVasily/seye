#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QMessageBox>

login::login(QString* role, QWidget *parent) :
    QDialog(parent), role(role),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

int q=0;// флаг верности входа

void login::on_pushButton_clicked()
{
    QString user = ui->username->text();
    QString pass = ui->password->text();

    QString host = "127.0.0.1"; // "31.211.74.221";
    dblink = new seye::DBService(host, user, pass);

    ((MainWindow*)parent())->setDatabase(dblink);

    *role = dblink->getRole(user);
    if(role->isEmpty()) {QMessageBox::warning(this,"Ошибка", "Неверный логин или  пароль");}
    else { q++;// флаг успешно  авторизации
        //        QMessageBox::information(this,"Успех", "Авторизция прошла успешно");
                login::close();}




//    if( user == "zyx" && pass == "qwert"){

//        q++;// флаг успешно  авторизации
////        QMessageBox::information(this,"Успех", "Авторизция прошла успешно");
//        login::close();
//         }
//    else{
//        QMessageBox::warning(this,"Ошибка", "Неверный логин или  пароль");
////        login::closeEvent(QCloseEvent *event)
//        }
}

void login::closeEvent(QCloseEvent *event)
    {
        if(q==0){
            event->ignore();
        }
    }
