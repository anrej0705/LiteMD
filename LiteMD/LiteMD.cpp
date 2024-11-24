#include "LiteMD.h"
#include "ui_update_event.h"
#include "OrientalPushButton.h"
#include "GuiDownloader.h"
#include "dialogBoxes.h"
#include "exceptionHandler.h"
#include <QtWidgets>
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}
extern struct parser_switchers parswitch;
LiteMD::LiteMD(QWidget *parent) : QMainWindow(parent)
{
	//Настройка флагов по умолчанию
	parswitch.en_simple_url = 1;
	parswitch.en_adv_url = 1;
	parswitch.en_header_lvl = 1;

	//Инициализация окон редактора и рендера текста
	mde = new mdEditor;
	mds = new mdScreen;
	//---------------------------------------------

	//Блок элементов интерфейса
	QScrollArea* mdsArea = new QScrollArea;
	OrientablePushButton* btnDown = new OrientablePushButton("--->", this);
	OrientablePushButton* btnUp = new OrientablePushButton("--->", this);
	editorWindow = new QGroupBox(tr("Editor"));
	viewerWindow = new QGroupBox(tr("Viewer"));
	QWidget* scrollDock = new QWidget;
	QWidget* mainWgt = new QWidget;
	quick_tb = new QToolBar;
	serv_tb = new QToolBar;
	dwModule = new DownloaderGui;
	mFile = new QMenu(tr("&File"));
	mEdit = new QMenu(tr("&Edit"));
	mSettings = new QMenu(tr("&Service"));
	mHelp = new QMenu(tr("&Help"));
	workProgressCap = new QLabel(tr("work in progress"));
	mdlSet = new appSettings;
	xmlW = new xmlWriter;
	xmlR = new xmlReader;
	cLog = new currentChangelog;
	showTim = new QTimer;
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
				throw(exceptionHandler(exceptionHandler::FATAL));
		}
	}

	//Создаем обработчика событий
	qApp->installEventFilter(new ui_event_filter(qApp));

	//Инициализируем контейнер настроек
	if (enableIndevFeatures)
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
	
	//Блок менеджеров размещения кнопок
	QVBoxLayout* editorLay = new QVBoxLayout;
	QVBoxLayout* viewerLay = new QVBoxLayout;
	QVBoxLayout* scrollDockLay = new QVBoxLayout;
	QHBoxLayout* mainWindowHorizontalSetup = new QHBoxLayout;
	//---------------------------------

	//Настройка кнопок
	actAbout = new QAction(QPixmap("ress/icon_about.png"), tr("&About"), 0);
	actOpen = new QAction(QPixmap("ress/icon_open_document.png"), tr("&Open..."));
	actSave = new QAction(QPixmap("ress/icon_save.png"), tr("&Save"));
	actSaveAs = new QAction(QPixmap("ress/icon_save_as.png"), tr("S&ave As..."));
	actQuit = new QAction(QPixmap("ress/icon_quit.png"), tr("&Quit"));
	actDownloader = new QAction(QPixmap("ress/icon_http_downloader.png"), tr("HTTP &Downloader module"));
	actSet = new QAction(QPixmap("ress/icon_settings.png"), tr("&Settings"));
	actNew = new QAction(QPixmap("ress/icon_new_document.png"), tr("&New"));
	actPlaceUrl = new QAction(QPixmap("ress/icon_place_url.png"), tr("Make &URL"));
	actPlaceAltUrl = new QAction(QPixmap("ress/icon_place_url_alternate.png"), tr("Make alt&enate URL"));
	actSetTextFormat = new QAction(QPixmap("ress/icon_set_text_format.png"), tr("Te&xt Format"));
	actHelp = new QAction(QPixmap("ress/icon_help.png"), tr("&Help"));
	actOpenChangelog = new QAction(QPixmap("ress/icon_show_changelog.png"), tr("Sh&ow changelog"));
	actBugReport = new QAction(QPixmap("ress/icon_bug.png"), tr("&Bug!"));
	//----------------
	
	//Установка обработчика события смены языка
	qApp->installEventFilter(this);

	//Отключенные механики
	actSetTextFormat->setDisabled(1);
	actHelp->setDisabled(1);
	actBugReport->setDisabled(1);	//Отключено до 0.2.2
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
	quick_tb->addSeparator();
	quick_tb->addAction(actPlaceUrl);
	quick_tb->addAction(actPlaceAltUrl);
	quick_tb->addAction(actSetTextFormat);
	quick_tb->addSeparator();
	serv_tb->addAction(actBugReport);
	serv_tb->addSeparator();
	serv_tb->addAction(actHelp);
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
	btnUp->setEnabled(0);	//Пока что отключаем до реализации механики кнопки
	btnDown->setEnabled(0);	//Пока что отключаем до реализации механики кнопки
	scrollDockLay->addSpacing(100);
	scrollDockLay->addWidget(btnUp);
	scrollDockLay->addWidget(btnDown);
	scrollDockLay->addSpacing(100);
	scrollDock->setLayout(scrollDockLay);
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
	mFile->addSeparator();
	mFile->addAction(actQuit);
	mEdit->addAction(actPlaceUrl);
	mEdit->addAction(actPlaceAltUrl);
	mEdit->addAction(actSetTextFormat);
	mSettings->addAction(actDownloader);
	mSettings->addSeparator();
	mSettings->addAction(actSet);
	mSettings->addAction(actBugReport);
	mHelp->addAction(actHelp);
	mHelp->addAction(actAbout);
	mHelp->addAction(actOpenChangelog);
	menuBar()->addMenu(mFile);
	menuBar()->addMenu(mEdit);
	menuBar()->addMenu(mSettings);
	menuBar()->addMenu(mHelp);
	//------------------

	//Блок сигнально-слотовых связей
	if (!connect(mde, SIGNAL(textEdited(const QString&)), mds, SLOT(slotSetText(const QString&))))
		QErrorMessage::qtHandler(); //Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(mde, SIGNAL(titleChanged(const QString&)), this, SLOT(slotTitleChanged(const QString&))))
		QErrorMessage::qtHandler();	//Соединяем сигнал открытия файла со слотом изменения заголовка под файл
	if (!connect(mdlSet, SIGNAL(signalTitleChanged(const QString&)), this, SLOT(slotTitleChanged(const QString&))))
		QErrorMessage::qtHandler();	//Соединяем сигнал из окна настроек со слотом изменения заголовка под файл
	if (!connect(actAbout, SIGNAL(triggered()), this, SLOT(slotAbout())))
		QErrorMessage::qtHandler();	//Соединяем сигнал со слотом вызова окна о программе
	if (!connect(actOpen, SIGNAL(triggered()), mde, SLOT(slotOpen())))
		QErrorMessage::qtHandler();	//Соединяем сигнал со слотом открытия нового файла
	if (!connect(actSave, SIGNAL(triggered()), mde, SLOT(slotSave())))
		QErrorMessage::qtHandler();	//Соединяем сигнал со слотом сохранения файла
	if (!connect(actSaveAs, SIGNAL(triggered()), mde, SLOT(slotSaveAs())))
		QErrorMessage::qtHandler();	//Соединяем сигнал со слотом набора имени для сохранения
	if (!connect(actQuit, SIGNAL(triggered()), qApp, SLOT(quit())))
		QErrorMessage::qtHandler();	//Соединяем сигнал выхода из приложения
	if (!connect(mde, SIGNAL(statusString(QString)), this, SLOT(slot_mbar_send_string(QString))))
		QErrorMessage::qtHandler();	//Соединяем сигнал изменения строки состояния
	if (!connect(actDownloader, SIGNAL(triggered()), this, SLOT(httpModuleShow())))
		QErrorMessage::qtHandler(); //Соединяем сигнал срабатывания кнопки на метод отображения
	if (!connect(actNew, SIGNAL(triggered()), mde, SLOT(slotNew())))
		QErrorMessage::qtHandler();	//Команда создания нового документа
	if (!connect(mde, SIGNAL(changeTitle()), this, SLOT(slotFileEdited())))
		QErrorMessage::qtHandler();	//Вешаем звездочку в начале заголовка если документ изменялс
	if (!connect(mde, SIGNAL(resetTitle()), this, SLOT(slotTitleReset())))
		QErrorMessage::qtHandler();	//Сбрасываем заголовок при создании нового файла
	if (!connect(this, SIGNAL(saveFile()), mde, SLOT(slotSave())))
		QErrorMessage::qtHandler();	//Спрашиваем сохранить ли перед закрытием
	if (!connect(actSet, SIGNAL(triggered()), mdlSet, SLOT(show())))
		QErrorMessage::qtHandler();	//Открытие окна настроек
	if (!connect(actPlaceUrl, SIGNAL(triggered()), mde, SLOT(convertToUrl())))
		QErrorMessage::qtHandler();	//Конвертация в (ссылку)
	if (!connect(actPlaceAltUrl, SIGNAL(triggered()), mde, SLOT(convToAltUrl())))
		QErrorMessage::qtHandler();	//Конвертация в [альтернативную](ссылку)
	if (!connect(showTim, SIGNAL(timeout()), cLog, SLOT(slotShowWindow())))
		QErrorMessage::qtHandler();	//Таймер на вызов окна
	if (!connect(actOpenChangelog, SIGNAL(triggered()), cLog, SLOT(slotShowWindow())))
		QErrorMessage::qtHandler();	//Вызов окна ченжлога
	//------------------------------

	//Рабочий долгосрочный костыль. Создаем пустой виджет и помещаем все в него
	mainWgt->setLayout(mainWindowHorizontalSetup);
	setCentralWidget(mainWgt);

	//Устанавливаем заголовок окна
	setWindowTitle(tr("LiteMD") + APP_STAGE + APP_VERSION + tr(" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))/* + tr("[MAX FILE SIZE 65K]")*/);

	//Кешируем имя окна для возможности восстановления исходного заголовка
	//defTitle = windowTitle();	//patch 0.2.2 исправление версии "0.0.0" при открытии файла
	
	//Устанавливаем иконку приложения
	setWindowIcon(QIcon("icon.ico"));

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

	//Показываем сообщение готовности к работе
	statusBar()->showMessage(tr("Ready"), 3000);
}
//О программе
void LiteMD::slotAbout()
{
	QMessageBox::about(this, "LiteMD", tr("Ver.") + APP_STAGE + APP_VERSION + (" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))
		+ tr("<BR>By anrej0705<BR>See me at Github:") + "<BR><A HREF=\"github.com/anrej0705\">github.com/anrej0705</A><BR><BR>" 
		+ tr("This app is free for use,modify and reupload<BR><BR>LiteMD IS FREE SOFTWARE! IF YOU PAID FOR IT YOU HAVE BEEN SCAMMED!") 
		+ "<BR>" + "<BR>| Qt 5.14.2 | C++17 | C11 | Boost 1.84.00 |<BR>" + "<BR>" 
		+ tr("Github repo: " ) + "<A HREF=\"https://github.com/anrej0705/LiteMD\">https://github.com/anrej0705/LiteMD</A><BR>" 
		+ tr("Releases: ") + "<A HREF=\"https://github.com/anrej0705/LiteMD/releases\">https://github.com/anrej0705/LiteMD/releases</A>");
}
//Слот редактирования заголовка(добавления файла к концу)
void LiteMD::slotTitleChanged(const QString& title)
{
	//Контейнеры для помещения элементов заголовка
	std::string newTitle/* = defTitle.toStdString()*/;	//Патч 0.2.2 исправление заголовка
	std::string fileFullPath = title.toStdString();
	//Формируем заголовок из контейнеров и устанавливаем в приложение
	newTitle.append((tr("LiteMD") + APP_STAGE + APP_VERSION + tr(" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))).toStdString() + " [" + fileFullPath.substr(fileFullPath.rfind('/') + 1, fileFullPath.size() - fileFullPath.rfind('/')) + "]");
	setWindowTitle(QString::fromStdString(newTitle));
}
//Сброс заголовка
void LiteMD::slotTitleReset()
{
	setWindowTitle(defTitle);
}
//Слот показа состояния на панели снизу
void LiteMD::slot_mbar_send_string(const QString& str)
{
	statusBar()->showMessage(str, 4000);
}
//Слот установки флага редактирования флага
void LiteMD::slotFileEdited()
{
	if (!appTitleUpdated)
	{
		QString title = windowTitle();
		title.insert(0, '*');
		setWindowTitle(title);
		appTitleUpdated = 1;
	}
}
//Перехватчик события закрытия
void LiteMD::closeEvent(QCloseEvent* ce)
{
	//1
	//Если файл редактировался то спрашиваем нужно ли сохранить
	if (fileChangedState)
	{
		if (!confirmSave())
		{
			//Если пользователь не захотел сохранять то просто закрываемся
			appClose = 1;
			dwModule->close();
			ce->accept();
		}
		else
		{
			//Если пользователь захотел сохранить то закрывамся после сохранения
			appClose = 1;
			emit saveFile();
			dwModule->close();
			ce->accept();
		}
	}
	//Выставляем флаг закрытия и закрываем модуль если он открыт
	appClose = 1;
	dwModule->close();
}
//Вызов модуля загрузки
void LiteMD::httpModuleShow()
{
	//Если по указателю есть то что нам надо то показываем
	if(dynamic_cast<DownloaderGui*>(dwModule))
	{
		dwModule->slotShow();
		return;
	}
	//Иначе создаем объект и показываем
	dwModule = new DownloaderGui;
	dwModule->slotShow();
}
LiteMD::~LiteMD()
{
	//deleteOnExit();
}
