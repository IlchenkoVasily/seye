#ifndef ZONEINFO_H
#define ZONEINFO_H

#include <QWidget>

namespace Ui {
class zoneinfo;
}

class zoneinfo : public QWidget
{
    Q_OBJECT

public:
    explicit zoneinfo(QWidget *parent = nullptr);
    ~zoneinfo();

private:
    Ui::zoneinfo *ui;
    zoneinfo *info;
};

#endif // ZONEINFO_H
