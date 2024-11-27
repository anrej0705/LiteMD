#include <QtWidgets>
#include <qurl.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include "Downloader.h"
#include "logger_backend.h"
#include <boost/container/string.hpp>
//Базовый конструктор, создает объект загрузчика
Downloader::Downloader(QObject* dobj) : QObject(dobj)
{
	push_log("[HTTP ЗАГРУЗЧИК]Инициализация бекенда");
	//Инициализируем менеджер подключения
	netman = new QNetworkAccessManager(this);
	//Соединяем сигнал менеждера к слоту обрабатывающему сигнал завершения загрузки
	if (!QObject::connect(netman, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotFinished(QNetworkReply*))))
		QErrorMessage::qtHandler();
}
//Инициатор закачки
void Downloader::download(const QUrl& url)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	log_stroke->append("[HTTP ЗАГРУЗЧИК]Загрузка по URL ");
	log_stroke->append(url.toString().toLocal8Bit());
	push_log(log_stroke->c_str());

	//Создаем объект загрузчика и передаем ссылку по которой будем качать
	QNetworkRequest netreq(url);
	//Формируем объект ответа сети и сохраняем там указатель для получения сигнало прогресса
	QNetworkReply* netrep = netman->get(netreq);
	//Подключем сигнал прогресса загрузки нашего файла из сети к слоту который будет отображать на индикаторе загрузки
	if (!QObject::connect(netrep, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64))))
		QErrorMessage::qtHandler();

	delete(log_stroke);
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
