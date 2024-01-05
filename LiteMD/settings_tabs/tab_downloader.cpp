#include "appSettings.h"
void appSettings::configureDownloaderSettingsTab()
{
	downloaderSettings = new QWidget;

	//Инициализируем метки

	//Инициализируем элементы взаимодействия

	//Отрубаем до реализации механики

	//Инициализируем рамку
	QGroupBox* downloader_box = new QGroupBox;
	downloader_box->setAutoFillBackground(1);

	//Инициализируем рамки для подписей и элементов интерактивного взаимодействия
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//Менеджеры размещения
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//Вспомогательная компоновка
	QHBoxLayout* downloader_box_lay = new QHBoxLayout;
	QHBoxLayout* manager = new QHBoxLayout;

	//Добавляем в компоновку

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
	downloader_box_lay->addWidget(lbl_box);
	downloader_box_lay->addWidget(interact_box);

	//Прикрепляем скомпонованные элементы
	downloader_box->setLayout(downloader_box_lay);
	manager->addWidget(downloader_box);

	downloaderSettings->setLayout(manager);
}
