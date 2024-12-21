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

		//Красивая рамка
		QGroupBox* frame;

		//Костыль для рамки
		QVBoxLayout* frameFix;

		//Окно логов
		QPlainTextEdit* logFrame;

		//Менеджеры компоновки
		QHBoxLayout* buttonLay;
		QVBoxLayout* builder;
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		logger(QWidget* log = 0);
		void closeEvent(QCloseEvent*);		//Перехватчик закрытия окна
	public slots:
		void slot_read_n_show();			//Копирка текста из стека логов
		void slot_clear_logs();				//Чистим логи
		void slot_save_logs();				//Бекап логов в файл
};