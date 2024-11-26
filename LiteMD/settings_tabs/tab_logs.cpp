#include "appSettings.h"
#include "ui_update_event.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

void appSettings::configureLogsTab()
{
	//������������� �������
	tabLogs = new QWidget;

	//�������������� �����
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//�������� ������������� ���������� - ����� � ������ � ��� ������
	QVBoxLayout* builder = new QVBoxLayout;
	
	//�������� ��������������� ���������� - ��� ���� ������
	QHBoxLayout* buttonLay = new QHBoxLayout;

	//������������� ���������
	clearLog = new QPushButton(tr("clearLog"));
	saveLog = new QPushButton(tr("saveLog"));
	logHint = new QLabel(tr("logHint"));
	logBox = new QPlainTextEdit;

	//��������� ���������� ����
	logBox->setReadOnly(1);

	//��������� �������
	logHint->setAlignment(Qt::AlignLeft);

	//������ ������
	buttonLay->addWidget(saveLog);
	buttonLay->addWidget(clearLog);

	//������ ����
	builder->addWidget(logHint);
	builder->addWidget(logBox);
	builder->addLayout(buttonLay);

	//���������� ���������� � ������
	tabLogs->setLayout(builder);
}
