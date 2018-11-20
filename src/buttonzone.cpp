#include <QtGui>
#include "buttonzone.h"
#include <QApplication>
//#include <QDialog>
#include "zoneinfo.h"

 ButtonZone::ButtonZone(QObject *parent)
     : QItemDelegate(parent)
 {
 }

 void ButtonZone::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
 {

     if (index.column() == 4) {

     QStyleOptionButton button;
     QRect r = option.rect;//getting the rect of the cell
     int x,y,w,h;
     x = r.left() + r.width() - 30;//the X coordinate
     y = r.top();//the Y coordinate
     w = 30;//button width
     h = 30;//button height
     button.rect = QRect(x,y,w,h);
     button.text = ">^ω^<";
     button.state = QStyle::State_Enabled;

     QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
      }
}
 bool ButtonZone::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
 {
     if( event->type() == QEvent::MouseButtonRelease  )
     {
         if (index.column() == 4) {
         QMouseEvent * e = (QMouseEvent *)event;
         int clickX = e->x();
         int clickY = e->y();

         QRect r = option.rect;//getting the rect of the cell
         int x,y,w,h;
         x = r.left() + r.width() - 30;//the X coordinate
         y = r.top();//the Y coordinate
         w = 30;//button width
         h = 30;//button height

         if( clickX > x && clickX < x + w )
             if( clickY > y && clickY < y + h )
             {
//                 QDialog * d = new QDialog(); // открытие окна
//                 d->setGeometry(0,0,100,100);
//                 d->show();

                 zoneinfo *info = new zoneinfo();
                 info->show();
             }
     }
}

     return true;
 }
