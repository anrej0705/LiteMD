#include "appSettings.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

void appSettings::configureExtendedTab()
{
	//Создаём объект сразу же чтобы не было головной боли потом
	extendedMd = new QWidget;

	//Инициализируем рамку
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//Менеджер вертикального размещения - текст с логами и две кнопки
	QVBoxLayout* builder = new QVBoxLayout;

	//Менеджер горизонтального размещения - для двух кнопок
	QHBoxLayout* buttonLay = new QHBoxLayout;

	//Инициализация элементов
	parseStrikethroughHint = new QLabel(tr("parseStrikethroughHint"));
	parseStrikethrough = new QCheckBox;

	parseStrikethrough->setChecked(1);

	//Задаем высоту
	parseStrikethroughHint->setFixedHeight(SETTINGS_HEIGH);
	parseStrikethrough->setFixedHeight(SETTINGS_HEIGH);

	//Инициализируем рамку
	QGroupBox* extended_box = new QGroupBox;
	extended_box->setAutoFillBackground(1);

	//Инициализируем рамки для подписей и элементов интерактивного взаимодействия
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//Менеджеры размещения
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//Вспомогательная компоновка
	QHBoxLayout* extended_box_lay = new QHBoxLayout;
	QHBoxLayout* manager = new QHBoxLayout;

	//Добавляем в компоновку
	lbl_lay->addWidget(parseStrikethroughHint);

	//Подключение системы "Что это?"
	parseStrikethrough->setWhatsThis(tr("parseStrikethroughHelp"));

	interact_lay->addWidget(parseStrikethrough);

	//Настраиваем порядок размещения элементов - сверху
	lbl_lay->setAlignment(Qt::AlignTop);
	interact_lay->setAlignment(Qt::AlignTop);

	//Привязываем менеджеры компоновки к виджетам
	lbl_box->setLayout(lbl_lay);
	interact_box->setLayout(interact_lay);

	//Задаем ширину блока
	lbl_box->setFixedWidth(HINTS_WIDTH);
	interact_box->setFixedWidth(INTERACT_WIDTH);

	//Компонуем виджеты с подсказками и элемантами взаимодействия
	extended_box_lay->addWidget(lbl_box);
	extended_box_lay->addWidget(interact_box);

	//Прикрепляем скомпонованные элементы
	extended_box->setLayout(extended_box_lay);
	manager->addWidget(extended_box);

	extendedMd->setLayout(manager);
}
