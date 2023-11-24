#include <QtWidgets>
#include "mdScreen.h"
#include "syntax_preprocessor.h"
#include "syntax_postprocessor.h"
#include <string>
#include <regex>

struct visualTags
{
	std::string tag_href_open = "a href=\"";
	std::string tag_href_close = "\"";
	std::string tag_href_end = "</a>";
	std::string left_angl_bracket_replacer = "&#60";
	std::string right_angl_bracket_replacer = "&#62";
}vType;

mdScreen::mdScreen(QWidget* scrWgt) : QLabel(scrWgt)
{
	setTextFormat(Qt::RichText);
	lengShift = 0;
	regexHyperlink = new std::wregex(L"[<]{1,1}(http|https|ftp)(://)\\S{1,}[>]{1,1}", std::wregex::collate);
	//regexHyperlink = new std::wregex(L"[<]{1,1}\\S{1,}[>]{1,1}", std::wregex::collate);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setTextInteractionFlags(Qt::TextBrowserInteraction);
	setOpenExternalLinks(1);
}

//Парсер <url> по регексу
std::wstring mdScreen::hyperlinkParser(std::wstring& str)
{
	int index = 0;
	int range = 0;
	std::wstring buffer = str;

	std::list<std::wstring> garbage;
	std::list<std::wstring> xpressions;

	for (std::wsregex_iterator it = std::wsregex_iterator(str.cbegin(), str.cend(), *regexHyperlink);it != std::wsregex_iterator();++it)
	{
		range = it->position();
		buffer = str.substr(index, range - index);
		preprocessTrianlgeBrackets(buffer);
		garbage.push_back(buffer);
		for (int i = range;i < str.size();++i)
		{
			if (mdInput.at(i) == '>')
			{
				index = i + 1;
				break;
			}
		}
		buffer = str.substr(range, index - range);
		xpressions.push_back(buffer);
	}
	if (index < mdInput.size())
	{
		buffer = mdInput.substr(index, mdInput.size() - index);
		preprocessTrianlgeBrackets(buffer);
		garbage.push_back(buffer);
	}
	buffer = std::wstring(L"");
	for (;;)
	{
		if (garbage.size() != 0)
		{
			buffer += garbage.front();
			garbage.pop_front();
		}
		if (xpressions.size() != 0)
		{
			buffer += xpressions.front();
			xpressions.pop_front();
		}
		if (garbage.size() == 0 && xpressions.size() == 0)
			break;
	}
	return buffer;
}

//Простой слот - принимает сигнал и изменяет виджет
void mdScreen::slotSetText(const QString& str)
{
	mdInput = str.toStdWString();
	mdFormatted = QString::fromStdWString(mdInput);

	mdInput = hyperlinkParser(mdInput);

	//Обрабатываем и вставляем ссылки
	//Ищем ссылки с помощью регулярного выражения
	std::wsregex_iterator beg{mdInput.cbegin(),mdInput.cend(),*regexHyperlink};
	std::wsregex_iterator end{};
	std::wstring buffer = QString("NULL").toStdWString();
	std::wstring debugPeek;

	lengShift = 0;
	mdFormatted = QString::fromStdWString(mdInput);

	for (std::wsregex_iterator i = beg; i != end;++i)
	{
		//Вставляем тег "a href="
 		mdFormatted.insert(i->position() + lengShift + 1, QString::fromStdString(vType.tag_href_open));
		debugPeek = mdFormatted.toStdWString();
		//Вытаскиваем из текста чистую ссылку
		buffer = std::wstring(mdFormatted.toStdWString()).substr(i->position() + lengShift + vType.tag_href_open.size() + 1,i->length()-2);
		//Прикрепляем закрывающий тег
		mdFormatted.insert(i->position() + buffer.size() + vType.tag_href_open.size() + lengShift + 1, QString::fromStdString(vType.tag_href_close));
		debugPeek = mdFormatted.toStdWString();
		//Прикрепляем подпись ссылки
		mdFormatted.insert(i->position() + buffer.size() + vType.tag_href_open.size() + vType.tag_href_close.size() + lengShift + 2,QString::fromStdWString(buffer));
		debugPeek = mdFormatted.toStdWString();
		//Прикрепляем закрывающий тег
		mdFormatted.insert(i->position() + buffer.size() + vType.tag_href_open.size() + vType.tag_href_close.size() + buffer.size() + lengShift + 2, QString::fromStdString(vType.tag_href_end));
		debugPeek = mdFormatted.toStdWString();
		//Вычисляем смещение открывающий тег + закрывающая часть тега + вставленное текстовое представление ссылки+ закрывающий тег
		lengShift += vType.tag_href_open.size() + vType.tag_href_close.size() + buffer.size() + vType.tag_href_end.size();
		debugPeek = mdFormatted.toStdWString();
		//qApp->processEvents();	Работает нестабильно
	}
	
	processCRLF(mdFormatted);

	this->setText(mdFormatted);
}
