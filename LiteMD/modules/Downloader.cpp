#include <QtWidgets>
#include <qurl.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include "Downloader.h"
//Базовый конструктор, создает объект загрузчика
Downloader::Downloader(QObject* dobj) : QObject(dobj)
{
	//Инициализируем менеджер подключения
	netman = new QNetworkAccessManager(this);
	//Соединяем сигнал менеждера к слоту обрабатывающему сигнал завершения загрузки
	if (!QObject::connect(netman, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotFinished(QNetworkReply*))))
		QErrorMessage::qtHandler();
}
//Инициатор закачки
void Downloader::download(const QUrl& url)
{
	//Создаем объект загрузчика и передаем ссылку по которой будем качать
	QNetworkRequest netreq(url);
	//Формируем объект ответа сети и сохраняем там указатель для получения сигнало прогресса
	QNetworkReply* netrep = netman->get(netreq);
	//Подключем сигнал прогресса загрузки нашего файла из сети к слоту который будет отображать на индикаторе загрузки
	if (!QObject::connect(netrep, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64))))
		QErrorMessage::qtHandler();
}
//Слот, вызываемый по завершению загрузки
void Downloader::slotFinished(QNetworkReply* netrep)
{
	//Проверяем статус ошибок. Если есть ошибки то отправляем сигнал об ошибки
	if (netrep->error() != QNetworkReply::NoError)
		emit error();
	else
		//Иначе отправляем сигнал завершения загрузки
		emit done(netrep->url(), netrep->readAll());
	//Отправляем запрос на удаление объекта не моментально
	netrep->deleteLater();
}
