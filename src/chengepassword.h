#ifndef CHENGEPASSWORD_H
#define CHENGEPASSWORD_H

#include <QDialog>

namespace Ui {
class ChengePassword;
}

class ChengePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChengePassword(QWidget *parent = nullptr);
    ~ChengePassword();

private:
    Ui::ChengePassword *ui;
};

#endif // CHENGEPASSWORD_H
