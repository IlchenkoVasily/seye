#include <QtGui>
#include "buttonzone.h"
#include <QApplication>
//#include <QDialog>
#include "zoneinfo.h"
#include "polygonmodel.h"
#include <QStringList>
#include <QColor>

 ButtonZone::ButtonZone(QObject *parent)
     : QItemDelegate(parent)
 {
 }

 void ButtonZone::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
 {

     if (index.column() == 3) {
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

         if( clickX > x && clickX < x + w )
             if( clickY > y && clickY < y + h )
             {
//                 QDialog * d = new QDialog(); // открытие окна
//                 d->setGeometry(0,0,100,100);
//                 d->show();

                 zoneinfo *info = new zoneinfo();
                 info->show();
//                 nationalityCombo->setModel( new NationalityModel( this ) );

             }
     }
 }

     return true;
 }




 ComboBoxDelegate::ComboBoxDelegate(QObject *parent): QItemDelegate(parent)
 {
 }
int w =0;

 QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 {
     QComboBox *editor = new QComboBox(parent);
     QStringList colorNames;
     colorNames <<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";

     editor ->setFocusPolicy(Qt::NoFocus);
     int size = editor ->style()->pixelMetric(QStyle::PM_SmallIconSize);
     QPixmap pixmap(size,size-5);

     int con=0;
     foreach (const QString &colorName, colorNames) {
         editor ->addItem(colorName);  //Добавляем название цветов
         QColor newColor(colorName);
         if (index.column() == 1) {
         newColor.setAlpha(128);
         }
         pixmap.fill(QColor(newColor));

         QRect rBorder(0,0,size-1,size-6);
         QPainter p(&pixmap);
         QPen pen(Qt::lightGray, 1, Qt::SolidLine);
         p.setPen(pen);
         p.drawRect(rBorder);

         editor->setItemData(con, pixmap, Qt::DecorationRole);//Добавляем изображение цвета в комбо
         con=con+1;
     }

     return editor;

 }

// void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
// {
//     QStringList colorNames ;
//     colorNames << "darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";
//     QStyleOptionComboBox t_style_option_combo_box;
////     w =editor->currentIndex();
//     t_style_option_combo_box.currentText =colorNames.at(w);
//     t_style_option_combo_box.rect = option.rect;

//     QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &t_style_option_combo_box, painter);
//     QApplication::style()->drawItemText(painter,
//                                         t_style_option_combo_box.rect,
//                                         Qt::AlignCenter,
//                                         QApplication::palette(),
//                                         true,
//                                         t_style_option_combo_box.currentText);

// }


 void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
 {
//     QStringList list;
//     list << " qqq" << "(64, 255, 64, 128)" << "Times" << "Courier";
//     QComboBox *comboBox = static_cast<QComboBox*>(editor);
//     comboBox->addItems(list);
//     comboBox->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);


//     QComboBox *comboBox = new QComboBox(editor);


 }

 void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
 {
     QComboBox *comboBox = static_cast<QComboBox*>(editor);
     QString value = comboBox->itemText(comboBox->currentIndex());
     QColor newColor(value);
     if (index.column() == 1) {
     newColor.setAlpha(128);
        }
     model->setData(index, newColor.name(QColor::HexArgb), seye::PolygonModel::ColorRole);
     qDebug()<< newColor;
//     model->setData(index, value, seye::PolygonModel::ColorRole);
 }

 void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index ) const
 {
     editor->setGeometry(option.rect);

 }

