#include "LiteMD.h"
#include "OrientalPushButton.h"
#include <QtWidgets>

//Номер билда, пока задаётся вручную
#define buildNumber 428

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
	QGroupBox* editorWindow = new QGroupBox("Editor");
	QGroupBox* viewerWindow = new QGroupBox("Viewer");
	QWidget* scrollDock = new QWidget;
	QWidget* mainWgt = new QWidget;
	QAction* actAbout = new QAction("&About",0);
	QAction* actOpen = new QAction("&Open...");
	QAction* actSave = new QAction("&Save");
	QAction* actSaveAs = new QAction("S&ave As...");
	QAction* actQuit = new QAction("&Quit");
	mFile = new QMenu("&File");
	mHelp = new QMenu("&Help");
	//-------------------------
	
	//Блок менеджеров размещения кнопок
	QVBoxLayout* editorLay = new QVBoxLayout;
	QVBoxLayout* viewerLay = new QVBoxLayout;
	QVBoxLayout* scrollDockLay = new QVBoxLayout;
	QHBoxLayout* mainWindowHorizontalSetup = new QHBoxLayout;
	//---------------------------------

	//Блок конфигурации элементов интерфейса
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
	mds->setMinimumWidth(320);
	mdsArea->resize(viewerWindow->sizeHint());
	//--------------------------------------

	//Блок настроек меню
	mFile->addAction(actOpen);
	mFile->addAction(actSave);
	mFile->addAction(actSaveAs);
	mFile->addSeparator();
	mFile->addAction(actQuit);
	mHelp->addAction(actAbout);
	menuBar()->addMenu(mFile);
	menuBar()->addMenu(mHelp);
	//------------------

	//Блок сигнально-слотовых связей
	if (!connect(mde, SIGNAL(textEdited(const QString&)), mds, SLOT(slotSetText(const QString&))))
		QErrorMessage::qtHandler(); //Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(mde, SIGNAL(titleChanged(QString&)), this, SLOT(slotTitleChanged(QString&))))
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
	//------------------------------

	//Рабочий долгосрочный костыль. Создаем пустой виджет и помещаем все в него
	mainWgt->setLayout(mainWindowHorizontalSetup);
	setCentralWidget(mainWgt);

	//Устанавливаем заголовок окна
	setWindowTitle("LiteMD alpha 0.1.3 build " + QString::number(buildNumber));
	
	//Устанавливаем иконку приложения
	setWindowIcon(QIcon("icon.ico"));

	//Показываем сообщение готовности к работе
	statusBar()->showMessage("Ready", 3000);
}
void LiteMD::slotAbout()
{
	QMessageBox::about(this, "LiteMD", "Ver. alpha 0.1.3 build " + QString::number(buildNumber) + "\nBy Anrej0705\nSee me at Github:\ngithub.com/anrej0705\n\nThis app is free for use,modify\nand reupload\n\nI hope u like my app :D");
}
void LiteMD::slotTitleChanged(QString& title)
{
	setWindowTitle(title);
}

LiteMD::~LiteMD()
{}
