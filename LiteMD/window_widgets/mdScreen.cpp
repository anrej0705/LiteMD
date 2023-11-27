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
	advRegexHyperlink = new std::wregex(L"(\\[(.*?)\\])(\\(\\S{1,})\\)", std::wregex::collate);
	//advRegexHyperlink = new std::wregex(L"\\[(.*?)\\]\\(.*?\\)", std::wregex::collate);
	//advRegexHyperlink = new std::wregex(L"\\[(.*?)\\]\\((.*?)\\)", std::wregex::collate);
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
	lengShift = 0;
	buffer = L"";
	mdInput = mdFormatted.toStdWString();

	int indexIn = 0;
	int index = 0;
	int range = 0;
	int urname_length = 0;
	std::wstring bufferLink=L"";
	std::wstring bufferName=L"";
	for (int i = 0;i < mdInput.size();++i)
	{
		index = mdInput.find('[', index);
		range = mdInput.find(']', index);
		if (index == -1 || range == -1)
			break;
		buffer = mdInput.substr(index, range - index+1);
		bufferName = buffer;
		indexIn = index;
		index = range;
		if (index!=mdInput.size()-1)
			if(mdInput.at(index + 1) == '(')
			{
				range = mdInput.find(')', index + 1);
				if (range == -1)
					break;
				buffer = mdInput.substr(index + 1, range - index);
				bufferLink = buffer;
				buffer = L"";
				buffer.append(bufferLink);
				buffer.append(bufferName);
				urname_length = buffer.size();
				buffer.replace(0, 1, L"<");
				lengShift += 4;
				buffer.insert(1, std::wstring(vType.tag_href_open.begin(),vType.tag_href_open.end()));
				buffer.insert(buffer.find(')'), std::wstring(vType.tag_href_close.begin(), vType.tag_href_close.end()));
				buffer.replace(buffer.find(')'), 2, L">");
				buffer.replace(buffer.find(']'), 1, std::wstring(vType.tag_href_end.begin(), vType.tag_href_end.end()));
				mdInput.replace(indexIn, urname_length, buffer);
				mdFormatted = QString::fromStdWString(mdInput);
			}
	}

	this->setText(mdFormatted);
}
