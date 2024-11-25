#pragma once
#include <QtWidgets>
#include "ui_update_event.h"
#include "appSettings.h"

class logger : public QDialog
{
	Q_OBJECT
	private:
		//Кнопки
		QPushButton* clearLog;
		QPushButton* saveLog;

		//Подпись
		QLabel* logHint;

		//Поле логов
		QTextEdit* logFrame;

		//Менеджеры компоновки
		QHBoxLayout* buttonLay;
		QVBoxLayout* builder;
	public:
		logger(QWidget* log = 0);
};