#include <qitemdelegate.h>
#include <passport.h>


class QComboBox;

class MyDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    MyDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QComboBox *typeComboBox;
};
