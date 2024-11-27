#include "appSettings.h"
#include "ui_update_event.h"
#include "event_id_constructor.h"
#include "global_definitions.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
#include <QtWidgets>
#include <boost/container/vector.hpp>
extern "C"
{
	#include "globalFlags.h"
}
struct parser_switchers parswitch;
appSettings::appSettings(QWidget* aWgt) : QDialog(aWgt)
{
	push_log("[QT]Инициализация окна настроек");
	//Окно настроек, будет переписано
	setModal(1);
	setWindowTitle(tr("LiteMD Settings"));

	//Предварительная настройка вкладок
	configureBasicSettingsTab();
	configureRenderSettingsTab();
	configureDownloaderSettingsTab();
	configureLogsTab();

	//Инициализируем указатели
	xmlw = new xmlWriter;
	settingsLister = new QTabWidget(this);
	btnOk = new QPushButton(tr("&Ok"));
	btnCancel = new QPushButton(tr("&Cancel"));
	btnApply = new QPushButton(tr("&Apply"));
	controlBtnLay = new QHBoxLayout;
	dialogWindow = new QVBoxLayout;

	//ui_event = new ui_update_event;

	//QCoreApplication::instance()->installEventFilter(qApp);
	//qApp->installEventFilter(new appSettings_event_filter(APP_EVENT_appSettings_UPDATE_EVENT, this));
	qApp->installEventFilter(this);

	//Настройка компоновщиков
	controlBtnLay->setAlignment(Qt::AlignRight);

	//Настройка кнопок
	btnOk->setFixedWidth(120);
	btnCancel->setFixedWidth(120);
	btnApply->setFixedWidth(120);

	//Добавляем виджет кнопок и вкладок
	controlBtnLay->addWidget(btnOk);
	controlBtnLay->addWidget(btnCancel);
	controlBtnLay->addWidget(btnApply);
	dialogWindow->addWidget(settingsLister);
	dialogWindow->addLayout(controlBtnLay);

	//Иконка
	this->setWindowIcon(QIcon("icon.ico"));

	//Устанавливаем менеджер как основной виджет
	setLayout(dialogWindow);
	
	//Устанавливаем связи кнопок
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(slot_apply_settings())))
		QErrorMessage::qtHandler();	//Кнопка применить изменения
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();	//Кнопка "ок"
	if (!connect(btnCancel, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();	//Кнопка закрыть
	if (!connect(langList, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_lang_selected(int))))
		QErrorMessage::qtHandler();	//Сигнал смены номера локали
	if (!connect(btnApply, SIGNAL(clicked()), this, SLOT(slot_apply_settings())))
		QErrorMessage::qtHandler();	//Кнопка применить настройки
	if (!connect(allowWarnings, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_warn_allow(int))))
		QErrorMessage::qtHandler();	//Переключатель предупреждений из устаревшего HTTP
	if (!connect(parseSimplLinks, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_simple_url_parser(int))))
		QErrorMessage::qtHandler();	//Переключатель активации парсера <www.url.com>
	if (!connect(parseAdvLinksl, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_adv_url_parser(int))))
		QErrorMessage::qtHandler();	//Переключатель активации парсера (url)[www.url.com]
	if (!connect(parseHeaderLvl, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_header_lvl_parser(int))))
		QErrorMessage::qtHandler();	//Переключатель активации парсера заголовко
	if (!connect(depFunc, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_deprecated(int))))
		QErrorMessage::qtHandler();	//Переключатель устаревших функций
	if (!connect(devFunc, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_features(int))))
		QErrorMessage::qtHandler();	//Переключатель функций в разработке
	if (!connect(settingsLister, SIGNAL(currentChanged(int)), this, SLOT(slot_tab_changed(int))))
		QErrorMessage::qtHandler();	//Сигнал о выборе определенной вкладки
	if (!connect(clearLog, SIGNAL(clicked()), this, SLOT(slot_clear_logs())))
		QErrorMessage::qtHandler();	//Кнопка очистки логов
	if (!connect(saveLog, SIGNAL(clicked()), this, SLOT(slot_save_logs())))
		QErrorMessage::qtHandler();	//Кнопка сохранения логов
	

	//Ставим заглушку
	workprogress = new QLabel("<H1>"+tr("Work in progress, come later))")+"</H1>");
	QPixmap workprg_cap("ress\\work_progress_cap.png");
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
	xmlw->writeConfig();	//Сохраняем в XML
}

void appSettings::slot_switch_simple_url_parser(int bit)
{
	parswitch.en_simple_url = static_cast<bool>(bit);
	parswitch.en_simple_url == 0 ? push_log("[НАСТРОЙКИ]Парсер простых ссылок отключён") : push_log("[НАСТРОЙКИ]Парсер простых ссылок включён");
}

void appSettings::slot_switch_adv_url_parser(int bit)
{
	parswitch.en_adv_url = static_cast<bool>(bit);
	parswitch.en_adv_url == 0 ? push_log("[НАСТРОЙКИ]Парсер форматированных ссылок отключён") : push_log("[НАСТРОЙКИ]Парсер форматированных ссылок включён");
}

void appSettings::slot_switch_header_lvl_parser(int bit)
{
	parswitch.en_header_lvl = static_cast<bool>(bit);
	parswitch.en_header_lvl == 0 ? push_log("[НАСТРОЙКИ]Парсер заголовков отключён") : push_log("[НАСТРОЙКИ]Парсер заголовков включён");
}

void appSettings::slot_switch_warn_allow(int bit)
{
	allowHttpWarn = static_cast<bool>(bit);
	allowHttpWarn  == 0 ? push_log("[НАСТРОЙКИ]Парсер заголовков отключён") : push_log("[НАСТРОЙКИ]Парсер заголовков включён");
}

void appSettings::slot_switch_deprecated(int bit)
{
	enableDeprFeatures = static_cast<bool>(bit);
	if (::enableDeprFeatures)
	{
		push_log("[НАСТРОЙКИ]Переключение на устаревший функционал");
	}
}

void appSettings::slot_switch_features(int bit)
{
	enableIndevFeatures = static_cast<bool>(bit);
	if (enableIndevFeatures)
	{
		//Посылаем событие в LiteMD.cpp
		push_log("[НАСТРОЙКИ]Активирован функционал находящийся в разработке, возможна нестабильная работа");
		if (!QCoreApplication::sendEvent(qApp, new event_id_constructor(APP_EVENT_enable_dev_func_EVENT)))
			QErrorMessage::qtHandler();
		themeHint->setEnabled(1);
		saveSettingsHint->setEnabled(1);
		autoSaveHint->setEnabled(1);
		saveFreqHint->setEnabled(1);
		themeList->setEnabled(1);
		saveSettings->setEnabled(1);
		autoSave->setEnabled(1);
		saveFreq->setEnabled(1);
		colorThemeHint->setEnabled(1);
		colorTheme->setEnabled(1);
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
		themeHint->setDisabled(1);
		saveSettingsHint->setDisabled(1);
		autoSaveHint->setDisabled(1);
		saveFreqHint->setDisabled(1);
		themeList->setDisabled(1);
		saveSettings->setDisabled(1);
		autoSave->setDisabled(1);
		saveFreq->setDisabled(1);
		colorThemeHint->setDisabled(1);
		colorTheme->setDisabled(1);
	}
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
