#include "logger.h"

logger::logger(QWidget* log) : QDialog(log)
{
	//����� ��������� ��� ���� ����� ���� �������� �� ����� ���
	setModal(1);
	this->setWindowTitle(tr("logTitle"));

	//����� ���� ��� ������������� ������ ����
	setFixedSize(800, 600);

	//�������� ���������� ������
	buttonLay = new QHBoxLayout;

	//������ ������� ����� ������ ������� ����� �����
	qApp->installEventFilter(this);

	//������
	this->setWindowIcon(QIcon("icon.ico"));

	//�������� ���������� ������ � �������� � ���������� ����
	builder = new QVBoxLayout;

	//������ �������� �� tab_logs.cpp
	clearLog = new QPushButton(tr("clearLog"));
	saveLog = new QPushButton(tr("saveLog"));
	logFrame = new QTextEdit;

	//����� � � �������
	frame = new QGroupBox;
	frameFix = new QVBoxLayout;

	//������ ������
	buttonLay->addWidget(saveLog);
	buttonLay->addWidget(clearLog);

	//������ ����
	builder->addWidget(logFrame);
	builder->addLayout(buttonLay);

	//���������
	frame->setLayout(builder);
	frame->setTitle(tr("logHint"));
	frameFix->addWidget(frame);

	//���������� ���������� � ������
	this->setLayout(frameFix);

	//������ ���������
	logFrame->setReadOnly(1);		//���������� ����������� ������ � ���� �����
}
