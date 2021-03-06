/*************************************************
 * Данный модуль реализован Васиным Василием.
 ************************************************/

#ifndef NOTICE_H
#define NOTICE_H

#include <QObject>
#include <QStringListModel>
#include <QList>
#include <QTime>
#include <QListWidget>
#include <QColor>
#include <QDebug>

#include "enums.h"

//id = -1  пустой объект
/*
    Тут сделаны уведомления
    В mainwindow.h объявите уведомления
    seye::notice* noticeService;
    Напишите в mainwindow.cpp
    noticeService = new seye::notice(ui->noticeList);
    Для соединения сигнала со слотом
    connect(this, SIGNAL(<Сигнал, который даёт уведомления>),
            noticeService, SLOT(NoticeAlarm(int, QString, int)));
    Вместо NoticeAlarm можете кинуть NoticeMessage(строка)
    В notice.h соедiните сигнал требующий id нарушителя
    со слотом  outNotice
    connect(newList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(outNotice(QListWidgetItem*)));
*/

namespace seye
{

    class Notice : public QObject
    {
        Q_OBJECT
    public:
        /*
            Здесь заносим листВиджет в класс
            и колдуем над ним
        */
        Notice(QListWidget *newList);

        /*
            Максимальное количество уведомлений
            все остальные удаляются
            (удаляются первые)
        */
        int noticeSize = 100500;

        /*
            Лист id нарушителей. id = -1 показывает,
            что уведомление - просто текст
            и не содержит id
        */
        QList<QString> noticeListId; //лист с id нарушителей

        /*
            Над этим листВиджетом делаем всё
            и кидаем в тот, что на форме
        */
        QListWidget *noticeList;

        /*
            Структура айди объектов и их последний статус
            Структура накрылась
        */
        struct allowId
        {
            QString id;
            QString lastStatus;
        };

        //Вместо массива со структурой
        QList<QString> toAllowStat;
        QList<QString> toAllowId;

        /*
            Массив с цветами для уведомлений
            тут, думаю, понятны
        */
        QColor noticeColor[10] = {
            Qt::white,                  // offline
            Qt::gray,                   // out of attention
            Qt::blue,                   // destroyed
            QColor(),                   // Lost (non-notify)
            QColor(0, 193, 35, 255),    // new
            QColor(),                   // Allowed (non)
            Qt::red                     // intruder
        };

        /*
            Проверяет количество уведомлений
            удаляет лишние(первые)
            оставляет последние
        */
        void NoticeListChecker();

        //последняя информация
        QString lastId;
        State lastNotice;

    public slots:
        /*
            Вызывайте его, он отправляет
            сигнал, ловите его, но вместо
            idObject кидайте что угодно,
            int отдаёт
        */
        void outNotice(QListWidgetItem*);

        /*
            Данный слот принимает уведомление в виде текста
            и выводит его. Больше ничего
        */
        void NoticeMessage(QString NoticeMessage);

        /*
            Данный слот принимает id объекта,
            qstring имя_нарушителя,
            номер краски для уведомления
            Запоминает текущее время и выводит красиво(?).
        */
        void NoticeAlarm(QString idObject, QString nameObject, State idNotice);

    signals:
        /*
            Сигнал на отправку id нарушителя
            если id = -1, то этот объект
            просто сообщение не объект
        */
        void outNoticeInfo(QString idObject);
    };
}
#endif // NOTICE_H
