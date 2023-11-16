#include "LiteMD.h"
#include "OrientalPushButton.h"
#include <QtWidgets>

LiteMD::LiteMD(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this); //Всегда должно быть наверху!

	//Инициализация окон редактора и рендера текста
	mde = new mdEditor;
	mds = new mdScreen;
	//Пока костыли, позже сниму
	mde->setFixedSize(320, 480);
	mds->setFixedSize(320, 480);
	//---------------------------------------------

	//Блок элементов интерфейса
	OrientablePushButton* btnDown = new OrientablePushButton("--->",this);
	OrientablePushButton* btnUp = new OrientablePushButton("--->", this);
	QWidget* scrollDock = new QWidget;
	QWidget* mainWgt = new QWidget;
	QAction* actAbout = new QAction("&About",0);
	mHelp = new QMenu("&Help");
	//-------------------------
	
	//Блок менеджеров размещения кнопок
	QVBoxLayout* scrollDockLay = new QVBoxLayout;
	QHBoxLayout* mainWindowHorizontalSetup = new QHBoxLayout;
	//---------------------------------

	//Блок конфигурации элементов интерфейса
	btnUp->setOrientation(OrientablePushButton::VerticalBottomTop);
	btnDown->setOrientation(OrientablePushButton::VerticalTopBottom);
	btnUp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	btnDown->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	scrollDockLay->addSpacing(100);
	scrollDockLay->addWidget(btnUp);
	scrollDockLay->addWidget(btnDown);
	scrollDockLay->addSpacing(100);
	scrollDock->setLayout(scrollDockLay);
	mainWindowHorizontalSetup->addWidget(mde);
	mainWindowHorizontalSetup->addWidget(scrollDock);
	mainWindowHorizontalSetup->addWidget(mds);
	//--------------------------------------

	//Блок настроек меню
	mHelp->addAction(actAbout);
	menuBar()->addMenu(mHelp);
	//------------------

	//Блок сигнально-слотовых связей
	if (!connect(mde, SIGNAL(textEdited(const QString&)), mds, SLOT(slotSetText(const QString&))))
		QErrorMessage::qtHandler(); //Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(actAbout, SIGNAL(triggered()), this, SLOT(slotAbout())))
		QErrorMessage::qtHandler();	//Соединяем сигнал со слотом вызова окна о программе
	//------------------------------

	//Рабочий долгосрочный костыль. Создаем пустой виджет и помещаем все в него
	mainWgt->setLayout(mainWindowHorizontalSetup);
	setCentralWidget(mainWgt);

	//Устанавливаем заголовок окна
	setWindowTitle("LiteMD rc-20231117");
}
void LiteMD::slotAbout()
{
	QMessageBox::about(this, "LiteMD", "Ver. rc-20231117 build 47\nBy Anrej0705\nSee me at Github:\ngithub.com/anrej0705");
}

LiteMD::~LiteMD()
{}
