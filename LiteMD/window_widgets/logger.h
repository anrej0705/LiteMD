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

		//Рамка для крЫсоты
		QGroupBox* frame;

		//Костыль для помещения рамки
		QVBoxLayout* frameFix;

		//Поле логов
		QPlainTextEdit* logFrame;

		//Менеджеры компоновки
		QHBoxLayout* buttonLay;
		QVBoxLayout* builder;
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		logger(QWidget* log = 0);
	public slots:
		void slot_read_n_show();
};