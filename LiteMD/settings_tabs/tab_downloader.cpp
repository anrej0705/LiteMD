#include "appSettings.h"
#include "global_definitions.h"
extern "C"
{
	#include "globalFlags.h"
}

void appSettings::configureDownloaderSettingsTab()
{
	downloaderSettings = new QWidget;

	//�������������� �����
	allowWarningsHint = new QLabel(tr("Show one-time deprecated warning"));
	allowCacheHint = new QLabel(tr("Enable content caching"));

	//�������������� �������� ��������������
	allowWarnings = new QCheckBox;
	allowCache = new QCheckBox;

	//�������� �� ���������� ��������
	allowCache->setChecked(0);
	allowCache->setDisabled(1);

	allowWarnings->setChecked(1);

	//������ ������
	allowWarningsHint->setFixedHeight(SETTINGS_HEIGH);
	allowCacheHint->setFixedHeight(SETTINGS_HEIGH);
	allowWarnings->setFixedHeight(SETTINGS_HEIGH);
	allowCache->setFixedHeight(SETTINGS_HEIGH);

	//�������������� �����
	QGroupBox* downloader_box = new QGroupBox;
	downloader_box->setAutoFillBackground(1);

	//�������������� ����� ��� �������� � ��������� �������������� ��������������
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//��������� ����������
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//��������������� ����������
	QHBoxLayout* downloader_box_lay = new QHBoxLayout;
	QHBoxLayout* manager = new QHBoxLayout;

	//��������� � ����������
	lbl_lay->addWidget(allowWarningsHint);
	lbl_lay->addWidget(allowCacheHint);

	interact_lay->addWidget(allowWarnings);
	interact_lay->addWidget(allowCache);

	//����������� ������� ���������� ��������� - ������
	lbl_lay->setAlignment(Qt::AlignTop);
	interact_lay->setAlignment(Qt::AlignTop);

	//����������� ��������� ���������� � ��������
	lbl_box->setLayout(lbl_lay);
	interact_box->setLayout(interact_lay);

	//������ ������ �����
	lbl_box->setFixedWidth(300);
	interact_box->setFixedWidth(300);

	//��������� ������� � ����������� � ���������� ��������������
	//basic_box_lay->setAlignment(Qt::AlignLeft);
	downloader_box_lay->addWidget(lbl_box);
	downloader_box_lay->addWidget(interact_box);

	//����������� �������������� ��������
	downloader_box->setLayout(downloader_box_lay);
	manager->addWidget(downloader_box);

	downloaderSettings->setLayout(manager);
}
