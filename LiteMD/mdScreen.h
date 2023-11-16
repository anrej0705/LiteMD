#pragma once
#include <QtWidgets>

class mdScreen : public QLabel
{
	Q_OBJECT
	private:
	public:
		mdScreen(QWidget* scrWgt = 0);
	public slots:
		void slotSetText(const QString&);	//Слот для принятия сигнала textChanged() и высылки сигнала с текстом
};