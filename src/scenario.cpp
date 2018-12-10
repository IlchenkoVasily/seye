#include "scenario.h"
#include "ui_scenario.h"
#include <QDebug>
#include <QMessageBox>

Scenario::Scenario(seye::DBService *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scenario)
{
    ui->setupUi(this);
//    ui->dateTimeStart->QDateTimeEdit::currentDate();
    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    dblink = db;
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
    int secs = start.secsTo(finish);


    qDebug() << secs;
    int mins = (secs%3600)/60;
    int hours=secs/3600;
    QTime time(hours, mins);
    QString q = QString::number( hours ) + ":" +  QString::number(mins ) + " " ;
    qDebug() << hours;
    qDebug() << mins;
//    ui->timer->setTime(time);
    ui->timer->setText(/*time.toString("hh:mm")*/q);


}

void Scenario::on_dateTimeStart_dateTimeChanged(const QDateTime &dateTime)
{
    QDateTime start = ui->dateTimeStart->dateTime();
    QDateTime finish = ui->dateTimeEnd->dateTime();
    int secs = start.secsTo(finish);

    qDebug() << secs;
    int mins = (secs%3600)/60;
    int hours=secs/3600;
    QString q = QString::number( hours ) + ":" +  QString::number(mins ) + " " ;
    qDebug() << hours;
    qDebug() << mins;
//    ui->timer->setTime(time);
    ui->timer->setText(q);

}

void Scenario::on_buttonBox_accepted()
{
    seye::Access access;
    access.name = ui->scenarioname->text();
    access.start = ui->dateTimeStart->dateTime();
    access.end = ui->dateTimeEnd->dateTime();
    int k = ui->status->currentIndex();
    QString statuc;
    if (k==0) {statuc = "Обычное";}
    else if (k==1) {statuc = "Приорететное";}
        else if (k==2) {statuc = "Неизменяемое";}
    access.priority = statuc;
    if (/*passport.id = */dblink->add(access)) accept(); // Тут надо слать данные в модель таблицы паспортов
    else QMessageBox::warning(this, "Неудачное добавление", "Проверьте введеные данные");
}
