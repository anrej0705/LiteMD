#include "appSettings.h"
#include <QtWidgets>
appSettings::appSettings(QWidget* aWgt) : QDialog(aWgt)
{
	//Окно настроек, будет переписано
	setModal(1);
	setWindowTitle(tr("LiteMD Settings"));

	//Предварительная настройка вкладок
	configureBasicSettingsTab();

	//Инициализируем указатели
	settingsLister = new QTabWidget(this);
	btnOk = new QPushButton(tr("&Ok"));
	btnCancel = new QPushButton(tr("&Cancel"));
	controlBtnLay = new QHBoxLayout;
	dialogWindow = new QVBoxLayout;

	//Добавляем виджет кнопок и вкладок
	controlBtnLay->addWidget(btnOk);
	controlBtnLay->addWidget(btnCancel);
	dialogWindow->addWidget(settingsLister);
	dialogWindow->addLayout(controlBtnLay);

	//Устанавливаем менеджер как основной виджет
	setLayout(dialogWindow);
	
	//Устанавливаем связи кнопок
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();
	if (!connect(btnCancel, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();

	//Ставим заглушку
	workprogress = new QLabel("<H1>"+tr("Work in progress, come later))")+"</H1>");
	QPixmap workprg_cap("ress\\work_progress_cap.png");
	QLabel* pxmap = new QLabel;
	QWidget* capTab = new QWidget;
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
	settingsLister->addTab(capTab, tr("Cap"));

	//Задаем фиксированный размер
	setFixedSize(800, 600);
}
