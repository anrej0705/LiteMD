#include "logger.h"

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
	logFrame = new QTextEdit;

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
}
