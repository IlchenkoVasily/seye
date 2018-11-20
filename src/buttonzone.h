#ifndef BUTTONZONE_H
#define BUTTONZONE_H
#include <QItemDelegate>


class ButtonZone : public QItemDelegate
{
    Q_OBJECT

public:
    ButtonZone(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

};


#endif // BUTTONZONE_H
