#include "logger.h"
#include "logger_backend.h"
#include <boost/container/vector.hpp>

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
	logFrame = new QPlainTextEdit;

	//������ ������ �������� ����� ������ �����
	clearLog->setFixedHeight(48);
	saveLog->setFixedHeight(48);

	//����� � � �������
	frame = new QGroupBox;
	frameFix = new QVBoxLayout;

	//������ ������
	buttonLay->addWidget(saveLog);
	buttonLay->addWidget(clearLog);

	//������ ����
	builder->addWidget(logFrame);

	//���������
	frame->setLayout(builder);
	frame->setTitle(tr("logHint"));
	frameFix->addWidget(frame);
	frameFix->addLayout(buttonLay);

	//���������� ���������� � ������
	this->setLayout(frameFix);

	//������ ���������
	logFrame->setReadOnly(1);		//���������� ����������� ������ � ���� �����

	if (!connect(clearLog, SIGNAL(clicked()), this, SLOT(slot_clear_logs())))
		QErrorMessage::qtHandler();	//���������� ������ ������� �����
}

void logger::slot_read_n_show()
{
	boost::container::vector<QString> container = logger_backend::getInstance().get_logs();
	for (uint32_t _index = 0; _index < container.size(); ++_index)
	{
		logFrame->appendPlainText(container.at(_index));
	}
	this->show();
}

void logger::slot_clear_logs()
{	//������ ���� �� �������
	logger_backend::getInstance().clear_logs();
	logFrame->clear();
}
