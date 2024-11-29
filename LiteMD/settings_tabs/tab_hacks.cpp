#include "appSettings.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

void appSettings::configureHackTab()
{
	//������ ������ ����� �� ����� �� ���� �������� ���� �����
	hacks = new QWidget;

	//�������������� �����
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//������������� ���������
	parseUnderlinedHint = new QLabel(tr("parseUnderlinedHint"));
	parseUnderlined = new QCheckBox;

	parseUnderlined->setChecked(1);

	//������ ������
	parseUnderlinedHint->setFixedHeight(SETTINGS_HEIGH);
	parseUnderlined->setFixedHeight(SETTINGS_HEIGH);

	//�������������� �����
	QGroupBox* hacks_box = new QGroupBox;
	hacks_box->setAutoFillBackground(1);

	//�������������� ����� ��� �������� � ��������� �������������� ��������������
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//��������� ����������
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//��������������� ����������
	QHBoxLayout* hacks_box_lay = new QHBoxLayout;
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
	hacks_box_lay->addWidget(lbl_box);
	hacks_box_lay->addWidget(interact_box);

	//����������� �������������� ��������
	hacks_box->setLayout(hacks_box_lay);
	manager->addWidget(hacks_box);

	hacks->setLayout(manager);
}
