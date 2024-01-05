#include "appSettings.h"
void appSettings::configureRenderSettingsTab()
{
	renderSettings = new QWidget;

	//Инициализируем метки
	parseLinksHint = new QLabel(tr("Parse URL links"));
	 
	//Инициализируем элементы взаимодействия
	parseLinks = new QCheckBox;

	//Отрубаем до реализации механики
	//parseLinks->setCheckable(0);
	parseLinks->setChecked(1);
	parseLinks->setDisabled(1);

	//Устанавливаем высоту
	parseLinksHint->setFixedHeight(20);
	parseLinks->setFixedHeight(20);

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
	lbl_lay->addWidget(parseLinksHint);

	interact_lay->addWidget(parseLinks);

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
