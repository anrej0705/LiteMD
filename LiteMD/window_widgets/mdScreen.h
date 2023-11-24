#pragma once
#include <QtWidgets>
#include <regex>

class mdScreen : public QLabel
{
	Q_OBJECT
	private:
		int lengShift;
		std::wregex* regexHyperlink;
		std::wregex* advRegexHyperlink;
		std::wstring mdInput;
		QString mdFormatted;
		std::wstring hyperlinkParser(std::wstring&);
	public:
		mdScreen(QWidget* scrWgt = 0);
	public slots:
		void slotSetText(const QString&);	//Слот для принятия сигнала textChanged() и высылки сигнала с текстом
};