#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    QString user = ui->username->text();
    QString pass = ui->password->text();

    //запрос к бд

//    if(){
//        QMessageBox::warning(this,"Ошибка", "Заполните все поля");
//    }
}
