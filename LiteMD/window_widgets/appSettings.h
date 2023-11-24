#pragma once
#include <QtWidgets>
class appSettings : public QDialog
{
	Q_OBJECT
	private:
		QTabWidget* settingsLister;
		QLabel* workprogress;
		QPushButton* btnOk;
		QPushButton* btnCancel;
		QHBoxLayout* controlBtnLay;
		QVBoxLayout* dialogWindow;
	public:
		appSettings(QWidget* aWgt = 0);
};
