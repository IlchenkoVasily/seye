#include "scenario.h"
#include "ui_scenario.h"
#include <QDebug>
#include <QMessageBox>

Scenario::Scenario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scenario)
{
    ui->setupUi(this);
//    ui->dateTimeStart->QDateTimeEdit::currentDate();

}


Scenario::~Scenario()
{
    delete ui;
}

void Scenario::on_buttonBox_rejected()
{
    QMessageBox::about(this, "Отмена","Операция отменена");

    Scenario::close();
}

void Scenario::on_dateTimeEnd_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime start = ui->dateTimeStart->dateTime();
    QDateTime finish = ui->dateTimeEnd->dateTime();
//    start = QDateTime::fromString(start,"hh.mm.ss-dd.MM.yyyy");
//    timeInterval(start, finish);
    int secs = finish.secsTo(start);
//    int secf = start.hour();

    qDebug() << secs;
    int mins = (secs%360-secs%60)/60;
    int hours=secs/360;
    QTime time(hours, mins);
    qDebug() << hours;
    qDebug() << mins;
    ui->timer->setTime(time);

}

void Scenario::on_dateTimeStart_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime start = ui->dateTimeStart->dateTime();
    QDateTime finish = ui->dateTimeEnd->dateTime();
//    start = QDateTime::fromString(start,"hh.mm.ss-dd.MM.yyyy");
//    timeInterval(start, finish);
    int secs = finish.secsTo(start);
//    int secf = start.hour();

    qDebug() << secs;
    int mins = (secs%3600-secs%60)/60;
    int hours=secs/3600;
    QTime time(hours, mins);
    qDebug() << hours;
    qDebug() << mins;
    ui->timer->setTime(time);

}
