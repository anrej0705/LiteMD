#include "appSettings.h"
#include "ui_update_event.h"
#include "event_id_constructor.h"
#include "global_definitions.h"
#include "exceptionHandler.h"
#include <QtWidgets>
extern "C"
{
	#include "globalFlags.h"
}
struct parser_switchers parswitch;
appSettings::appSettings(QWidget* aWgt) : QDialog(aWgt)
{
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
		QErrorMessage::qtHandler();
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();
	if (!connect(btnCancel, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();
	if (!connect(langList, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_lang_selected(int))))
		QErrorMessage::qtHandler();
	if (!connect(btnApply, SIGNAL(clicked()), this, SLOT(slot_apply_settings())))
		QErrorMessage::qtHandler();
	if (!connect(allowWarnings, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_warn_allow(int))))
		QErrorMessage::qtHandler();
	if (!connect(parseSimplLinks, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_simple_url_parser(int))))
		QErrorMessage::qtHandler();
	if (!connect(parseAdvLinksl, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_adv_url_parser(int))))
		QErrorMessage::qtHandler();
	if (!connect(parseHeaderLvl, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_header_lvl_parser(int))))
		QErrorMessage::qtHandler();
	if (!connect(depFunc, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_deprecated(int))))
		QErrorMessage::qtHandler();
	if (!connect(devFunc, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_features(int))))
		QErrorMessage::qtHandler();
	

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
}

void appSettings::slot_switch_adv_url_parser(int bit)
{
	parswitch.en_adv_url = static_cast<bool>(bit);
}

void appSettings::slot_switch_header_lvl_parser(int bit)
{
	parswitch.en_header_lvl = static_cast<bool>(bit);
}

void appSettings::slot_switch_warn_allow(int bit)
{
	allowHttpWarn = static_cast<bool>(bit);
}

void appSettings::slot_switch_deprecated(int bit)
{
	enableDeprFeatures = static_cast<bool>(bit);
}

void appSettings::slot_switch_features(int bit)
{
	if (!enableIndevFeatures)
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
	enableIndevFeatures = static_cast<bool>(bit);
}
