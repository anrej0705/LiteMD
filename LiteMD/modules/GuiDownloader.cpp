#include <QtWidgets>
#include <qurl.h>
#include "Downloader.h"
#include "GuiDownloader.h"
//Базовый конструктор
DownloaderGui::DownloaderGui(QWidget* dwgt) : QWidget(dwgt)
{
	//Устанавливаем заголовок окна модуля
	setWindowTitle(tr("HTTP Download module GUI(Deprecated)"));
	//Инициализируем объекты управления
	dw = new Downloader(this);
	//Инициализируем графическую оболочку
	dwPb = new QProgressBar;
	dwEt = new QLineEdit;
	dwPt = new QPushButton("&Go");
	//Создаем строку и записываем ссылку-пример(12.11.2023 не работает)
	QString strDownloadLink = "http://i122.fastpic.org/thumb/2023/1120/b5/_e3bc95b508c698aeaa35e205a2b8abb5.jpeg";
	//Копируем строку в поле ввода
	dwEt->setText(strDownloadLink);
	//Соединяем сигнал кнопки со слотом инициатора загрузки
	if (!connect(dwPt, SIGNAL(clicked()), SLOT(slotGo())))
		QErrorMessage::qtHandler();
	//Соединяем сигнал прогресса загрузки со слотом отображающем прогресс загрузки
	if (!connect(dw, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(slotDownloadProgress(qint64, qint64))))
		QErrorMessage::qtHandler();
	//Соединяем сигнал завершения со слотом
	if (!connect(dw, SIGNAL(done(const QUrl&, const QByteArray&)), this, SLOT(slotDone(const QUrl&, const QByteArray&))))
		QErrorMessage::qtHandler();
	//Инициализируем сетку для размещения виджетов
	QGridLayout* gLay = new QGridLayout;
	//Размещаем виджеты
	gLay->addWidget(dwEt, 0, 0);
	gLay->addWidget(dwPt, 0, 1);
	gLay->addWidget(dwPb, 1, 0, 1, 1);
	//Устанавливаем наш слой
	setLayout(gLay);
}
//Базовый деструктор
DownloaderGui::~DownloaderGui()
{}
//Слот инициатора загрузки
void DownloaderGui::slotGo()
{
	//Вызываем метод начала загрузки и передаем ссылку из поля ввода
	dw->download(QUrl(dwEt->text()));
}
//Слот принимающий сигналы прогресса загрузки
void DownloaderGui::slotDownloadProgress(qint64 nRecs, qint64 nTotal)
{
	//Если n меньше 0 значит у нас ошибка
	if (nTotal <= 0)
	{
		//Вызываем слот ошибки и прекращаем загрузку
		slotError();
		return;
	}
	//Иначе выставляем значения в процентах
	dwPb->setValue(100 * nRecs / nTotal);
}
//Слот принимающий сигнал о завершении загрузки
void DownloaderGui::slotDone(const QUrl& url, const QByteArray& ba)
{
	//Создаем хранилище имени файла, предварительно убираем из него лишние символы
	QString strFileName = url.path().section('/', -1);
	//Создаем объект файла
	QFile file(url.path().section('/', -1));
	//Если объект создался и открыт то выполняем
	if (file.open(QIODevice::WriteOnly))
	{
		//Записываем полученные данные в файл
		file.write(ba);
		//Закрываем файл
		file.close();
		//Если вдруг этот файл имеет расширение jpg или png то значит это картинка
		if(strFileName.endsWith(".jpg") || strFileName.endsWith(".png") || strFileName.endsWith(".jpeg"))
			//Отображаем картинку
			showPic(strFileName);
	}
}
//Метод выводящий на экран картинку
void DownloaderGui::showPic(const QString& strFileName)
{
	//Создаем объект картинки
	QPixmap pix(strFileName);
	//Настраиваем его уменьшив размер в 2 раза по обеим сторонам. Используем флаги сглажинивая и игнорирования соотн.сторон
	pix = pix.scaled(pix.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	//Создаем виджет напдиси
	QLabel* plbl = new QLabel;
	//Помещаем картинку
	plbl->setPixmap(pix);
	//Замораживаем изменение размера
	plbl->setFixedSize(pix.size());
	//Устанавливаем заголовок окна
	plbl->setWindowTitle(tr("View"));
	//Показываем на экран
	plbl->show();
}
//Слот обработчика ошибок
void DownloaderGui::slotError()
{
	//Вызываем сообщения о наличии ошибки при соедиении или загрузке
	QMessageBox::critical(0, tr("Error"), tr("An error while download is occured"));
}
void DownloaderGui::slotShow()
{
	this->move(190, 80);
	this->resize(800, 80);
	this->show();
	QMessageBox::information(0, tr("And he won't live long..."), tr("This module was added experimentally. In the future it will undergo changes or disappear"));
}
