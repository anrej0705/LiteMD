#include "helpCenter.h"
#include "logger_backend.h"
#include "LiteMD.h"

helpCenter::helpCenter(QWidget* qwgt) : QDialog(qwgt)
{
	push_log("[ЦЕНТР СПРАВКИ]Инициаилизация виджета");
	QScrollArea* mdsArea = new QScrollArea;
	renderComposeManager = new QVBoxLayout;
	renderFrame = new QGroupBox;
	btnLayout = new QHBoxLayout;
	helpFileName = getAppPath() + "/docs/Manual.md";

	//Инициализация рендера
	render = new mdScreen(renderFrame);
	
	//Настраиваем окно
	this->setModal(1);
	this->setWindowTitle(tr("Program guide"));

	//Задаём иконку
	QPixmap appIcon(getAppPath() + "/icon.ico");
	appIcon.setMask(appIcon.createMaskFromColor(QColor(0, 0, 0)));
	setWindowIcon(QIcon(appIcon));

	std::string temp;
	QString qtemp;

	QFile clogFile(helpFileName);

	push_log("[ЦЕНТР СПРАВКИ]Попытка открыть справочный файл");
	if (clogFile.open(QIODevice::ReadOnly))
	{
		QTextStream cLogStream(&clogFile);
		cLogStream.setCodec("UTF-8");
		temp = cLogStream.readAll().toStdString();
		qtemp = QString::fromStdString(temp);
		render->setWordWrap(1);
		render->slotSetText(qtemp);
	}
	else
	{
		push_log("[ЦЕНТР СПРАВКИ]Не удалось открыть Manual.md, возможно файл отсутствует");
		render->slotSetText(tr("Manual.md отсутствует"));
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
	btnLayout->setAlignment(Qt::AlignCenter);

	//Настраиваем визуал
	renderFrame->setAutoFillBackground(1);
	renderFrame->setLayout(renderComposeManager);
	renderFrame->setFixedHeight(708);
	renderComposeManager->addWidget(mdsArea);

	this->setFixedSize(1024, 768);
	this->setLayout(renderComposeManager);
}

void helpCenter::slotShowWindow()
{
	this->show();
}

void helpCenter::slotHideWindow()
{
	this->hide();
}
