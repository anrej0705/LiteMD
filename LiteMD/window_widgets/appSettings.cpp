#include "appSettings.h"
#include "ui_update_event.h"
#include "event_id_constructor.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
#include <QtWidgets>
#include <boost/container/vector.hpp>
#include "LiteMD.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}
int log_limit = LOGS_LIMIT;
struct parser_switchers parswitch;
struct depr_paerser_switchers dparswitch;

static QString writablePath;			//Путь для записываемых файлов

appSettings::appSettings(QWidget* aWgt) : QDialog(aWgt)
{
	push_log("[QT]Инициализация окна настроек");
	//Окно настроек, будет переписано
	setModal(1);
	setWindowTitle(tr("LiteMD Settings"));

	//Значение по умолчанию
	dataLocation = 0;

	//Windows платформы - получаем путь для хранения настроек(не админ права)
	switch (dataLocation)
	{
		case 0:
		{
			// 0 - %APPDATA%/Local
			writablePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
			push_log("[НАСТРОЙКИ]Выбран путь сохранения %APPDATA%/Local");
			break;
		}
		case 1:
		{
			// 1 - %APPDATA%/Roaming
			writablePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
			push_log("[НАСТРОЙКИ]Выбран путь сохранения %APPDATA%/Roaming");
			break;
		}
		case 2:
		{
			// 2 - Реестр
			push_log("[НАСТРОЙКИ]Реестр выбран в качестве хранилища настроек");
			break;
		}
		case 127:
		{
			//Требует прав администратора
			push_log(std::string("[НАСТРОЙКИ]Выбран путь сохранения" + getAppPath().toStdString()));
			writablePath = getAppPath();
			break;
		}
		case 255:
		{
			// 255 - Пользовательский путь
			push_log("[НАСТРОЙКИ]Задан пользовательский путь");
			break;
		}
	}

	//Проверяем существование каталога, если его нет - создаём
	if (!QDir(writablePath).exists())
		if (!QDir().mkdir(writablePath))	//Если каталог не существует, пробуем создать - если не полулочись роняем прогу
			throw exceptionHandler(exceptionHandler::FATAL, "Не удалось создать каталог для хранения настроек, продолжение работы невозможо");

	//Инициализация
	chosenTheme = (char*)calloc(1, sizeof(char));

	//Предварительная настройка вкладок
	configureBasicSettingsTab();
	configureRenderSettingsTab();
	configureDownloaderSettingsTab();
	configureLogsTab();
	configureExtendedTab();
	configureHackTab();

	//Формирование списка тем, но только после настройки вкладок
	//иначе упадёт
	uint8_t keys = 0;
	foreach(QString str, QStyleFactory::keys())
	{
		themeNamesList.insert(std::pair<uint8_t, std::string>(keys, str.toStdString()));
		themeList->addItem(str);
		++keys;
	}

	//Инициализируем указатели
	xmlw = new xmlWriter;
	settingsLister = new QTabWidget(this);
	btnOk = new QPushButton(tr("&Ok"));
	btnCancel = new QPushButton(tr("&Cancel"));
	btnApply = new QPushButton(tr("&Apply"));
	setDefault = new QPushButton(tr("setDefault"));
	controlBtnLay = new QHBoxLayout;
	setDefBtnLay = new QHBoxLayout;
	dialogWindow = new QVBoxLayout;
	btnComposer = new QHBoxLayout;

	//ui_event = new ui_update_event;

	//QCoreApplication::instance()->installEventFilter(qApp);
	//qApp->installEventFilter(new appSettings_event_filter(APP_EVENT_appSettings_UPDATE_EVENT, this));
	qApp->installEventFilter(this);

	//Настройка компоновщиков
	controlBtnLay->setAlignment(Qt::AlignRight);
	setDefBtnLay->setAlignment(Qt::AlignLeft);

	//Настройка кнопок
	btnOk->setFixedWidth(120);
	btnCancel->setFixedWidth(120);
	btnApply->setFixedWidth(120);
	setDefault->setFixedWidth(120);

	//Компонуем говнинку в красивое окошко
	controlBtnLay->addWidget(btnOk);
	controlBtnLay->addWidget(btnCancel);
	controlBtnLay->addWidget(btnApply);
	setDefBtnLay->addWidget(setDefault);
	btnComposer->addLayout(setDefBtnLay);
	btnComposer->addLayout(controlBtnLay);
	dialogWindow->addWidget(settingsLister);
	dialogWindow->addLayout(btnComposer);

	//Иконка
	QPixmap appIcon(getAppPath() + "/icon.ico");
	appIcon.setMask(appIcon.createMaskFromColor(QColor(0, 0, 0)));
	setWindowIcon(QIcon(appIcon));

	//Устанавливаем менеджер как основной виджет
	setLayout(dialogWindow);

	uint8_t connected_signals = 0;

	push_log("[QT->appSettings]Установка связей сигнал-слот");
	//Устанавливаем связи кнопок
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(slot_apply_settings())))
		QErrorMessage::qtHandler();	++connected_signals;//Кнопка применить изменения
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();	++connected_signals;//Кнопка "ок"
	if (!connect(btnCancel, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();	++connected_signals;//Кнопка закрыть
	if (!connect(langList, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_lang_selected(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Сигнал смены номера локали
	if (!connect(btnApply, SIGNAL(clicked()), this, SLOT(slot_apply_settings())))
		QErrorMessage::qtHandler();	++connected_signals;//Кнопка применить настройки
	if (!connect(allowWarnings, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_warn_allow(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель предупреждений из устаревшего HTTP
	if (!connect(parseSimplLinks, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_simple_url_parser(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации парсера <www.url.com>
	if (!connect(parseAdvLinksl, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_adv_url_parser(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации парсера (url)[www.url.com]
	if (!connect(parseHeaderLvl, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_header_lvl_parser(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации парсера заголовко
	if (!connect(depFunc, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_deprecated(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель устаревших функций
	if (!connect(devFunc, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_features(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель функций в разработке
	if (!connect(settingsLister, SIGNAL(currentChanged(int)), this, SLOT(slot_tab_changed(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Сигнал о выборе определенной вкладки
	if (!connect(clearLog, SIGNAL(clicked()), this, SLOT(slot_clear_logs())))
		QErrorMessage::qtHandler();	++connected_signals;//Кнопка очистки логов
	if (!connect(saveLog, SIGNAL(clicked()), this, SLOT(slot_save_logs())))
		QErrorMessage::qtHandler();	++connected_signals;//Кнопка сохранения логов
	if (!connect(deprSyntaxPrep, SIGNAL(stateChanged(int)), this, SLOT(slot_dparswitch_en_t_prep(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации препроцессора
	if (!connect(deprSyntaxPost, SIGNAL(stateChanged(int)), this, SLOT(slot_en_t_post(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации постпроцессора
	if (!connect(deprUrlSimplParser, SIGNAL(stateChanged(int)), this, SLOT(slot_en_url_bas_simple(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации парсера <www.url.com>
	if (!connect(deprUrlAdvParser, SIGNAL(stateChanged(int)), this, SLOT(slot_en_url_bas(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации парсера [name](url)
	if (!connect(deprUrlBasParser, SIGNAL(stateChanged(int)), this, SLOT(slot_en_url_adv(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации парсера <http://www.url.com>
	if (!connect(limitSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slot_set_limit(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Сигнал на счётчик лимита колва логов
	if (!connect(parseStrikethrough, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_strikethrough(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель активации парсер тильд
	if (!connect(parseUnderlined, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_underlined(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель ДЕактивации парсинга подчёркнутого текста
	if (!connect(setDefault, SIGNAL(clicked()), this, SLOT(slot_reset_settings())))
		QErrorMessage::qtHandler();	++connected_signals;//Кнопка "По умолчанию"
	if (!connect(combatilibtyUndr, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_compat(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель совместимости рендера
	if (!connect(parseItalic, SIGNAL(stateChanged(int)), this, SLOT(slot_en_italic(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель совместимости рендера
	if (!connect(themeList, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_ui_change(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель совместимости рендера
	if (!connect(parseBold, SIGNAL(stateChanged(int)), this, SLOT(slot_en_bold(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель совместимости рендера
	push_log(std::string("[QT->appSettings]Образовано " + std::to_string(connected_signals) + " связей"));
	

	//Ставим заглушку
	workprogress = new QLabel("<H1>"+tr("Work in progress, come later))")+"</H1>");
	QPixmap workprg_cap(getAppPath() + "\\ress\\work_progress_cap.png");
	QLabel* pxmap = new QLabel;
	capTab = new QWidget;
	QVBoxLayout* vLay = new QVBoxLayout;
	pxmap->setPixmap(workprg_cap);
	capTab->setLayout(vLay);
	pxmap->setAlignment(Qt::AlignCenter);
	workprogress->setAlignment(Qt::AlignCenter);
	vLay->addSpacing(95);
	vLay->addWidget(pxmap);
	vLay->addWidget(workprogress);
	vLay->addSpacing(95);

	//Задаем название вкладки
	settingsLister->addTab(basicSettings, tr("Basic"));
	settingsLister->addTab(renderSettings, tr("Render"));
	settingsLister->addTab(downloaderSettings, tr("Downloader"));
	settingsLister->addTab(tabLogs, tr("Logs"));
	settingsLister->addTab(extendedMd, tr("Extended"));
	settingsLister->addTab(hacks, tr("Hacks"));
	settingsLister->addTab(capTab, tr("Cap"));

	//Задаем фиксированный размер
	setFixedSize(800, 600);
}

void appSettings::slot_apply_settings()
{
	//Устанавливаем локаль
	if (!QCoreApplication::installTranslator(&lmd_lng))
		QErrorMessage::qtHandler();
	qApp->installTranslator(&lmd_lng);
	if (!QCoreApplication::sendEvent(qApp, new event_id_constructor(APP_EVENT_UI_UPDATE_EVENT)))	//Постим событие изменения интерфейса
		QErrorMessage::qtHandler();
	if(settingChanged)
		xmlw->writeConfig();	//Сохраняем в XML
}

void appSettings::slot_switch_simple_url_parser(int bit)
{
	settingChanged = 1;
	parswitch.en_simple_url = static_cast<bool>(bit);
	parswitch.en_simple_url == 0 ? push_log("[НАСТРОЙКИ]Парсер простых ссылок отключён") : push_log("[НАСТРОЙКИ]Парсер простых ссылок включён");
}

void appSettings::slot_switch_adv_url_parser(int bit)
{
	settingChanged = 1;
	parswitch.en_adv_url = static_cast<bool>(bit);
	parswitch.en_adv_url == 0 ? push_log("[НАСТРОЙКИ]Парсер форматированных ссылок отключён") : push_log("[НАСТРОЙКИ]Парсер форматированных ссылок включён");
}

void appSettings::slot_switch_header_lvl_parser(int bit)
{
	settingChanged = 1;
	parswitch.en_header_lvl = static_cast<bool>(bit);
	parswitch.en_header_lvl == 0 ? push_log("[НАСТРОЙКИ]Парсер заголовков отключён") : push_log("[НАСТРОЙКИ]Парсер заголовков включён");
}

void appSettings::slot_switch_warn_allow(int bit)
{
	settingChanged = 1;
	allowHttpWarn = static_cast<bool>(bit);
	allowHttpWarn  == 0 ? push_log("[НАСТРОЙКИ]Парсер заголовков отключён") : push_log("[НАСТРОЙКИ]Парсер заголовков включён");
}

void appSettings::slot_switch_deprecated(int bit)
{
	settingChanged = 1;
	enableDeprFeatures = static_cast<bool>(bit);
	if (::enableDeprFeatures)
	{
		push_log("[НАСТРОЙКИ]Переключение на устаревший функционал");
	}
	update_interactive();
}
void appSettings::slot_dparswitch_en_t_prep(int bit)
{
	settingChanged = 1;
	dparswitch.en_t_prep = static_cast<bool>(bit);
	dparswitch.en_t_prep == 0 ? push_log("[НАСТРОЙКИ]Препроцессор(устаревший) отключён") : push_log("[НАСТРОЙКИ]Препроцессор(устаревший) включён");
}
void appSettings::slot_en_t_post(int bit)
{
	settingChanged = 1;
	dparswitch.en_t_post = static_cast<bool>(bit);
	dparswitch.en_t_post == 0 ? push_log("[НАСТРОЙКИ]Постпроцессор(устаревший) отключён") : push_log("[НАСТРОЙКИ]Постпроцессор(устаревший) включён");
}
void appSettings::slot_en_url_bas_simple(int bit)
{
	settingChanged = 1;
	dparswitch.en_url_bas_simple = static_cast<bool>(bit);
	dparswitch.en_url_bas_simple == 0 ? push_log("[НАСТРОЙКИ]Парсер <www.url.com>(устаревший) отключён") : push_log("[НАСТРОЙКИ]Парсер <www.url.com>(устаревший) включён");
}
void appSettings::slot_en_url_bas(int bit)
{
	settingChanged = 1;
	dparswitch.en_url_bas = static_cast<bool>(bit);
	dparswitch.en_url_bas == 0 ? push_log("[НАСТРОЙКИ]Парсер <http://www.url.com>(устаревший) отключён") : push_log("[НАСТРОЙКИ]Парсер <http://www.url.com>(устаревший) включён");
}
void appSettings::slot_en_url_adv(int bit)
{
	settingChanged = 1;
	dparswitch.en_url_adv = static_cast<bool>(bit);
	dparswitch.en_url_adv == 0 ? push_log("[НАСТРОЙКИ]Парсер форматированных ссылок(устаревший) отключён") : push_log("[НАСТРОЙКИ]Парсер форматированных ссылок(устаревший) включён");
}

void appSettings::slot_switch_features(int bit)
{
	settingChanged = 1;
	enableIndevFeatures = static_cast<bool>(bit);
	/*if (enableIndevFeatures)
	{
		try
		{
			newRecentFilesArray();
		}
		catch (exceptionHandler)
		{
			(exceptionHandler(exceptionHandler::FATAL));
		}
	}
	else
	{
		try
		{
			deleteOnExit();
		}
		catch (exceptionHandler)
		{
			(exceptionHandler(exceptionHandler::FATAL));
		} 
	}*/
	update_interactive();
}

void appSettings::slot_tab_changed(int tab_index)
{
	push_log(std::string("[НАСТРОЙКИ]Смена индекса активной вкладки на " + std::to_string(tab_index)).c_str());
	if (tab_index == 3)
	{
		boost::container::vector<QString> container = logger_backend::getInstance().get_logs();
		for (uint32_t _index = 0; _index < container.size(); ++_index)
		{
			logBox->appendPlainText(container.at(_index));
		}
	}
	else//На всякий случай будем чистить поле логов
	{
		logBox->clear();
	}
}

void appSettings::slot_clear_logs()
{	//Чистка логов
	logger_backend::getInstance().clear_logs();
	logBox->clear();
}

void appSettings::slot_save_logs()
{
	QFile mdObject;
	QString mdFileName;
	//Если пусто то выходим
	/*if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		return;
	}*/
	//Вызываем диалоговое окно сохранения
	mdFileName = QFileDialog::getSaveFileName(0, tr("Save logs"), "log", tr("*.txt"));
	//Присваиваем хандлеру имя файла
	mdObject.setFileName(mdFileName);
	//Если удалось открыть файл на запись то выполняем
	if (mdObject.open(QIODevice::WriteOnly))
	{
		//Присваиваем выходному потоку указатель на хандлер и задаем юникод и затем сохраняем
		QTextStream out(&mdObject);
		out.setCodec("UTF-8");
		out << logBox->toPlainText();
		//Закрываем файл, сбрасываем файл и отсылаем сигнал
		mdObject.close();
	}
}

void appSettings::slot_switch_underlined(int bit)
{
	settingChanged = 1;
	parswitch.en_underlined = static_cast<bool>(bit);
	parswitch.en_underlined == 0 ? push_log("[НАСТРОЙКИ]Обработка подчёркивания отключена") : push_log("[НАСТРОЙКИ]Обработка подчёркивания включена");
}

void appSettings::slot_switch_strikethrough(int bit)
{
	settingChanged = 1;
	parswitch.en_ex_strkthg = static_cast<bool>(bit);
	parswitch.en_ex_strkthg == 0 ? push_log("[НАСТРОЙКИ]Обработка тильды отключена") : push_log("[НАСТРОЙКИ]Обработка тильды включена");
}

void appSettings::slot_en_italic(int bit)
{
	settingChanged = 1;
	parswitch.en_italic = static_cast<bool>(bit);
	parswitch.en_italic == 0 ? push_log("[НАСТРОЙКИ]Обработка курсива отключена") : push_log("[НАСТРОЙКИ]Обработка курсива включена");
}

void appSettings::slot_set_limit(int limit)
{
	//Чтобы 200IQ гений на юзере не сломал прогу зададим минимум равный текущему размеру логов
	limitSpinBox->setMinimum(logger_backend::getInstance().get_size());

	//Задаем лимит от юзера
	if (limit > logger_backend::getInstance().get_size())
		logger_backend::getInstance().set_limit(limit);
	else
		logger_backend::getInstance().set_limit(logger_backend::getInstance().get_size() + 1);
}

//Сброс параметров в дефолтные настройки
void appSettings::slot_reset_settings()
{
	settingChanged = 1;
	parswitch.en_simple_url = 1;
	parswitch.en_adv_url = 1;
	parswitch.en_header_lvl = 1;
	parswitch.en_ex_strkthg = 1;
	parswitch.en_underlined = 1;
	parswitch.en_compat_undr = 1;
	parswitch.en_italic = 1;
	parswitch.en_bold = 1;

	enableDeprFeatures = 0;
	enableIndevFeatures = 0;
	try
	{
		deleteOnExit();
	}
	catch (exceptionHandler)
	{
		(exceptionHandler(exceptionHandler::FATAL));
	}
	allowHttpWarn = 1;

	dparswitch.en_t_post = 0;
	dparswitch.en_t_prep = 0;
	dparswitch.en_url_adv = 0;
	dparswitch.en_url_bas = 0;
	dparswitch.en_url_bas_simple = 0;

	limitSpinBox->setValue(LOGS_LIMIT);
	log_limit = LOGS_LIMIT;
	if (!QCoreApplication::sendEvent(qApp, new event_id_constructor(APP_EVENT_UI_UPDATE_USER_SETTINGS)))
		QErrorMessage::qtHandler();//Отправка события на обновление визуала - галочек, радиокнопок и прочего
	update_interactive();
	update_ui();
}

void appSettings::slot_switch_compat(int bit)
{
	settingChanged = 1;
	parswitch.en_compat_undr = static_cast<bool>(bit);
	parswitch.en_compat_undr == 0 ? push_log("[НАСТРОЙКИ]Режим совместимости отключён(обработка <ins>)") : push_log("[НАСТРОЙКИ]Режим совместимости включён(обработка <u>)");
}

void appSettings::slot_ui_change(int index)
{
	//Переменная для проверки
	char* newPtr;

	//Выделяем блок памяти
	newPtr = (char*)realloc(chosenTheme, sizeof(themeNamesList[index].size()));

	//Если указатель не зашкварился то перемещаем его
	if (newPtr != NULL)
		chosenTheme = newPtr;
	else
		throw(exceptionHandler(exceptionHandler::WARNING, "O KURWA!!1\nНе удалось выделить память для char chosenTheme"));

	//Копируем имя темы
	strcpy(chosenTheme, themeNamesList[index].c_str());

	//Подымаем флаг так как юзер сменил тему
	//uiChanged = 1;

	//Шлём смску
	if (!QCoreApplication::sendEvent(qApp, new event_id_constructor(APP_EVENT_UI_UPDATE_EVENT)))	//Постим событие изменения интерфейса
		QErrorMessage::qtHandler();
}

void appSettings::slot_en_bold(int bit)
{
	settingChanged = 1;
	parswitch.en_bold = static_cast<bool>(bit);
	parswitch.en_bold == 0 ? push_log("[НАСТРОЙКИ]Обработка жирного текста отключена") : push_log("[НАСТРОЙКИ]Обработка жирного текста включена");
}

QString getConfigPath()
{
	return writablePath;
}
