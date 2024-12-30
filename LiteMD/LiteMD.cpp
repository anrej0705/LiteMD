#include "LiteMD.h"
#include "ui_update_event.h"
#include "GuiDownloader.h"
#include "dialogBoxes.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
#include "LastFileManager.h"
#include <filesystem>
#include <QtWidgets>
#include <boost/container/string.hpp>
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}
QString appPath;
extern struct parser_switchers parswitch;
extern struct depr_paerser_switchers dparswitch;
inline QIcon setAppIcon(); //601:607
LiteMD::LiteMD(int argc, char** argv, QWidget* parent) : QMainWindow(parent)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	push_log("[QT]Настройка интерфейса QMainWindow");
	//Настройка флагов по умолчанию
	parswitch.en_simple_url = 1;
	parswitch.en_adv_url = 1;
	parswitch.en_header_lvl = 1;
	parswitch.en_ex_strkthg = 1;
	parswitch.en_underlined = 1;
	parswitch.en_compat_undr = 1;
	parswitch.en_italic = 1;
	parswitch.en_bold = 1;

	dparswitch.en_t_post = 0;
	dparswitch.en_t_prep = 0;
	dparswitch.en_url_adv = 0;
	dparswitch.en_url_bas = 0;
	dparswitch.en_url_bas_simple = 0;

	appPath = QCoreApplication::applicationDirPath();
	push_log(std::string("[MAIN]Задан каталог приложения " + appPath.toStdString()));

	//Инициализация окон редактора и рендера текста
	mde = new mdEditor;
	mds = new mdScreen;
	//---------------------------------------------

	QWidget* scrollDock = new QWidget;
	QWidget* mainWgt = new QWidget;

	//Блок элементов интерфейса
	mdlSet = new appSettings;
	xmlR = new xmlReader;
	xmlW = new xmlWriter;
	dwModule = new DownloaderGui;
	cLog = new currentChangelog;
	showTim = new QTimer;
	logWindow = new logger;
	actPlaceHeader = new CustomToolButton;
	actSetTextFormat = new CustomToolButton;
	mdsArea = new QScrollArea;
	quick_tb = new QToolBar;
	serv_tb = new QToolBar;
	help = new helpCenter;
	btnDown = new OrientablePushButton("--->", this);
	btnUp = new OrientablePushButton("--->", this);
	editorWindow = new QGroupBox(tr("Editor"));
	viewerWindow = new QGroupBox(tr("Viewer"));
	headersMenu = new QMenu(tr("Set headers"));
	formatStyle = new QMenu(tr("Set format style"));
	mFile = new QMenu(tr("&File"));
	mEdit = new QMenu(tr("&Edit"));
	mSettings = new QMenu(tr("&Service"));
	mHelp = new QMenu(tr("&Help"));
	recentFiles = new QMenu(tr("recentFiles"));
	workProgressCap = new QLabel(tr("work in progress"));
	dirSwitch1 = new QPushButton("<>");
	dirSwitch2 = new QPushButton("<>");
	manageDir = new QPushButton("manageDir");
	syncCtlBtn = new QPushButton("==");

	//Визуал ASCII-стрелочки
	for (uint8_t it = 0; it < 12; ++it)
		hintsList[it] = new QLabel("<>");
	//-------------------------

	//Блок конфигурации элементов интерфейса
	quick_tb->setMovable(0);	//В будущем будет переведено в настройки
	if (!xmlR->checkFileExisting())	//Проверяем существование файлов настроек
	{
		localeDetector();		//Запускаем обнаружение установленного в системе языка
		xmlW->writeConfig();	//Если нет - создаем по умолчанию
	}
	else						//Если есть - читаем
	{
		//Пытаемся читать, если не получается - пытаемся записать и снова прочитать
		//Если снова не получается - выходим из проги
		if (!xmlR->readConfig())
		{
			xmlW->writeConfig();
			if (!xmlR->readConfig())
				throw(exceptionHandler(exceptionHandler::FATAL), "Ошибка записи конфига");
		}
	}

	//Создаем обработчика событий
	qApp->installEventFilter(new ui_event_filter(qApp));

	//Инициализируем контейнер настроек
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
	}*/
	
	//Блок менеджеров размещения кнопок
	QVBoxLayout* editorLay = new QVBoxLayout;
	QVBoxLayout* viewerLay = new QVBoxLayout;
	QVBoxLayout* scrollDockLay = new QVBoxLayout;
	QHBoxLayout* mainWindowHorizontalSetup = new QHBoxLayout;
	//---------------------------------

	//Настройка кнопок
	actAbout = new QAction(QPixmap(appPath + "/ress/icon_about.png"), tr("&About"), 0);
	actOpen = new QAction(QPixmap(appPath + "/ress/icon_open_document.png"), tr("&Open..."));
	actSave = new QAction(QPixmap(appPath + "/ress/icon_save.png"), tr("&Save"));
	actSaveAs = new QAction(QPixmap(appPath + "/ress/icon_save_as.png"), tr("S&ave As..."));
	actClose = new QAction(QPixmap(appPath + "/ress/icon_file_close.png"), tr("Close"));
	actQuit = new QAction(QPixmap(appPath + "/ress/icon_quit.png"), tr("&Quit"));
	actDownloader = new QAction(QPixmap(appPath + "/ress/icon_http_downloader.png"), tr("HTTP &Downloader module"));
	actSet = new QAction(QPixmap(appPath + "/ress/icon_settings.png"), tr("&Settings"));
	actNew = new QAction(QPixmap(appPath + "/ress/icon_new_document.png"), tr("&New"));
	actPlaceUrl = new QAction(QPixmap(appPath + "/ress/icon_place_url.png"), tr("Make &URL"));
	actPlaceAltUrl = new QAction(QPixmap(appPath + "/ress/icon_place_url_alternate.png"), tr("Make alt&enate URL"));
	actHelp = new QAction(QPixmap(appPath + "/ress/icon_help.png"), tr("&Help"));
	actOpenChangelog = new QAction(QPixmap(appPath + "/ress/icon_show_changelog.png"), tr("Sh&ow changelog"));
	actBugReport = new QAction(QPixmap(appPath + "/ress/icon_bug.png"), tr("&Bug!"));
	actSetH1 = new QAction(QPixmap(appPath + "/ress/icon_set_h1.png"), tr("Set H1"));
	actSetH2 = new QAction(QPixmap(appPath + "/ress/icon_set_h2.png"), tr("Set H2"));
	actSetH3 = new QAction(QPixmap(appPath + "/ress/icon_set_h3.png"), tr("Set H3"));
	actSetH4 = new QAction(QPixmap(appPath + "/ress/icon_set_h4.png"), tr("Set H4"));
	actSetH5 = new QAction(QPixmap(appPath + "/ress/icon_set_h5.png"), tr("Set H5"));
	actShieldSymbol = new QAction(QPixmap(appPath + "/ress/icon_set_shielding.png"), tr("Es&cape character"));
	setBold = new QAction(QPixmap(appPath + "/ress/icon_set_bold.png"), tr("Set bold"));
	setItalic = new QAction(QPixmap(appPath + "/ress/icon_set_italic.png"), tr("Set italic"));
	setUnderlined = new QAction(QPixmap(appPath + "/ress/icon_set_under.png"), tr("Set underlined"));
	setStrikethrough = new QAction(QPixmap(appPath + "/ress/icon_set_strike.png"), tr("Set strikethrough"));
	checkUpdates = new QAction(QPixmap(appPath + "/ress/icon_check_updates.png"), tr("checkUpdates"));
	actclearRecent = new QAction(tr("actclearRecent"));
	//----------------
	
	//Настройка выпадающих менюшек
	actPlaceHeader->setIcon(QPixmap(appPath + "/ress/icon_set_header.png"));
	actPlaceHeader->setMenu(headersMenu);
	actPlaceHeader->setPopupMode(QToolButton::InstantPopup);
	actPlaceHeader->setArrowType(Qt::NoArrow);
	headersMenu->addAction(actSetH1);
	headersMenu->addAction(actSetH2);
	headersMenu->addAction(actSetH3);
	headersMenu->addAction(actSetH4);
	headersMenu->addAction(actSetH5);
	actSetTextFormat->setIcon(QPixmap(appPath + "/ress/icon_set_text_format.png"));
	actSetTextFormat->setMenu(formatStyle);
	actSetTextFormat->setPopupMode(QToolButton::InstantPopup);
	actSetTextFormat->setArrowType(Qt::NoArrow);
	formatStyle->addAction(setBold);
	formatStyle->addAction(setItalic);
	formatStyle->addAction(setUnderlined);
	formatStyle->addAction(setStrikethrough);
	//----------------------------

	//Установка обработчика события смены языка
	qApp->installEventFilter(this);

	//Отключенные механики
	actHelp->setDisabled(1);
	checkUpdates->setDisabled(1);
	//--------------------

	//Настройка тулбаров
	quick_tb->setMovable(0);
	serv_tb->setMovable(0);
	serv_tb->setLayoutDirection(Qt::RightToLeft);
	//--------------------

	//Добавляем кнопки в доки
	quick_tb->addAction(actNew);
	quick_tb->addAction(actOpen);
	quick_tb->addAction(actSave);
	quick_tb->addAction(actSaveAs);
	quick_tb->addAction(actClose);
	quick_tb->addSeparator();
	quick_tb->addAction(actPlaceUrl);
	quick_tb->addAction(actPlaceAltUrl);
	quick_tb->addWidget(actSetTextFormat);
	quick_tb->addWidget(actPlaceHeader);
	quick_tb->addAction(actShieldSymbol);
	quick_tb->addSeparator();
	serv_tb->addAction(actBugReport);
	serv_tb->addSeparator();
	serv_tb->addAction(actHelp);
	serv_tb->addSeparator();
	serv_tb->addAction(checkUpdates);
	serv_tb->addSeparator();
	serv_tb->addAction(actAbout);
	serv_tb->addAction(actOpenChangelog);
	serv_tb->addSeparator();
	serv_tb->addAction(actSet);
	serv_tb->addAction(actDownloader);
	//-----------------------

	this->addToolBar(Qt::TopToolBarArea, quick_tb);
	this->addToolBar(Qt::TopToolBarArea, serv_tb);
	actAbout->setShortcut(Qt::CTRL | Qt::Key_A);
	actOpen->setShortcut(Qt::CTRL | Qt::Key_O);
	actSave->setShortcut(Qt::CTRL | Qt::Key_S);
	actSaveAs->setShortcut(Qt::ALT | Qt::Key_S);
	actClose->setShortcut(Qt::CTRL | Qt::Key_W);
	actQuit->setShortcut(Qt::CTRL | Qt::Key_Q);
	actSet->setShortcut(Qt::CTRL | Qt::Key_H);
	actNew->setShortcut(Qt::CTRL | Qt::Key_N);
	actBugReport->setShortcut(Qt::CTRL | Qt::Key_B);
	mdsArea->setWidgetResizable(1);
	mdsArea->setWidget(mds);
	mds->setWordWrap(1);
	btnUp->setOrientation(OrientablePushButton::VerticalBottomTop);
	btnDown->setOrientation(OrientablePushButton::VerticalTopBottom);
	btnUp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	btnDown->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

	//Визуал ASCII-стрелочки - настройка
	for (uint8_t it = 0; it < 12; ++it)
	{
		hintsList[it]->setFixedWidth(32);
		hintsList[it]->setFixedHeight(10);
		hintsList[it]->setAlignment(Qt::AlignCenter);
	}

	//Настройки центральной панели
	uint8_t qlabelIt = 0;
	btnUp->setFixedWidth(32);
	btnDown->setFixedWidth(32);
	manageDir->setFixedWidth(32);
	manageDir->setFixedHeight(40);
	syncCtlBtn->setFixedWidth(32);
	syncCtlBtn->setFixedHeight(40);
	dirSwitch1->setFixedWidth(32);
	dirSwitch2->setFixedWidth(32);
	dirSwitch1->setFixedHeight(48);
	dirSwitch2->setFixedHeight(48);
	for (; qlabelIt < 4; ++qlabelIt)
		scrollDockLay->addWidget(hintsList[qlabelIt]);
	scrollDockLay->addWidget(manageDir);
	for (; qlabelIt < 6; ++qlabelIt)
		scrollDockLay->addWidget(hintsList[qlabelIt]);
	scrollDockLay->addWidget(dirSwitch1);
	scrollDockLay->addWidget(btnUp);
	scrollDockLay->addWidget(btnDown);
	scrollDockLay->addWidget(dirSwitch2);
	for (; qlabelIt < 8; ++qlabelIt)
		scrollDockLay->addWidget(hintsList[qlabelIt]);
	scrollDockLay->addWidget(syncCtlBtn);
	for (; qlabelIt < 12; ++qlabelIt)
		scrollDockLay->addWidget(hintsList[qlabelIt]);
	scrollDock->setLayout(scrollDockLay);
	//--------------------------------------

	//Настройка отображения и компоновки
	editorLay->addWidget(mde);
	viewerLay->addWidget(mdsArea);
	editorWindow->setLayout(editorLay);
	viewerWindow->setLayout(viewerLay);
	mainWindowHorizontalSetup->addWidget(editorWindow);
	mainWindowHorizontalSetup->addWidget(scrollDock);
	mainWindowHorizontalSetup->addWidget(viewerWindow);
	mde->resize(editorLay->sizeHint());
	mde->setMinimumWidth(320);
	mdsArea->setMinimumWidth(320);
	mdsArea->resize(viewerWindow->sizeHint());
	//--------------------------------------

	//Блок настроек меню
	mFile->addAction(actNew);
	mFile->addSeparator();
	mFile->addAction(actOpen);
	mFile->addAction(actSave);
	mFile->addAction(actSaveAs);
	recentFiles->addAction(actclearRecent);
	recentFiles->addSeparator();

	initLastFileMenu();				//Инициализация меню последних файлов(прим. anrej0705)

	mFile->addSeparator();
	mFile->addAction(actClose);
	mFile->addSeparator();
	mFile->addMenu(recentFiles);	//Меню недавних файлов от SilverWolf2K20
	mFile->addSeparator();
	mFile->addAction(actQuit);
	mEdit->addAction(actPlaceUrl);
	mEdit->addAction(actPlaceAltUrl);
	mEdit->addMenu(formatStyle);
	mEdit->addMenu(headersMenu);
	mEdit->addAction(actShieldSymbol);
	mSettings->addAction(actDownloader);
	mSettings->addSeparator();
	mSettings->addAction(actSet);
	mSettings->addAction(actBugReport);
	mHelp->addAction(actHelp);
	mHelp->addAction(actAbout);
	mHelp->addSeparator();
	mHelp->addAction(checkUpdates);
	mHelp->addSeparator();
	mHelp->addAction(actOpenChangelog);
	menuBar()->addMenu(mFile);
	menuBar()->addMenu(mEdit);
	menuBar()->addMenu(mSettings);
	menuBar()->addMenu(mHelp);
	//------------------

	uint8_t connected_signals = 0;

	push_log("[QT->LiteMD]Установка связей сигнал-слот");
	//Блок сигнально-слотовых связей
	if (!connect(mde, SIGNAL(textEdited(const QString&)), mds, SLOT(slotSetText(const QString&))))
		QErrorMessage::qtHandler(); ++connected_signals;//Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(mde, SIGNAL(titleChanged(const QString&)), this, SLOT(slotTitleChanged(const QString&))))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал открытия файла со слотом изменения заголовка под файл
	if (!connect(mdlSet, SIGNAL(signalTitleChanged(const QString&)), this, SLOT(slotTitleChanged(const QString&))))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал из окна настроек со слотом изменения заголовка под файл
	if (!connect(actAbout, SIGNAL(triggered()), this, SLOT(slotAbout())))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал со слотом вызова окна о программе
	if (!connect(actOpen, SIGNAL(triggered()), mde, SLOT(slotOpen())))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал со слотом открытия нового файла
	if (!connect(actSave, SIGNAL(triggered()), mde, SLOT(slotSave())))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал со слотом сохранения файла
	if (!connect(actSaveAs, SIGNAL(triggered()), mde, SLOT(slotSaveAs())))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал со слотом набора имени для сохранения
	if (!connect(actQuit, SIGNAL(triggered()), qApp, SLOT(quit())))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал выхода из приложения
	if (!connect(mde, SIGNAL(statusString(QString)), this, SLOT(slot_mbar_send_string(QString))))
		QErrorMessage::qtHandler();	++connected_signals;//Соединяем сигнал изменения строки состояния
	if (!connect(actDownloader, SIGNAL(triggered()), this, SLOT(httpModuleShow())))
		QErrorMessage::qtHandler(); ++connected_signals;//Соединяем сигнал срабатывания кнопки на метод отображения
	if (!connect(actNew, SIGNAL(triggered()), mde, SLOT(slotNew())))
		QErrorMessage::qtHandler();	++connected_signals;//Команда создания нового документа
	if (!connect(mde, SIGNAL(changeTitle()), this, SLOT(slotFileEdited())))
		QErrorMessage::qtHandler();	++connected_signals;//Вешаем звездочку в начале заголовка если документ изменялс
	if (!connect(mde, SIGNAL(resetTitle()), this, SLOT(slotTitleReset())))
		QErrorMessage::qtHandler();	++connected_signals;//Сбрасываем заголовок при создании нового файла
	if (!connect(this, SIGNAL(saveFile()), mde, SLOT(slotSave())))
		QErrorMessage::qtHandler();	++connected_signals;//Спрашиваем сохранить ли перед закрытием
	if (!connect(actSet, SIGNAL(triggered()), mdlSet, SLOT(show())))
		QErrorMessage::qtHandler();	++connected_signals;//Открытие окна настроек
	if (!connect(actPlaceUrl, SIGNAL(triggered()), mde, SLOT(convertToUrl())))
		QErrorMessage::qtHandler();	++connected_signals;//Конвертация в (ссылку)
	if (!connect(actPlaceAltUrl, SIGNAL(triggered()), mde, SLOT(convToAltUrl())))
		QErrorMessage::qtHandler();	++connected_signals;//Конвертация в [альтернативную](ссылку)
	if (!connect(showTim, SIGNAL(timeout()), cLog, SLOT(slotShowWindow())))
		QErrorMessage::qtHandler();	++connected_signals;//Таймер на вызов окна
	if (!connect(actOpenChangelog, SIGNAL(triggered()), cLog, SLOT(slotShowWindow())))
		QErrorMessage::qtHandler();	++connected_signals;//Вызов окна ченжлога
	if (!connect(actBugReport, SIGNAL(triggered()), logWindow, SLOT(slot_read_n_show())))
		QErrorMessage::qtHandler();	++connected_signals;//Вызов окна логов
	if (!connect(actSetH1, SIGNAL(triggered()), mde, SLOT(slotSetH1())))
		QErrorMessage::qtHandler();	++connected_signals;//Установка заголовка H1
	if (!connect(actSetH2, SIGNAL(triggered()), mde, SLOT(slotSetH2())))
		QErrorMessage::qtHandler();	++connected_signals;//Установка заголовка H2
	if (!connect(actSetH3, SIGNAL(triggered()), mde, SLOT(slotSetH3())))
		QErrorMessage::qtHandler();	++connected_signals;//Установка заголовка H3
	if (!connect(actSetH4, SIGNAL(triggered()), mde, SLOT(slotSetH4())))
		QErrorMessage::qtHandler();	++connected_signals;//Установка заголовка H4
	if (!connect(actSetH5, SIGNAL(triggered()), mde, SLOT(slotSetH5())))
		QErrorMessage::qtHandler();	++connected_signals;//Установка заголовка H5
	if (!connect(setBold, SIGNAL(triggered()), mde, SLOT(slotSetBold())))
		QErrorMessage::qtHandler();	++connected_signals;//Жирный текст
	if (!connect(setItalic, SIGNAL(triggered()), mde, SLOT(slotSetItalic())))
		QErrorMessage::qtHandler();	++connected_signals;//Курсив
	if (!connect(setUnderlined, SIGNAL(triggered()), mde, SLOT(slotSetUnrderline())))
		QErrorMessage::qtHandler();	++connected_signals;//Подчёркнутый
	if (!connect(setStrikethrough, SIGNAL(triggered()), mde, SLOT(slotSetStrikethrough())))
		QErrorMessage::qtHandler();	++connected_signals;//Зачёркнутый
	if (!connect(actShieldSymbol, SIGNAL(triggered()), mde, SLOT(slotSetEscape())))
		QErrorMessage::qtHandler();	++connected_signals;//Экранировать
	if (!connect(checkUpdates, SIGNAL(triggered()), this, SLOT(slotCheckUpdates())))
		QErrorMessage::qtHandler();	++connected_signals;//Проверка обновлений //0.3.7
	if (!connect(actClose, SIGNAL(triggered()), this, SLOT(slotFileClose())))
		QErrorMessage::qtHandler();	++connected_signals;//Закрыть документ
	if (!connect(btnDown, SIGNAL(clicked()), this, SLOT(slotMdsDown())))
		QErrorMessage::qtHandler();	++connected_signals;//Листание вниз
	if (!connect(btnUp, SIGNAL(clicked()), this, SLOT(slotMdsUp())))
		QErrorMessage::qtHandler();	++connected_signals;//Листание вверх
	if (!connect(dirSwitch1, SIGNAL(clicked()), this, SLOT(slotSwitchDir())))
		QErrorMessage::qtHandler();	++connected_signals;//Нижняя кнопка переключения
	if (!connect(dirSwitch2, SIGNAL(clicked()), this, SLOT(slotSwitchDir())))
		QErrorMessage::qtHandler();	++connected_signals;//Верхняя кнопка переключения
	if (!connect(manageDir, SIGNAL(clicked()), this, SLOT(slotManageDir())))
		QErrorMessage::qtHandler();	++connected_signals;//Управление руч/авт
	if (!connect(syncCtlBtn, SIGNAL(clicked()), this, SLOT(slotSwitchSync())))
		QErrorMessage::qtHandler();	++connected_signals;//Синхронизация окон руч/авт
	if (!connect(mdsArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollEvent(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Синхронизация от рендера
	if (!connect(mde->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollEvent(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Синхронизация от рендера
	if (!connect(actclearRecent, SIGNAL(triggered()), this, SLOT(slotRemoveRf())))
		QErrorMessage::qtHandler();	++connected_signals;//Синхронизация от рендера
	if (!connect(actHelp, SIGNAL(triggered()), help, SLOT(show())))
		QErrorMessage::qtHandler();	++connected_signals;//Синхронизация от рендера
	push_log(std::string("[QT->LiteMD]Образовано " + std::to_string(connected_signals) + " связей"));
	//------------------------------

	//Рабочий долгосрочный костыль. Создаем пустой виджет и помещаем все в него
	mainWgt->setLayout(mainWindowHorizontalSetup);
	setCentralWidget(mainWgt);

	log_stroke->append("[VESRION]Версия приложения");
	log_stroke->append(APP_STAGE);
	log_stroke->append(APP_VERSION);
	log_stroke->append(" ");
	log_stroke->append(std::to_string(BUILD_NUMBER).c_str());
	push_log(log_stroke->c_str());
	log_stroke->clear();

	//Устанавливаем заголовок окна
	setWindowTitle(tr("LiteMD") + APP_STAGE + APP_VERSION + tr(" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))/* + tr("[MAX FILE SIZE 65K]")*/ + " [" + tr("Untitled") + "]");

	//Кешируем имя окна для возможности восстановления исходного заголовка
	//defTitle = windowTitle();	//patch 0.2.2 исправление версии "0.0.0" при открытии файла
	
	//Устанавливаем иконку приложения
	setWindowIcon(setAppIcon());

	//Если приложение запускается в первый раз или конфиг файл отсутствует то будем считать что это первый запуск
	//И показываем юзеру текущий ченджлог используя наш рендер, перед показом даём задержку 1 сек чтобы окно успело
	//Отрисоваться
	if (!logReadState)
	{
		showTim->setInterval(203);
		showTim->setSingleShot(1);
		showTim->start();
	}

	//Устанавливаем язык
	mdlSet->slot_lang_selected(langCode);
	mdlSet->slot_apply_settings(); 

	if (argc == 2)	//Если вторым аргументом файл то сразу открываем его
	{
		mde->openFileArg(argv[1]);
	}

	//Предустановка текста на кнопке
	managePrior == 0 ? manageDir->setText(tr("manageDirAuto")) : manageDir->setText(tr("manageDirManual"));
	if (!managePrior)
	{
		dirSwitch1->setText("<>");
		dirSwitch2->setText("<>");
		dirSwitch1->setEnabled(0);
		dirSwitch2->setEnabled(0);
	}
	else
	{
		if (scrollPrior) //Предустанки кнопки направления
		{
			dirSwitch1->setText("<<");
			dirSwitch2->setText("<<");
			for (uint8_t it = 0; it < 12; ++it)
				hintsList[it]->setText("<<");
		}
		else if (!scrollPrior)
		{
			dirSwitch1->setText(">>");
			dirSwitch2->setText(">>");
			for (uint8_t it = 0; it < 12; ++it)
				hintsList[it]->setText(">>");
		}
	}

	//Показываем сообщение готовности к работе
	statusBar()->showMessage(tr("Ready"), 3000);

	defTitle.append(tr("LiteMD") + QString(APP_STAGE) + QString(APP_VERSION) + (" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER)) + " [" + tr("Untitled") + "]");

	delete(log_stroke);
}

LiteMD::~LiteMD()
{
	//0.2.7 Позже поработаю здесь
	//free(chosenTheme);
	//deleteOnExit();
	//0.3.1 Проверяем наличие поднятого флага разрешений обновлений
	if (enableUpdate)
	{
		//Создаём названия исходного файла и файла который надо скопировать
		std::string source = getAppPath().toStdString() + "/LiteMD.exe";
		std::string old_src = getAppPath().toStdString() + "/LiteMD_old.exe";

		try
		{
			//Пробуем создать копию
			if (std::filesystem::copy_file(source, old_src))
			{
				//Если всё успешно то запускаем копию которая уже будет выполнять обновление
				system(getAppPath().toLocal8Bit() + "/LiteMD_old.exe");
			}
		}
		catch (std::filesystem::filesystem_error& e)
		{
			//Файл может существовать, поэтому просто вызовем его
			system(getAppPath().toLocal8Bit() + "/LiteMD_old.exe");
			exit(0);
		}
	}
}

QString getAppPath()
{
	//Возвращаем QString каталог в котором запущена приложуха
	return appPath;
}