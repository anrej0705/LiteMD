#pragma once
#include <QtWidgets>
#include <qurl.h>
#include "Downloader.h"
//Графическая оболочка для управления загрузчиком
class DownloaderGui : public QWidget
{
	Q_OBJECT
	private:
		//Объект загрузчика
		Downloader* dw;
		//Шкала прогресса
		QProgressBar* dwPb;
		//Поле ввода адреса
		QLineEdit* dwEt;
		//Кнопка начала загрузки
		QPushButton* dwPt;
		//Метод для показа загруженной картинки
		void showPic(const QString&);
	public:
		//Базовый конструктор
		DownloaderGui(QWidget* dwgt = 0);
	private slots:
		//Слот инициации загрузки
		void slotGo();
		//Слот обработчика ошибки
		void slotError();
		//Слот обрабатывающий сигналы прогресса загрузки
		void slotDownloadProgress(qint64, qint64);
		//Слот показывающий что загрузка завершена
		void slotDone(const QUrl&, const QByteArray&);
		//Слот, отображающий окно
		void slotShow();
};
