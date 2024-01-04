#include "appSettings.h"
#include <QtWidgets>
appSettings::appSettings(QWidget* aWgt) : QDialog(aWgt)
{
	//���� ��������, ����� ����������
	setModal(1);
	setWindowTitle(tr("LiteMD Settings"));

	//��������������� ��������� �������
	configureBasicSettingsTab();

	//�������������� ���������
	settingsLister = new QTabWidget(this);
	btnOk = new QPushButton(tr("&Ok"));
	btnCancel = new QPushButton(tr("&Cancel"));
	controlBtnLay = new QHBoxLayout;
	dialogWindow = new QVBoxLayout;

	//��������� ������ ������ � �������
	controlBtnLay->addWidget(btnOk);
	controlBtnLay->addWidget(btnCancel);
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
