#pragma once
#include <QtWidgets>
#include <qurl.h>
#include <qnetworkaccessmanager.h>
//Класс загрузчика по сети
class Downloader : public QObject
{
	Q_OBJECT
	private:
		//Менеджер сетевых подключений
		QNetworkAccessManager* netman;
	public:
		//Базовый конструктор
		Downloader(QObject* dobj = 0);
		//Инициатор закачки
		void download(const QUrl&);
	signals:
		//Сигнал прогресса загрузки
		void downloadProgress(qint64, qint64);
		//Сигнал завершения загрузки
		void done(const QUrl&, const QByteArray&);
		//Сигнал ошибки подключения
		void error();
	private slots:
		//Слот завершения загрузки
		void slotFinished(QNetworkReply*);
};
