#include "logger.h"

logger::logger(QWidget* log) : QDialog(log)
{
	//Задаём модальный тип окна чтобы юзер случайно не скрыл его
	setModal(1);

	//Задаём пока что фиксированный размер окна
	setFixedSize(800, 600);

	//Менеджер размещения кнопок
	buttonLay = new QHBoxLayout;

	//Менеджер размещения панели с кнопками и текстового поля
	builder = new QVBoxLayout;

	//Всякая приблуда из tab_logs.cpp
	clearLog = new QPushButton(tr("clearLog"));
	saveLog = new QPushButton(tr("saveLog"));
	logHint = new QLabel(tr("logHint"));
	logFrame = new QTextEdit;

	//Настройка подписи
	logHint->setAlignment(Qt::AlignLeft);

	//Сборка кнопок
	buttonLay->addWidget(saveLog);
	buttonLay->addWidget(clearLog);

	//Сборка окна
	builder->addWidget(logHint);
	builder->addWidget(logFrame);
	builder->addLayout(buttonLay);

	//Назначение менеджнера в виджет
	this->setLayout(builder);

}
