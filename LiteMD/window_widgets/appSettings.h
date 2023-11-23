#pragma once
#include <QtWidgets>
class appSettings : public QTabWidget
{
	private:
		QLabel* workprogress;
	public:
		appSettings(QWidget* aWgt = 0);
};
