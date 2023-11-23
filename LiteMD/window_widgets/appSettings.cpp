#include "appSettings.h"
#include <QtWidgets>
appSettings::appSettings(QWidget* aWgt) : QDialog(aWgt)
{
	setModal(1);
	setWindowTitle("LiteMD Settings");
	settingsLister = new QTabWidget(this);
	btnOk = new QPushButton("&Ok");
	btnCancel = new QPushButton("&Cancel");
	controlBtnLay = new QHBoxLayout;
	dialogWindow = new QVBoxLayout;
	controlBtnLay->addWidget(btnOk);
	controlBtnLay->addWidget(btnCancel);
	dialogWindow->addWidget(settingsLister);
	dialogWindow->addLayout(controlBtnLay);
	setLayout(dialogWindow);
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();
	if (!connect(btnCancel, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();

	workprogress = new QLabel("<H1>Work in progress, come later))</H1>");
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

	settingsLister->addTab(capTab, "Basic");
	setFixedSize(800, 600);
}
