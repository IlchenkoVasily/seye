#include "notice.h"

using namespace seye;

void notice::NoticeMessage(QString NoticeMessage)
    {
        /*
            Кидает строку в уведомление
            не красит
            даёт id = -1 ну и проверка ещё
        */
        /*
        * Это задаёт максимальное значение
        * отображаемых уведомлений
        * уберите комментарии и
        * не забудьте задать в notice.h
        * максимальное количество уведомлений
        * показываемое за раз
        noticeListId.append(-1);
        noticeList->addItem(NoticeMessage);
        NoticeListChecker();*/
    }

notice::notice(QListWidget *newList)
{
    noticeList = newList;
    connect(newList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(outNotice(QListWidgetItem*)));
}

void notice::NoticeListChecker()
{
    if(noticeListId.size() > noticeSize)
    {
        /*
            Тут удаляет первые элементы у
            набора строк, айдишников,
            отображаемой строки
            если их больше чем можно
            (noticeSize)
        */
        noticeListId.removeFirst();
        noticeListId.removeFirst();
        noticeList->takeItem(0);
    }
}

void notice::NoticeAlarm(int idObject, QString nameObject, int idNotice)
{
    QString noticeListToOut;
    if(idNotice == 0)
        noticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" нарушил рубеж";
    if(idNotice == 1)
        noticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" покинул зону внимания";
    if(idNotice == 2)
        noticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" отключился";
    if(idNotice == 3)
        noticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" появился";
    /*
        кидает id объекта в лист
        кидает форматированную строку в листВиджет
        перекрашивает последний объект
        который только пришёл
        и проверка ещё
    */
    noticeListId.append(idObject);
    noticeList->addItem(noticeListToOut);
    noticeList->item(noticeList->count()-1)->setForeground(noticeColor[idNotice]);
    NoticeListChecker();
}

void notice::outNotice(QListWidgetItem*)
{
    /*
        Берёт id из листа нарушителей и подаёт сигнал
        с ним
    */
    int current = noticeListId[noticeList->currentRow()];

    emit outNoticeInfo(current);
};
/*
void notice::outNoticeInfoEmit(int idObject)
{
    emit notice::outNoticeInfo(idObject);
}*/
//notice::notice()
//{

//}
