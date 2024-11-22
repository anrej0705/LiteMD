#include "appSettings.h"
#include "global_definitions.h"
extern "C"
{
	#include "globalFlags.h"
}

void appSettings::configureRenderSettingsTab()
{
	renderSettings = new QWidget;

	//Инициализируем метки
	parseSimplLinksHint = new QLabel(tr("Parse URL links"));
	parseAdvLinksHint = new QLabel(tr("Обрабатывать форматированные ссылки"));
	parseHeaderLvlHint = new QLabel(tr("Обрабатывать уровни заголовков"));
	 
	//Инициализируем элементы взаимодействия
	parseSimplLinks = new QCheckBox;
	parseAdvLinksl = new QCheckBox;
	parseHeaderLvl = new QCheckBox;

	//Устанавливаем высоту
	parseSimplLinksHint->setFixedHeight(SETTINGS_HEIGH);
	parseAdvLinksHint->setFixedHeight(SETTINGS_HEIGH);
	parseHeaderLvlHint->setFixedHeight(SETTINGS_HEIGH);
	parseSimplLinks->setFixedHeight(SETTINGS_HEIGH);
	parseAdvLinksl->setFixedHeight(SETTINGS_HEIGH);
	parseHeaderLvl->setFixedHeight(SETTINGS_HEIGH);

	parseSimplLinks->setChecked(1);
	parseAdvLinksl->setChecked(1);
	parseHeaderLvl->setChecked(1);

	//Инициализируем рамку
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//Инициализируем рамки для подписей и элементов интерактивного взаимодействия
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//Менеджеры размещения
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//Вспомогательная компоновка
	QHBoxLayout* render_box_lay = new QHBoxLayout;
	QHBoxLayout* manager = new QHBoxLayout;

	//Добавляем в компоновку
	lbl_lay->addWidget(parseSimplLinksHint);
	lbl_lay->addWidget(parseAdvLinksHint);
	lbl_lay->addWidget(parseHeaderLvlHint);

	interact_lay->addWidget(parseSimplLinks);
	interact_lay->addWidget(parseAdvLinksl);
	interact_lay->addWidget(parseHeaderLvl);

	//Настраиваем порядок размещения элементов - сверху
	lbl_lay->setAlignment(Qt::AlignTop);
	interact_lay->setAlignment(Qt::AlignTop);

	//Привязываем менеджеры компоновки к виджетам
	lbl_box->setLayout(lbl_lay);
	interact_box->setLayout(interact_lay);

	//Задаем ширину блока
	lbl_box->setFixedWidth(300);
	interact_box->setFixedWidth(300);

	//Компонуем виджеты с подсказками и элемантами взаимодействия
	//basic_box_lay->setAlignment(Qt::AlignLeft);
	render_box_lay->addWidget(lbl_box);
	render_box_lay->addWidget(interact_box);

	//Прикрепляем скомпонованные элементы
	render_box->setLayout(render_box_lay);
	manager->addWidget(render_box);

	renderSettings->setLayout(manager);
}
