#include "frameChangelog.h"
#include "logger_backend.h"
#include "LiteMD.h"
extern "C"
{
	#include "globalFlags.h"
}
currentChangelog::currentChangelog(QWidget* qwgt) : QDialog(qwgt)
{
	push_log("[CHANGELOG]Загрука сведений о текущей версии");
	QScrollArea* mdsArea = new QScrollArea;
	compositionManager = new QVBoxLayout;
	renderComposeManager = new QVBoxLayout;
	btnLayout = new QHBoxLayout;
	renderFrame = new QGroupBox;
	render = new mdScreen(renderFrame);
	dismissButton = new QPushButton("OK");
	xmlW = new xmlWriter;
	this->setModal(1);
	this->setWindowTitle(tr("New version changelog"));
	QPixmap appIcon(getAppPath() + "/icon.ico");
	appIcon.setMask(appIcon.createMaskFromColor(QColor(0, 0, 0)));
	setWindowIcon(QIcon(appIcon));

	std::wstring temp;
	QString qtemp;

	QFile clogFile(getAppPath() + "/docs/Current_ver.md");
	if (clogFile.open(QIODevice::ReadOnly))
	{
		QTextStream cLogStream(&clogFile);
		cLogStream.setCodec("UTF-8");
		temp = cLogStream.readAll().toStdWString();
		qtemp = QString::fromStdWString(temp);
		render->setWordWrap(1);
		render->slotSetText(qtemp);
	}
	else
	{
		render->slotSetText(tr("Current_ver.md отсутствует"));
	}

	//Настраиваем полосу прокрутки
	mdsArea->setWidgetResizable(1);
	mdsArea->setWidget(render);
	//viewerLay->addWidget(mdsArea);
	mdsArea->setMinimumWidth(320);
	//mdsArea->resize(viewerWindow->sizeHint());

	//Устанавливаем фильтр на отлов события смены языка
	qApp->installEventFilter(this);

	//Настройка кнопки
	dismissButton->setFixedWidth(120);
	btnLayout->setAlignment(Qt::AlignCenter);

	//Настраиваем визуал
	compositionManager->addWidget(renderFrame);
	compositionManager->addLayout(btnLayout);
	renderFrame->setAutoFillBackground(1);
	renderFrame->setLayout(renderComposeManager);
	renderFrame->setFixedHeight(324);
	renderComposeManager->addWidget(mdsArea);
	btnLayout->addWidget(dismissButton);

	//Соединяем кнопку
	if (!connect(dismissButton, SIGNAL(clicked()), this, SLOT(slotHideWindow())))
		QErrorMessage::qtHandler();	//Соединяем кнопку со слотом, прячет окно

	this->setFixedSize(512, 384);
	this->setLayout(compositionManager);
}

void currentChangelog::slotShowWindow()
{
	this->show();
}

void currentChangelog::slotHideWindow()
{
	logReadState = 1;
	xmlW->writeConfig();
	this->hide();
}