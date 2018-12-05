#include "scenario.h"
#include "ui_scenario.h"

Scenario::Scenario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Scenario)
{
    ui->setupUi(this);
    QDate start = ui->dateTimeStart->date();
    QDate finish = ui->dateTimeStart->date();
//    QDate timer = finish-start;

}

Scenario::~Scenario()
{
    delete ui;
}
