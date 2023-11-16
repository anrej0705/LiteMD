#include "LiteMD.h"
#include <QtWidgets>

LiteMD::LiteMD(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this); //Всегда должно быть наверху!
	mde = new mdEditor;
	mds = new mdScreen;
	//Пока костыли, позже сниму
	mde->setFixedSize(240, 360);
	mds->setFixedSize(240, 360);
	//Размещаем виджеты
	QHBoxLayout* mainWindowHorizontalSetup = new QHBoxLayout;
	mainWindowHorizontalSetup->addWidget(mde);
	mainWindowHorizontalSetup->addWidget(mds);
	//Рабочий костыль
	QWidget* mainWgt = new QWidget;
	mainWgt->setLayout(mainWindowHorizontalSetup);
	setCentralWidget(mainWgt);
	//Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(mde, SIGNAL(textEdited(const QString&)), mds, SLOT(slotSetText(const QString&))))
		QErrorMessage::qtHandler();
}

LiteMD::~LiteMD()
{}
