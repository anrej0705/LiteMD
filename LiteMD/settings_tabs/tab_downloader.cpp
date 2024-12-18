#include "appSettings.h"
#include "global_definitions.h"
extern "C"
{
	#include "globalFlags.h"
}

void appSettings::configureDownloaderSettingsTab()
{
	downloaderSettings = new QWidget;

	//Инициализируем метки
	allowWarningsHint = new QLabel(tr("Show one-time deprecated warning"));
	allowCacheHint = new QLabel(tr("Enable content caching"));

	//Инициализируем элементы взаимодействия
	allowWarnings = new QCheckBox;
	allowCache = new QCheckBox;

	//Отрубаем до реализации механики
	allowCache->setChecked(0);
	allowCache->setDisabled(1);

	allowWarnings->setChecked(1);

	//Задаем высоту
	allowWarningsHint->setFixedHeight(SETTINGS_HEIGH);
	allowCacheHint->setFixedHeight(SETTINGS_HEIGH);
	allowWarnings->setFixedHeight(SETTINGS_HEIGH);
	allowCache->setFixedHeight(SETTINGS_HEIGH);

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
	lbl_lay->addWidget(allowWarningsHint);
	lbl_lay->addWidget(allowCacheHint);

	//Подключение системы "Что это?"
	allowWarnings->setWhatsThis(tr("allowWarningsHelp"));
	allowCache->setWhatsThis(tr("allowCacheHelp"));

	interact_lay->addWidget(allowWarnings);
	interact_lay->addWidget(allowCache);

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
	downloader_box_lay->addWidget(lbl_box);
	downloader_box_lay->addWidget(interact_box);

	//Прикрепляем скомпонованные элементы
	downloader_box->setLayout(downloader_box_lay);
	manager->addWidget(downloader_box);

	downloaderSettings->setLayout(manager);
}
