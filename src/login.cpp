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

void login::on_pushButton_clicked()
{
    QString user = ui->username->text();
    QString pass = ui->password->text();

    QString host = "31.211.74.221";
    dblink = new seye::DBService(host, user, pass);

    qobject_cast<MainWindow*>(parent())->setDatabase(dblink);

    *role = dblink->getRole(user);
    if(role->isEmpty()) {QMessageBox::warning(this,"Ошибка", "Неверный логин или  пароль");}
    else { q++;// флаг успешно  авторизации
        //        QMessageBox::information(this,"Успех", "Авторизция прошла успешно");
                login::close();}
}

void login::closeEvent(QCloseEvent *event)
    {
        if(q==0){
            event->ignore();
        }
    }
