#include "frameChangelog.h"
extern "C"
{
	#include "globalFlags.h"
}
currentChangelog::currentChangelog(QWidget* qwgt) : QDialog(qwgt)
{
	compositionManager = new QVBoxLayout;
	renderComposeManager = new QVBoxLayout;
	btnLayout = new QHBoxLayout;
	renderFrame = new QGroupBox;
	render = new mdScreen(renderFrame);
	dismissButton = new QPushButton("OK");
	xmlW = new xmlWriter;
	this->setModal(1);
	this->setWindowTitle(tr("New version changelog"));
	this->setWindowIcon(QIcon("icon.ico"));

	QFile clogFile("docs/Current_ver.md");
	if (clogFile.open(QIODevice::ReadOnly))
	{
		QTextStream cLogStream(&clogFile);
		cLogStream.setCodec("UTF-8");
		render->setWordWrap(1);
		render->slotSetText(cLogStream.readAll());
	}
	else
	{
		render->slotSetText(tr("Current_ver.md отсутствует"));
	}

	//Настройка кнопки
	dismissButton->setFixedWidth(120);
	btnLayout->setAlignment(Qt::AlignCenter);

	//Настраиваем визуал
	compositionManager->addWidget(renderFrame);
	compositionManager->addLayout(btnLayout);
	renderFrame->setAutoFillBackground(1);
	renderFrame->setLayout(renderComposeManager);
	renderFrame->setFixedHeight(300);
	renderComposeManager->addWidget(render);
	btnLayout->addWidget(dismissButton);

	//Соединяем кнопку
	if (!connect(dismissButton, SIGNAL(clicked()), this, SLOT(slotHideWindow())))
		QErrorMessage::qtHandler();	//Соединяем кнопку со слотом, прячет окно

	this->setFixedSize(480, 360);
	this->setLayout(compositionManager);
}

void currentChangelog::slotShowWindow()
{
	this->show();
}

void currentChangelog::slotHideWindow()
{
	logReadState = 1;
	//xmlW->writeConfig();
	this->hide();
}