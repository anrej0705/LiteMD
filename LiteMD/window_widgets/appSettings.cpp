#include "appSettings.h"
#include <QtWidgets>
appSettings::appSettings(QWidget* aWgt) : QDialog(aWgt)
{
	//���� ��������, ����� ����������
	setModal(1);
	setWindowTitle(tr("LiteMD Settings"));

	//��������������� ��������� �������
	configureBasicSettingsTab();

	engine = new QQmlEngine;

	//�������������� ���������
	settingsLister = new QTabWidget(this);
	btnOk = new QPushButton(tr("&Ok"));
	btnCancel = new QPushButton(tr("&Cancel"));
	btnApply = new QPushButton(tr("&Apply"));
	controlBtnLay = new QHBoxLayout;
	dialogWindow = new QVBoxLayout;

	//��������� �������������
	controlBtnLay->setAlignment(Qt::AlignRight);

	//��������� ������
	btnOk->setFixedWidth(120);
	btnCancel->setFixedWidth(120);
	btnApply->setFixedWidth(120);

	//��������� ������ ������ � �������
	controlBtnLay->addWidget(btnOk);
	controlBtnLay->addWidget(btnCancel);
	controlBtnLay->addWidget(btnApply);
	dialogWindow->addWidget(settingsLister);
	dialogWindow->addLayout(controlBtnLay);

	//������������� �������� ��� �������� ������
	setLayout(dialogWindow);
	
	//������������� ����� ������
	if (!connect(btnOk, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();
	if (!connect(btnCancel, SIGNAL(clicked()), this, SLOT(hide())))
		QErrorMessage::qtHandler();
	if (!connect(langList, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_lang_selected(int))))
		QErrorMessage::qtHandler();
	if (!connect(btnApply, SIGNAL(clicked()), this, SLOT(slot_apply_settings())))
		QErrorMessage::qtHandler();
	

	//������ ��������
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

	//������ �������� �������
	settingsLister->addTab(basicSettings, tr("Basic"));
	settingsLister->addTab(capTab, tr("Cap"));

	//������ ������������� ������
	setFixedSize(800, 600);
}

void appSettings::slot_apply_settings()
{
	//������������� ������
	if (!QCoreApplication::installTranslator(&lmd_lng))
		QErrorMessage::qtHandler();
	qApp->installTranslator(&lmd_lng);
	btnCancel->setText(tr("&Cancel"));
}
