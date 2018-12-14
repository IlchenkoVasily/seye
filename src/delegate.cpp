#include <QtGui>
#include "delegate.h"
#include <QApplication>
//#include <QDialog>
//#include <passport.h>
#include "popup.h"
#include "mainwindow.h"

 MyDelegate::MyDelegate(QObject *parent)
     : QItemDelegate(parent)
 {
 }

 void MyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
 {

     if (index.column() == 3) {

     QStyleOptionButton button;
     QRect r = option.rect;//getting the rect of the cell
     int x,y,w,h;
     x = r.left()/* + r.width() - 30*/;//the X coordinate
     y = r.top();//the Y coordinate
     w = 40;//button width
     h = 30;//button height
     button.rect = QRect(x,y,w,h);
     button.text = "...";
     button.state = QStyle::State_Enabled;

     QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
      }
}

 bool MyDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
 {

     if( event->type() == QEvent::MouseButtonRelease  )
     {
         if (index.column() == 3) {
         QMouseEvent * e = (QMouseEvent *)event;
         int clickX = e->x();
         int clickY = e->y();

         QRect r = option.rect;//getting the rect of the cell
         int x,y,w,h;
         x = r.left() + r.width() - 30;//the X coordinate
         y = r.top();//the Y coordinate
         w = 30;//button width
         h = 30;//button height

//         int q=0; //флаг открытия окна
         if( clickX > x && clickX < x + w )
             if( clickY > y && clickY < y + h  )
                 if (e->button() == Qt::LeftButton)
                 {
                     mainwindow = qobject_cast<MainWindow*>(parent());
                     seye::DBService *db = qobject_cast<MainWindow*>(parent())->database();
                     seye::Passport doc = db->getPassportFor(model->data(index, seye::ObjectModel::IdRole).toString());
                     if (doc.device == nullptr)
                     {
                         doc.firstName = "Девайс";
                         doc.lastName = "не прикреплён";
                         doc.birthday = QDate::currentDate();
                     }
                     popUp = new PopUp(mainwindow);
                     popUp->setPopupText(doc.toStringForPopUp());
//                     clickX=mainwindow-> QWidget::frameGeometry().width();
//                     clickY=mainwindow-> QWidget::frameGeometry().height();
//                     popUp->show(clickX, clickY);
                     popUp->show(0, 0);
                 }
     }
}

     return true;
 }
