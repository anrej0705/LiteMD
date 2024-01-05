#include "appSettings.h"
void appSettings::configureDownloaderSettingsTab()
{
	downloaderSettings = new QWidget;

	//�������������� �����

	//�������������� �������� ��������������

	//�������� �� ���������� ��������

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
