#include "appSettings.h"
#include "ui_update_event.h"

//Конвертер имени языка - имя_языка(код_языка)
QString localeNameConverter(QString lang_string_name, QString file_loc_name);

//Конфигурируем окно базовых настроек
void appSettings::configureBasicSettingsTab()
{
	//Инициализация подписей
	langListHint = new QLabel(tr("Language"));
	themeHint = new QLabel(tr("UI Theme"));
	saveSettingsHint = new QLabel(tr("Settings save type"));
	autoSaveHint = new QLabel(tr("Autosave"));
	saveFreqHint = new QLabel(tr("Autosave frequency"));

	//Инициализируем элементы взаимодействий
	langList = new QComboBox;
	themeList = new QComboBox;
	saveSettings = new QComboBox;
	autoSave = new QCheckBox;
	saveFreq = new QComboBox;

	//Инициализируем поле
	basicSettings = new QWidget;

	//Инициализируем рамку
	basic_box = new QGroupBox;
	basic_box->setAutoFillBackground(1);

	//Инициализируем рамки для подписей и элементов интерактивного взаимодействия
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//Менеджеры размещения
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//Настраиваем порядок размещения элементов - сверху
	lbl_lay->setAlignment(Qt::AlignTop);
	interact_lay->setAlignment(Qt::AlignTop);

	QHBoxLayout* basic_box_lay = new QHBoxLayout;
	QHBoxLayout* manager = new QHBoxLayout;
	QVBoxLayout* elemManager = new QVBoxLayout;

	//Создаем контейнер пути и прописываем туда путь до файлов локализации
	QString lang_path = QApplication::applicationDirPath();
	lang_path.append("/loc");

	//Указатель на текущую локаль
	QString current_lang = QLocale::system().name();

	//Объект для представления директории loc
	QDir lang_dir(lang_path);

	//Список файлов локалей
	QStringList available_langs = lang_dir.entryList(QStringList("LiteMD_*.qm"));

	loc_map = new std::map<uint8_t, QString>;

	for (uint8_t locales = 0; locales < static_cast<uint8_t>(available_langs.size()); ++locales)
	{
		QString locale_name = available_langs[locales];
		loc_map->insert(std::pair<uint8_t, QString>(locales, locale_name));
		locale_name.truncate(locale_name.lastIndexOf("."));
		locale_name.remove(0, locale_name.indexOf("_",0) + 1);
		QString locale = localeNameConverter(QLocale::languageToString(QLocale(locale_name).language()),locale_name);
		langList->addItem(locale);
		if (current_lang == locale_name)
			langList->setCurrentIndex(locales);
	}

	//Конфигурируем дизайн кнопки и размещаем
	QHBoxLayout* langListManager = new QHBoxLayout;

	//Добавляем элементы в левую половину(подсказки)
	lbl_lay->addWidget(langListHint);
	lbl_lay->addWidget(themeHint);
	lbl_lay->addWidget(saveSettingsHint);
	lbl_lay->addWidget(autoSaveHint);
	lbl_lay->addWidget(saveFreqHint);

	//Добавляем элементы в правую половину(взаимодействие)
	interact_lay->addWidget(langList);
	interact_lay->addWidget(themeList);
	interact_lay->addWidget(saveSettings);
	interact_lay->addWidget(autoSave);
	interact_lay->addWidget(saveFreq);

	//Отключаем элементы, механика которых не реализована
	themeList->addItem(tr("Will be added in future"));
	saveSettings->addItem(tr("Will be added in future"));
	saveFreq->addItem(tr("Will be added in future"));
	autoSave->setChecked(0);
	themeList->setDisabled(1);
	saveSettings->setDisabled(1);
	saveFreq->setDisabled(1);
	autoSave->setCheckable(0);

	//Настраиваем высоту(подгон высоты текста к элементам управления)
	langListHint->setFixedHeight(20);
	themeHint->setFixedHeight(20);
	saveSettingsHint->setFixedHeight(20);
	autoSaveHint->setFixedHeight(20);
	saveFreqHint->setFixedHeight(20);

	langList->setFixedHeight(20);
	themeList->setFixedHeight(20);
	saveSettings->setFixedHeight(20);
	autoSave->setFixedHeight(20);
	saveFreq->setFixedHeight(20);

	//Привязываем менеджеры компоновки к виджетам
	lbl_box->setLayout(lbl_lay);
	interact_box->setLayout(interact_lay);

	//Задаем ширину блока
	lbl_box->setFixedWidth(300);
	interact_box->setFixedWidth(300);

	//Компонуем виджеты с подсказками и элемантами взаимодействия
	//basic_box_lay->setAlignment(Qt::AlignLeft);
	basic_box_lay->addWidget(lbl_box);
	basic_box_lay->addWidget(interact_box);

	//Прикрепляем скомпонованные элементы
	basic_box->setLayout(basic_box_lay);
	manager->addWidget(basic_box);

	basicSettings->setLayout(manager);
}
QString localeNameConverter(QString lang_string_name, QString file_loc_name)
{
	//Создаем буфер в который прилепляем последовательно название языка в системе Qt, скобки и код языка
	//И возвращаем результат
	QString output = QObject::tr(lang_string_name.toLocal8Bit()) + "(" + file_loc_name + ")";
	return output;
}
void appSettings::slot_lang_selected(int lIndx)
{
	int langIndx = 0;
	langIndx = lIndx;
	auto it = loc_map->cbegin();
	std::advance(it, lIndx);
	QString lang_file = it->second;

	if(!lmd_lng.load("loc/"+ lang_file, "."))
		QErrorMessage::qtHandler();
}
