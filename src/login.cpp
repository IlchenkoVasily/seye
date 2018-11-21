#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

int q=0;

void login::on_pushButton_clicked()
{
    QString user = ui->username->text();
    QString pass = ui->password->text();

    if( user == "zyx" && pass == "qwert"){

        q++;// флаг успешно  авторизации
        QMessageBox::information(this,"Успех", "Авторизция прошла успешно");
        login::close();
         }
    else{
        QMessageBox::warning(this,"Ошибка", "Неверный логин или  пароль");
//        login::closeEvent(QCloseEvent *event)
        }
}

void login::closeEvent(QCloseEvent *event)
    {
        if(q==0){
            event->ignore();
        }
    //создаем бокс или просто решаем, что делать, если нажат крестик

    }
