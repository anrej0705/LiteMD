#include "appSettings.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

void appSettings::configureExtendedTab()
{
	//������ ������ ����� �� ����� �� ���� �������� ���� �����
	extendedMd = new QWidget;

	//�������������� �����
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//�������� ������������� ���������� - ����� � ������ � ��� ������
	QVBoxLayout* builder = new QVBoxLayout;

	//�������� ��������������� ���������� - ��� ���� ������
	QHBoxLayout* buttonLay = new QHBoxLayout;

	//������������� ���������
	parseStrikethroughHint = new QLabel(tr("parseStrikethroughHint"));
	parseStrikethrough = new QCheckBox;

	parseStrikethrough->setChecked(1);

	//������ ������
	parseStrikethroughHint->setFixedHeight(SETTINGS_HEIGH);
	parseStrikethrough->setFixedHeight(SETTINGS_HEIGH);

	//�������������� �����
	QGroupBox* extended_box = new QGroupBox;
	extended_box->setAutoFillBackground(1);

	//�������������� ����� ��� �������� � ��������� �������������� ��������������
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//��������� ����������
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//��������������� ����������
	QHBoxLayout* extended_box_lay = new QHBoxLayout;
	QHBoxLayout* manager = new QHBoxLayout;

	//��������� � ����������
	lbl_lay->addWidget(parseStrikethroughHint);

	interact_lay->addWidget(parseStrikethrough);

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
	extended_box_lay->addWidget(lbl_box);
	extended_box_lay->addWidget(interact_box);

	//����������� �������������� ��������
	extended_box->setLayout(extended_box_lay);
	manager->addWidget(extended_box);

	extendedMd->setLayout(manager);
}
