#include <QtWidgets>
#include "mdScreen.h"
#include "syntax_preprocessor.h"
#include "syntax_postprocessor.h"
#include "symbolCleaner.h"
#include "urlBasicSimplifiedParser.h"
#include "urlBasicParser.h"
#include "urlAdvancedParser.h"
#include <string>
#include <regex>

//Контейнер для хранения готовых html конструкций
struct visualTags
{
	std::string tag_href_open = "a href=\"";
	std::string tag_href_close = "\"";
	std::string tag_href_end = "</a>";
	std::string left_angl_bracket_replacer = "&#60";
	std::string right_angl_bracket_replacer = "&#62";
}vType;

void mdScreen::textProcessorDeprecated()
{
	//Обрабатываем текст препроцессором
	mdInput = hyperlinkParser(mdInput);

	//Обрабатываем и вставляем ссылки
	//Ищем ссылки с помощью регулярного выражения
	std::wsregex_iterator beg{mdInput.cbegin(),mdInput.cend(),*regexHyperlink};
	std::wsregex_iterator end{};
	std::wstring buffer = QString("NULL").toStdWString();
	std::wstring debugPeek;

	//Сбрасываем смещение
	lengShift = 0;
	mdFormatted = QString::fromStdWString(mdInput);

	//Выполняем пока в строке существуют регулярки
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

	//Очищаем от артефактов обработки и сбрасываем флаги и буфер
	processCRLF(mdFormatted);
	lengShift = 0;
	buffer = L"";
	mdInput = mdFormatted.toStdWString();

	//Обработка альтернативных ссылок
	int indexIn = 0;
	int index = 0;
	int range = 0;
	int urname_length = 0;
	std::wstring bufferLink=L"";
	std::wstring bufferName=L"";
	//Выполняем пока не дошли до конца строки
	for (int i = 0;i < mdInput.size();++i)
	{
		//Получаем указатель на пользовательское название ссылки
		index = mdInput.find('[', index);
		range = mdInput.find(']', index);
		//Если внезапно чего-то этого не оказалось то прерываем
		if (index == -1 || range == -1)
			break;
		//Копируем пользовательское название в буффер
		buffer = mdInput.substr(index, range - index+1);
		bufferName = buffer;
		//Копируем указатели на начало и длину
		indexIn = index;
		index = range;
		//Выполяем пока указатель не дошёл до конца строки
		if (index!=mdInput.size()-1)
			//Если найдена открывающая скобка то выполняем
			if(mdInput.at(index + 1) == '(')
			{
				//Получаем длину ссылки до закрывающей скобки
				range = mdInput.find(')', index + 1);
				//Если закрывающей скобки нет то выходим
				if (range == -1)
					break;
				//Копируем в буфер ссылку
				buffer = mdInput.substr(index + 1, range - index);
				bufferLink = buffer;
				buffer = L"";
				//Прикрепляем к буферу сначала ссылку а затем пользовательское название ссылки
				buffer.append(bufferLink);
				buffer.append(bufferName);
				//Вычисляем размер ссылки
				urname_length = buffer.size();
				//Преобразуем ссылку в html формат
				buffer.replace(0, 1, L"<");
				lengShift += 4;
				//Вставляем тег a href
				buffer.insert(1, std::wstring(vType.tag_href_open.begin(),vType.tag_href_open.end()));
				buffer.insert(buffer.find(')'), std::wstring(vType.tag_href_close.begin(), vType.tag_href_close.end()));
				//Вставляем закрывающий тег
				buffer.replace(buffer.find(')'), 2, L">");
				buffer.replace(buffer.find(']'), 1, std::wstring(vType.tag_href_end.begin(), vType.tag_href_end.end()));
				//Вставляем ссылку
				mdInput.replace(indexIn, urname_length, buffer);
				//Переносим в массив на вывод
				mdFormatted = QString::fromStdWString(mdInput);
			}
	}
}
