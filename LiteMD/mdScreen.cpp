#include <QtWidgets>
#include "mdScreen.h"
mdScreen::mdScreen(QWidget* scrWgt) : QLabel(scrWgt)
{
	QString link = "http://www.google.com";
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//setText("test");
	setText("<a href=\"" + link + "\">"+ link+"");
	setTextInteractionFlags(Qt::TextBrowserInteraction);
	setOpenExternalLinks(1);
}
//Простой слот - принимает сигнал и изменяет виджет
void mdScreen::slotSetText(const QString& str)
{
	mdInput = str.toStdString();
	mdFormatted = QString::fromStdString(mdInput);
	this->setText(mdFormatted);
}
void mdScreen::slotSetHyperlink(int start,int length)
{

	//std::string leftPart = mdInput.substr(0, start);
	//std::string rigthPart = mdInput.substr(length, mdInput.size());
	//std::string url = mdInput.substr(start, length-start);
	//url = leftPart + "<a href=\"" + url + "\">" + url + "" + rigthPart;
	//mdInput = url;
	//mdFormatted = QString::fromStdString(mdInput);
	//this->setText(mdFormatted);
}
