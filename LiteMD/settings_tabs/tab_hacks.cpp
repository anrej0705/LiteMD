#include "appSettings.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

void appSettings::configureHackTab()
{
	//Создаём объект сразу же чтобы не было головной боли потом
	hacks = new QWidget;

	//Инициализируем рамку
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//Инициализация элементов
	parseUnderlinedHint = new QLabel(tr("parseUnderlinedHint"));
	parseUnderlined = new QCheckBox;

	parseUnderlined->setChecked(1);

	//Задаем высоту
	parseUnderlinedHint->setFixedHeight(SETTINGS_HEIGH);
	parseUnderlined->setFixedHeight(SETTINGS_HEIGH);

	//Инициализируем рамку
	QGroupBox* hacks_box = new QGroupBox;
	hacks_box->setAutoFillBackground(1);

	//Инициализируем рамки для подписей и элементов интерактивного взаимодействия
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//Менеджеры размещения
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//Вспомогательная компоновка
	QHBoxLayout* hacks_box_lay = new QHBoxLayout;
	QHBoxLayout* manager = new QHBoxLayout;

	//Добавляем в компоновку
	lbl_lay->addWidget(parseUnderlinedHint);

	//Подключение системы "Что это?"
	parseUnderlined->setWhatsThis(tr("parseUnderlinedHelp"));

	interact_lay->addWidget(parseUnderlined);

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
	//basic_box_lay->setAlignment(Qt::AlignLeft);
	hacks_box_lay->addWidget(lbl_box);
	hacks_box_lay->addWidget(interact_box);

	//Прикрепляем скомпонованные элементы
	hacks_box->setLayout(hacks_box_lay);
	manager->addWidget(hacks_box);

	hacks->setLayout(manager);
}
