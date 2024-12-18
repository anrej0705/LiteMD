#include "appSettings.h"
#include "ui_update_event.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

void appSettings::configureLogsTab()
{
	//Инициализация виджета
	tabLogs = new QWidget;

	//Инициализируем рамку
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//Менеджер вертикального размещения - текст с логами и две кнопки
	QVBoxLayout* builder = new QVBoxLayout;
	
	//Менеджер горизонтального размещения - для двух кнопок
	QHBoxLayout* buttonLay = new QHBoxLayout;

	//Инициализация элементов
	clearLog = new QPushButton(tr("clearLog"));
	saveLog = new QPushButton(tr("saveLog"));
	logHint = new QLabel(tr("logHint"));
	logBox = new QPlainTextEdit;

	//Настройка текстового поля
	logBox->setReadOnly(1);

	//Подключение системы "Что это?"
	clearLog->setWhatsThis(tr("clearLogHelp"));
	saveLog->setWhatsThis(tr("saveLogHelp"));
	logBox->setWhatsThis(tr("logBoxHelp"));

	//Настройка подписи
	logHint->setAlignment(Qt::AlignLeft);

	//Сборка кнопок
	buttonLay->addWidget(saveLog);
	buttonLay->addWidget(clearLog);

	//Сборка окна
	builder->addWidget(logHint);
	builder->addWidget(logBox);
	builder->addLayout(buttonLay);

	//Назначение менеджнера в виджет
	tabLogs->setLayout(builder);
}
