#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

login::login(seye::DBService* link, QString* role, QWidget *parent) :
    QDialog(parent), role(role), dblink(link),
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

    QString host = "31.211.74.221";
    dblink = new seye::DBService(host, user, pass);

    *role = dblink->getRole(user);
    *role = user;
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
    //создаем бокс или просто решаем, что делать, если нажат крестик

    }