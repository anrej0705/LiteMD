#pragma once
#include <QtWidgets>

class mdScreen : public QLabel
{
	Q_OBJECT
	private:
		std::string mdInput;
		QString mdFormatted;
	public:
		mdScreen(QWidget* scrWgt = 0);
	public slots:
		void slotSetText(const QString&);	//Слот для принятия сигнала textChanged() и высылки сигнала с текстом
		void slotSetHyperlink(int,int);	//Слот преобразующий строку в ссылку
};