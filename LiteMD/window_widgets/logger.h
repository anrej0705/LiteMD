#pragma once
#include <QtWidgets>
#include "ui_update_event.h"
#include "appSettings.h"

class logger : public QDialog
{
	Q_OBJECT
	private:
		//������
		QPushButton* clearLog;
		QPushButton* saveLog;

		//�������
		QLabel* logHint;

		//���� �����
		QTextEdit* logFrame;

		//��������� ����������
		QHBoxLayout* buttonLay;
		QVBoxLayout* builder;
	public:
		logger(QWidget* log = 0);
};