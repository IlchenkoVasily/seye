#include <QtGui>
#include "buttonzone.h"
#include <QApplication>
//#include <QDialog>
#include "zoneinfo.h"
#include <QStringList>

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

//     if (index.column() == 2) {
//     QStyleOptionButton button;
//     QRect r = option.rect;//getting the rect of the cell
//     int x,y,w,h;
//     x = r.left() + r.width() - 30;//the X coordinate
//     y = r.top();//the Y coordinate
//     w = 30;//button width
//     h = 30;//button height
//     button.rect = QRect(x,y,w,h);
//     button.text = "<^ω^<";
//     button.state = QStyle::State_Enabled;
//     button.palette = Qt::black;

//     QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
//      }
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

//                 zoneinfo *info = new zoneinfo();
//                 info->show();
//                 nationalityCombo->setModel( new NationalityModel( this ) );

             }
     }
 }

     return true;
 }

 ComboBoxDelegate::ComboBoxDelegate(QObject *parent): QItemDelegate(parent)
 {
 }

 QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
 {
     QComboBox *editor = new QComboBox(parent);
     QStringList list;
     list << " qqq" << "Helvetica" << "Times" << "Courier";
//     str = fonts.at(2); // даст строку "Times"
     return editor;
 }

 void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
 {
//     QStringList list;
//     list << " qqq" << "(64, 255, 64, 128)" << "Times" << "Courier";
//     QComboBox *comboBox = static_cast<QComboBox*>(editor);
//     comboBox->addItems(list);
//     comboBox->setItemData(0, QBrush(Qt::red), Qt::TextColorRole);


     QComboBox *comboBox = new QComboBox(editor);
         QStringList colorNames ;
//         QStringList colorNames = QColor::colorNames() ;   //Если нужны ВСЕ цвета, присутствущие в системе, разремируем данную строчку и ремируем следующую. Иначе, используем только те цвета, которые указаны строчкой ниже
         colorNames <<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";

         comboBox ->setFocusPolicy(Qt::NoFocus);
         int size = comboBox ->style()->pixelMetric(QStyle::PM_SmallIconSize);
         QPixmap pixmap(size,size-5);

         int con=0;
         foreach (const QString &colorName, colorNames) {
             comboBox ->addItem(colorName);  //Добавляем название цветов
             pixmap.fill(QColor(colorName));

             QRect rBorder(0,0,size-1,size-6);
             QPainter p(&pixmap);
             QPen pen(Qt::lightGray, 1, Qt::SolidLine);
             p.setPen(pen);
             p.drawRect(rBorder);

             comboBox->setItemData(con, pixmap, Qt::DecorationRole);//Добавляем изображение цвета в комбо
             con=con+1;
         }

 }

 void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
 {
     QComboBox *comboBox = static_cast<QComboBox*>(editor);
     QString value = comboBox->itemText(comboBox->currentIndex());
     model->setData(index, value, Qt::EditRole);
 }

 void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & index ) const
 {
     editor->setGeometry(option.rect);
 }
