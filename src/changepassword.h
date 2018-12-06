#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

#include "dbservice.h"

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(const QString& userName, seye::DBService *db, QWidget *parent = nullptr);
    ~ChangePassword();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ChangePassword *ui;
    seye::DBService *dblink;
};

#endif // CHANGEPASSWORD_H
