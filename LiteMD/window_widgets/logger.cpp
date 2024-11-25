#include "logger.h"

logger::logger(QWidget* log) : QDialog(log)
{
	//����� ��������� ��� ���� ����� ���� �������� �� ����� ���
	setModal(1);

	//����� ���� ��� ������������� ������ ����
	setFixedSize(800, 600);

	//�������� ���������� ������
	buttonLay = new QHBoxLayout;

	//�������� ���������� ������ � �������� � ���������� ����
	builder = new QVBoxLayout;

	//������ �������� �� tab_logs.cpp
	clearLog = new QPushButton(tr("clearLog"));
	saveLog = new QPushButton(tr("saveLog"));
	logHint = new QLabel(tr("logHint"));
	logFrame = new QTextEdit;

	//��������� �������
	logHint->setAlignment(Qt::AlignLeft);

	//������ ������
	buttonLay->addWidget(saveLog);
	buttonLay->addWidget(clearLog);

	//������ ����
	builder->addWidget(logHint);
	builder->addWidget(logFrame);
	builder->addLayout(buttonLay);

	//���������� ���������� � ������
	this->setLayout(builder);

}
