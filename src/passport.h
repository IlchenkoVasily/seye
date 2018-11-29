#ifndef PASSPORT_H
#define PASSPORT_H

#include <QWidget>

namespace Ui {
class Passport;
}

class Passport : public QWidget
{
    Q_OBJECT

public:
    explicit Passport(QWidget *parent = nullptr);
    ~Passport();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Passport *ui;
    Passport *pass;
};

#endif // PASSPORT_H
