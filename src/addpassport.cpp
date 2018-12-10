#include "addpassport.h"
#include "ui_addpassport.h"

#include <QMessageBox>

AddPassport::AddPassport(seye::DBService *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPassport)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    dblink = db;
    ui->lineEditDevice->hide();
    ui->labelNotif->hide();
}

AddPassport::~AddPassport()
{
    delete ui;
}

void AddPassport::on_buttonBox_accepted()
{
    seye::Passport passport;
    passport.firstName = ui->lineEditFirstname->text();
    passport.lastName = ui->lineEditLastname->text();
    passport.callSign = ui->lineEditCallSign->text();
    passport.birthday = ui->editDate->date();
    if (ui->checkBox->isChecked()) passport.device = ui->lineEditDevice->text();

    if (passport.callSign.isEmpty()) QMessageBox::warning(this, "Ошибка", "Поле позывного должно быть заполнено");
    else
        if (passport.device.isEmpty() xor passport.device.size() == 16)
            if (/*passport.id = */dblink->add(passport)) accept(); // Тут надо слать данные в модель таблицы паспортов
            else QMessageBox::warning(this, "Неудачное добавление", "Проверьте введеные данные");
        else QMessageBox::warning(this, "Ошибка", "Поле девайса заполнено не полностью");
}

void AddPassport::on_checkBox_stateChanged(int arg1)
{
    if (arg1)
    {
        ui->lineEditDevice->show();
        ui->labelNotif->show();
    }
    else
    {
        ui->lineEditDevice->hide();
        ui->labelNotif->hide();
    }
}
