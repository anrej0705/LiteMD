#include "logger.h"
#include "logger_backend.h"
#include <boost/container/vector.hpp>

logger::logger(QWidget* log) : QDialog(log)
{
	//Задаём модальный тип окна чтобы юзер случайно не скрыл его
	setModal(1);
	this->setWindowTitle(tr("logTitle"));

	//Задаём пока что фиксированный размер окна
	setFixedSize(800, 600);

	//Менеджер размещения кнопок
	buttonLay = new QHBoxLayout;

	//Фильтр событий чтобы ловить событие смены языка
	qApp->installEventFilter(this);

	//Иконка
	this->setWindowIcon(QIcon("icon.ico"));

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
