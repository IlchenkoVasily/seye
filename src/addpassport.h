#ifndef ADDPASSPORT_H
#define ADDPASSPORT_H

#include <QDialog>

#include "dbservice.h"

namespace Ui {
class AddPassport;
}

class AddPassport : public QDialog
{
    Q_OBJECT

public:
    explicit AddPassport(seye::DBService *db, QWidget *parent = nullptr);
    ~AddPassport();

private slots:
    void on_buttonBox_accepted();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::AddPassport *ui;
    seye::DBService *dblink;
};

#endif // ADDPASSPORT_H
