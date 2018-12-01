#include "notice.h"

using namespace seye;

void Notice::NoticeMessage(QString NoticeMessage)
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
        * не забудьте задать в Notice.h
        * максимальное количество уведомлений
        * показываемое за раз
        NoticeListId.append(-1);
        NoticeList->addItem(NoticeMessage);
        NoticeListChecker();*/
    }

Notice::Notice(QListWidget *newList)
{
    noticeList = newList;
    connect(newList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(outNotice(QListWidgetItem*)));
}

void Notice::NoticeListChecker()
{
    if(noticeListId.size() > noticeSize)
    {
        /*
            Тут удаляет первые элементы у
            набора строк, айдишников,
            отображаемой строки
            если их больше чем можно
            (NoticeSize)
        */
        noticeListId.removeFirst();
        noticeListId.removeFirst();
        noticeList->takeItem(0);
    }
}

void Notice::NoticeAlarm(QString idObject, QString nameObject, State idNotice)
{
    QString NoticeListToOut;
    if(idNotice == State::Intruder)
        NoticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" нарушил рубеж";
    if(idNotice == State::OutOfAttention)
        NoticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" покинул зону внимания";
    if(idNotice == State::Destroyed)
        NoticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" отключился";
    if(idNotice == State::New)
        NoticeListToOut = QTime::currentTime().toString() + " \"" + nameObject + "\" появился";
    /*
        кидает id объекта в лист
        кидает форматированную строку в листВиджет
        перекрашивает последний объект
        который только пришёл
        и проверка ещё
    */
    noticeListId.append(idObject);
    noticeList->insertItem(0, NoticeListToOut);
    noticeList->item(0)->setForeground(noticeColor[idNotice]);
    NoticeListChecker();
}

void Notice::outNotice(QListWidgetItem*)
{
    /*
        Берёт id из листа нарушителей и подаёт сигнал
        с ним
    */
    QString current = noticeListId[noticeList->currentRow()];

    emit outNoticeInfo(current);
};
