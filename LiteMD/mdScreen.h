#pragma once
#include <QtWidgets>
#include <regex>

class mdScreen : public QLabel
{
	Q_OBJECT
	private:
		int lengShift;
		std::regex* regexHyperlink;
		std::string mdInput;
		QString mdFormatted;
	public:
		mdScreen(QWidget* scrWgt = 0);
	public slots:
		void slotSetText(const QString&);	//Слот для принятия сигнала textChanged() и высылки сигнала с текстом
};