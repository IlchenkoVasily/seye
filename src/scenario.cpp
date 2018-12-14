#include "scenario.h"
#include "ui_scenario.h"
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>
//#include <QAbstractItemModel>

#include "groups.h"
#include "mainwindow.h"

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
    access.name = ui->scenarioname->text();
    access.start = ui->dateTimeStart->dateTime();
    access.end = ui->dateTimeEnd->dateTime();
    int k = ui->status->currentIndex();
    QString statuc;
    if (k==0) {statuc = "Обычное";}
    else if (k==1) {statuc = "Приорететное";}
        else if (k==2) {statuc = "Неизменяемое";}
    access.priority = statuc;
    if (dblink->add(access)) accept();
    else QMessageBox::warning(this, "Неудачное добавление", "Проверьте введеные данные");
}

void Scenario::on_pushButton_2_clicked()
{
    Groups temp(this->parentWidget());
    temp.setModal(true);
    temp.exec();
    seye::Group *group = temp.selectedGroup();
    access.group = group->id;
    ui->pushButton_2->setText(group->name);
}

void Scenario::on_pushButton_clicked()
{
    auto model = qobject_cast<MainWindow*>(parent())->getPolygonModel();

    auto widg = new QDialog(this);
    auto layout = new QVBoxLayout(widg);

    auto button = new QPushButton;
    button->setText("Сохранить");

    auto zones = new QTableView(widg);
    zones->setModel(model);
    zones->setSelectionBehavior(QAbstractItemView::SelectRows);
    zones->setSelectionMode(QAbstractItemView::SingleSelection);
    zones->hideColumn(1);
    zones->hideColumn(2);
    zones->hideColumn(3);
    zones->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto label = new QLabel("Выберите зону", this);
    QFont font(label->font());
    font.setWeight(100);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(label);
    layout->addWidget(zones);
    layout->addWidget(button);

    layout->setAlignment(button, Qt::AlignRight);

    widg->setLayout(layout);
    widg->setWindowTitle("Выбор зон");

    connect(button, SIGNAL(clicked()),
            widg, SLOT(close()));

    widg->setModal(true);
    widg->exec();

    //---- after close ----//
    auto selection = zones->selectionModel()->selectedRows();

    if (!selection.isEmpty())
    {
        auto id = model->data(selection[0], seye::PolygonModel::IdRole).toInt();
        auto name = model->data(selection[0], seye::PolygonModel::NameRole).toString();

        access.zone = id;
        ui->pushButton->setText(name);
    }

    delete widg;
}
