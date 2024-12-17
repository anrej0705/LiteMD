#include "logger.h"
#include "logger_backend.h"
#include "LiteMD.h"
#include <boost/container/vector.hpp>

logger::logger(QWidget* log) : QDialog(log)
{
	//Задаём модальный тип окна чтобы юзер случайно не скрыл его
	setModal(1);
	this->setWindowTitle(tr("logTitle"));

	//Задаём пока что фиксированный размер окна
	setFixedSize(960, 600);

	//Менеджер размещения кнопок
	buttonLay = new QHBoxLayout;

	//Фильтр событий чтобы ловить событие смены языка
	qApp->installEventFilter(this);

	//Иконка
	QPixmap appIcon(getAppPath() + "/icon.ico");
	appIcon.setMask(appIcon.createMaskFromColor(QColor(0, 0, 0)));
	setWindowIcon(QIcon(appIcon));

	//Менеджер размещения панели с кнопками и текстового поля
	builder = new QVBoxLayout;

	//Всякая приблуда из tab_logs.cpp
	clearLog = new QPushButton(tr("clearLog"));
	saveLog = new QPushButton(tr("saveLog"));
	logFrame = new QPlainTextEdit;

	//Делаем кнопки побольше чтобы тыкать проще
	clearLog->setFixedHeight(48);
	saveLog->setFixedHeight(48);

	//Рамка и её костыль
	frame = new QGroupBox;
	frameFix = new QVBoxLayout;

	//Сборка кнопок
	buttonLay->addWidget(saveLog);
	buttonLay->addWidget(clearLog);

	//Сборка окна
	builder->addWidget(logFrame);

	//Костылинг
	frame->setLayout(builder);
	frame->setTitle(tr("logHint"));
	frameFix->addWidget(frame);
	frameFix->addLayout(buttonLay);

	//Назначение менеджнера в виджет
	this->setLayout(frameFix);

	//Всякие настройки
	logFrame->setReadOnly(1);		//Отключение возможности писать в окно логов

	if (!connect(clearLog, SIGNAL(clicked()), this, SLOT(slot_clear_logs())))
		QErrorMessage::qtHandler();	//Подключаем кнопку очистки логов
	if (!connect(saveLog, SIGNAL(clicked()), this, SLOT(slot_save_logs())))
		QErrorMessage::qtHandler();	//Подключаем кнопку сохранения логов
}

void logger::slot_read_n_show()
{
	boost::container::vector<QString> container = logger_backend::getInstance().get_logs();
	for (uint32_t _index = 0; _index < container.size(); ++_index)
	{
		logFrame->appendPlainText(container.at(_index));
	}
	this->show();
}

void logger::slot_clear_logs()
{	//Чистим логи по запросу
	logger_backend::getInstance().clear_logs();
	logFrame->clear();
}

void logger::slot_save_logs()
{
	QFile mdObject;
	QString mdFileName;
	//Если пусто то выходим
	/*if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		return;
	}*/
	//Вызываем диалоговое окно сохранения
	mdFileName = QFileDialog::getSaveFileName(0, tr("Save logs"), "log", tr("*.txt"));
	//Присваиваем хандлеру имя файла
	mdObject.setFileName(mdFileName);
	//Если удалось открыть файл на запись то выполняем
	if (mdObject.open(QIODevice::WriteOnly))
	{
		//Присваиваем выходному потоку указатель на хандлер и задаем юникод и затем сохраняем
		QTextStream out(&mdObject);
		out.setCodec("UTF-8");
		out << logFrame->toPlainText();
		//Закрываем файл, сбрасываем файл и отсылаем сигнал
		mdObject.close();
	}
}
//Перехватчик события закрытия, пизжен из LiteMD.cpp
void logger::closeEvent(QCloseEvent* ce)
{
	push_log("[QT]Вызвано событие закрытия окна логов");
	logFrame->clear();	//Чистка только UI части логов, при повторном вызове появится снова
	ce->accept();
}

