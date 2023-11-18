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
	mdInput = str;
	this->setText(str);
}
void mdScreen::slotSetHyperlink(int start)
{
	int size = 0;
	std::string ref(mdInput.toStdString());
	//std::string plaintxt(ref.substr(0, start));
	//this->setText(QString::fromStdString(plaintxt));
	//ref.insert(start, 1, '!');
	for (uint16_t index = start+1;index < ref.size();++index)
	{
		if (ref[index] == ' ')
		{
			size = index - start;
			break;
		}
		else if (index == ref.size()-1)
			size = index - start+1;
	}
	ref.insert(size+start, 1, '!');
	qDebug() << "Detect regular expression at " << QString::number(start) << " pos and " << QString::number(size+1) << " size. Substr = " << QString::fromStdString(ref.substr(start,size+1));
	this->setText(QString::fromStdString(ref));
	mdInput = QString::fromStdString(ref);
}
