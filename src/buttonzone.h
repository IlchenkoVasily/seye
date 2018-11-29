#ifndef BUTTONZONE_H
#define BUTTONZONE_H
#include <QItemDelegate>
#include "popup.h"
#include <QWidget>

class ButtonZone : public QItemDelegate
{
    Q_OBJECT

public:
    ButtonZone(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
private:
    PopUp *popUp;       // попап паспортов
 };

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QComboBox>

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ComboBoxDelegate(QObject *parent = 0);
//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QStringList colorNames ;


};

#endif // BUTTONZONE_H
