#include "passport.h"
#include "ui_passport.h"

Passport::Passport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Passport)
{
//    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
}

Passport::~Passport()
{
    delete ui;
}

void Passport::on_pushButton_clicked()
{
   Passport::close();
}
