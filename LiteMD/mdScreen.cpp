#include <QtWidgets>
#include "mdScreen.h"
#include <string>
#include <regex>

struct visualTags
{
	std::string tag_href_open = "a href=\"";
	std::string tag_href_close = "\"";
	std::string tag_href_end = "</a>";
}vType;

mdScreen::mdScreen(QWidget* scrWgt) : QLabel(scrWgt)
{
	lengShift = 0;
	regexHyperlink = new std::regex("[<]{1,1}\\S{1,}[>]{1,1}", std::regex::collate);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setTextInteractionFlags(Qt::TextBrowserInteraction);
	setOpenExternalLinks(1);
}
//Простой слот - принимает сигнал и изменяет виджет
void mdScreen::slotSetText(const QString& str)
{
	mdInput = str.toStdString();
	mdFormatted = QString::fromStdString(mdInput);

	//Ищем ссылки с помощью регулярного выражения
	std::sregex_iterator beg{mdInput.cbegin(),mdInput.cend(),*regexHyperlink};
	std::sregex_iterator end{};
	std::string buffer = "NULL";
	std::string test;

	lengShift = 0;

	int i = 0;
	QRegExp rx("[<]{1,1}\\S{1,}[>]{1,1}");
	rx.setPatternSyntax(QRegExp::Wildcard);
	i = rx.indexIn(mdFormatted);
	i = rx.indexIn(mdFormatted);

	for (std::sregex_iterator i = beg; i != end;++i)
	{
		//Вставляем тег "a href="
		mdFormatted.insert(i->position() + lengShift + 1, QString::fromStdString(vType.tag_href_open));
		test = mdFormatted.toStdString();
		//Вытаскиваем из текста чистую ссылку
		buffer = std::string(mdFormatted.toStdString()).substr(i->position() + lengShift + vType.tag_href_open.size() + 1,i->length()-2);
		//Прикрепляем закрывающий тег
		mdFormatted.insert(i->position() + buffer.size() + vType.tag_href_open.size() + lengShift + 1, QString::fromStdString(vType.tag_href_close));
		test = mdFormatted.toStdString();
		//Прикрепляем подпись ссылки
		mdFormatted.insert(i->position() + buffer.size() + vType.tag_href_open.size() + vType.tag_href_close.size() + lengShift + 2,QString::fromStdString(buffer));
		test = mdFormatted.toStdString();
		//Прикрепляем закрывающий тег
		mdFormatted.insert(i->position() + buffer.size() + vType.tag_href_open.size() + vType.tag_href_close.size() + buffer.size() + lengShift + 2, QString::fromStdString(vType.tag_href_end));
		test = mdFormatted.toStdString();
		//Вычисляем смещение открывающий тег + закрывающая часть тега + вставленное текстовое представление ссылки+ закрывающий тег
		lengShift += vType.tag_href_open.size() + vType.tag_href_close.size() + buffer.size() + vType.tag_href_end.size();
		test = mdFormatted.toStdString();
	}

	this->setText(mdFormatted);
}
