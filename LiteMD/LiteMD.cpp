#include "LiteMD.h"
#include "OrientalPushButton.h"
#include "GuiDownloader.h"
#include "dialogBoxes.h"
#include <QtWidgets>

//Номер билда, пока задаётся вручную
#define buildNumber 775

LiteMD::LiteMD(QWidget *parent) : QMainWindow(parent)
{
	//ui.setupUi(this); //Всегда должно быть наверху! UPD - пока отключено

	//Инициализация окон редактора и рендера текста
	mde = new mdEditor;
	mds = new mdScreen;
	//---------------------------------------------

	//Блок элементов интерфейса
	QScrollArea* mdsArea = new QScrollArea;
	OrientablePushButton* btnDown = new OrientablePushButton("--->",this);
	OrientablePushButton* btnUp = new OrientablePushButton("--->", this);
	QGroupBox* editorWindow = new QGroupBox(tr("Editor"));
	QGroupBox* viewerWindow = new QGroupBox(tr("Viewer"));
	QWidget* scrollDock = new QWidget;
	QWidget* mainWgt = new QWidget;
	QAction* actAbout = new QAction(tr("&About"),0);
	QAction* actOpen = new QAction(tr("&Open..."));
	QAction* actSave = new QAction(tr("&Save"));
	QAction* actSaveAs = new QAction(tr("S&ave As..."));
	QAction* actQuit = new QAction(tr("&Quit"));
	QAction* actDownloader = new QAction(tr("HTTP &Downloader module"));
	QAction* actSet = new QAction(tr("&Settings"));
	QAction* actNew = new QAction(tr("&New"));
	quick_access_dock = new QDockWidget(this);
	dwModule = new DownloaderGui;
	mFile = new QMenu(tr("&File"));
	mSettings = new QMenu(tr("&Service"));
	mHelp = new QMenu(tr("&Help"));
	workProgressCap = new QLabel("work in progress");
	mdlSet = new appSettings;
	//-------------------------
	
	//Блок менеджеров размещения кнопок
	QVBoxLayout* editorLay = new QVBoxLayout;
	QVBoxLayout* viewerLay = new QVBoxLayout;
	QVBoxLayout* scrollDockLay = new QVBoxLayout;
	QHBoxLayout* mainWindowHorizontalSetup = new QHBoxLayout;
	//---------------------------------

	//Блок конфигурации элементов интерфейса
	dwModule->setAttribute(Qt::WA_DeleteOnClose);
	quick_access_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	quick_access_dock->setTitleBarWidget(new QWidget());
	quick_access_dock->setFixedHeight(32);
	quick_access_dock->setWidget(workProgressCap);
	this->addDockWidget(Qt::TopDockWidgetArea, quick_access_dock);
	actAbout->setShortcut(Qt::CTRL | Qt::Key_A);
	actOpen->setShortcut(Qt::CTRL | Qt::Key_O);
	actSave->setShortcut(Qt::CTRL | Qt::Key_S);
	actSaveAs->setShortcut(Qt::ALT | Qt::Key_S);
	actQuit->setShortcut(Qt::CTRL | Qt::Key_Q);
	actSet->setShortcut(Qt::CTRL | Qt::Key_S);
	actNew->setShortcut(Qt::CTRL | Qt::Key_N);
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
	mSettings->addAction(actDownloader);
	mSettings->addSeparator();
	mSettings->addAction(actSet);
	mHelp->addAction(actAbout);
	menuBar()->addMenu(mFile);
	menuBar()->addMenu(mSettings);
	menuBar()->addMenu(mHelp);
	//------------------

	//Блок сигнально-слотовых связей
	if (!connect(mde, SIGNAL(textEdited(const QString&)), mds, SLOT(slotSetText(const QString&))))
		QErrorMessage::qtHandler(); //Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(mde, SIGNAL(titleChanged(const QString&)), this, SLOT(slotTitleChanged(const QString&))))
		QErrorMessage::qtHandler();	//Соединяем сигнал открытия файла со слотом изменения заголовка под файл
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
	if (!connect(actDownloader, SIGNAL(triggered()), dwModule, SLOT(slotShow())))
		QErrorMessage::qtHandler(); //Соединяем сигнал срабатывания кнопки на метод отображения
	if (!connect(actNew, SIGNAL(triggered()), mde, SLOT(slotNew())))
		QErrorMessage::qtHandler();	//Команда создания нового документа
	if (!connect(mde, SIGNAL(changeTitle()), this, SLOT(slotFileEdited())))
		QErrorMessage::qtHandler();	//Вешаем звездочку в начале заголовка если документ изменялс
	if (!connect(mde, SIGNAL(resetTitle()), this, SLOT(slotTitleReset())))
		QErrorMessage::qtHandler();	//Сбрасываем заголовок при создании нового файла
	if (!connect(this, SIGNAL(saveFile()), mde, SLOT(slotSaveAs())))
		QErrorMessage::qtHandler();	//Спрашиваем сохранить ли перед закрытием
	if (!connect(actSet, SIGNAL(triggered()), mdlSet, SLOT(show())))
		QErrorMessage::qtHandler();	//Открытие окна настроек
	//------------------------------

	//Рабочий долгосрочный костыль. Создаем пустой виджет и помещаем все в него
	mainWgt->setLayout(mainWindowHorizontalSetup);
	setCentralWidget(mainWgt);

	//Устанавливаем заголовок окна
	setWindowTitle(tr("LiteMD alpha 0.0.0 build ") + QString::number(buildNumber) + tr("[MAX FILE SIZE 65K]"));

	//Кешируем имя окна для возможности восстановления исходного заголовка
	defTitle = windowTitle();
	
	//Устанавливаем иконку приложения
	setWindowIcon(QIcon("icon.ico"));

	//Показываем сообщение готовности к работе
	statusBar()->showMessage(tr("Ready"), 3000);
}
void LiteMD::slotAbout()
{
	QMessageBox::about(this, "LiteMD", tr("Ver. alpha 0.0.0 build ") + QString::number(buildNumber) 
		+ tr("<BR>By anrej0705<BR>See me at Github:") + "<BR><A HREF=\"github.com/anrej0705\">github.com/anrej0705</A><BR><BR>" 
		+ tr("This app is free for use,modify and reupload<BR>") 
		+ "<BR>" + "<BR>Qt 5.14.2<BR>" + "<BR>" 
		+ tr("Repo on Github: " ) + "<A HREF=\"https://github.com/anrej0705/LiteMD\">https://github.com/anrej0705/LiteMD</A><BR>" 
		+ tr("Releases: ") + "<A HREF=\"https://github.com/anrej0705/LiteMD/releases\">https://github.com/anrej0705/LiteMD/releases</A>");
}
void LiteMD::slotTitleChanged(const QString& title)
{
	std::string newTitle = defTitle.toStdString();
	std::string fileFullPath = title.toStdString();
	newTitle.append(" [" + fileFullPath.substr(fileFullPath.rfind('/') + 1, fileFullPath.size() - fileFullPath.rfind('/')) + "]");
	setWindowTitle(QString::fromStdString(newTitle));
}
void LiteMD::slotTitleReset()
{
	setWindowTitle(defTitle);
}
void LiteMD::slot_mbar_send_string(const QString& str)
{
	statusBar()->showMessage(str, 4000);
}
void LiteMD::slotFileEdited()
{
	QString title = windowTitle();
	title.insert(0, '*');
	setWindowTitle(title);
}
void LiteMD::closeEvent(QCloseEvent* ce)
{
	bool save_accept = confirmSave();
	if (!save_accept)
	{
		dwModule->close();
		ce->accept();
	}
	else if (save_accept)
	{
		emit saveFile();
		dwModule->~DownloaderGui();
		ce->accept();
	}
}
LiteMD::~LiteMD()
{}
