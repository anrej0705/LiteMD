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
		//Создаем виджет напдиси
		QLabel* plbl;
		bool warned;
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		//Базовый конструктор
		DownloaderGui(QWidget* dwgt = 0);
		//Базовый деструктор
		~DownloaderGui();
		//Событие закрытия окна
		void closeEvent(QCloseEvent*);
	private slots:
		//Слот инициации загрузки
		void slotGo();
		//Слот обработчика ошибки
		void slotError();
		//Слот обрабатывающий сигналы прогресса загрузки
		void slotDownloadProgress(qint64, qint64);
		//Слот показывающий что загрузка завершена
		void slotDone(const QUrl&, const QByteArray&);
	public slots:
		//Слот, отображающий окно
		void slotShow();
};
