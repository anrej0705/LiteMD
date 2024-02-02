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

mdScreen::mdScreen(QWidget* scrWgt) : QLabel(scrWgt)
{
	//Задаем формат отображения как html
	setTextFormat(Qt::RichText);
	lengShift = 0;
	//Инициализируем регексы
	regexHyperlink = new std::wregex(L"[<]{1,1}(http|https|ftp://){0,1}\\S{1,}[>]{1,1}", std::wregex::collate);
	advRegexHyperlink = new std::wregex(L"(\\[(.*?)\\])(\\(\\S{1,})\\)", std::wregex::collate);
	//Задаем выравниваем на вверх и лево
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//Разрешаем интерактивный текст
	setTextInteractionFlags(Qt::TextBrowserInteraction);
	setOpenExternalLinks(1);
}

//Парсер и препроцессор <url> по регексу
std::wstring mdScreen::hyperlinkParser(std::wstring& str)
{
	//Создаем переменные для хранения точки входа и длины регулярки
	int index = 0;
	int range = 0;
	std::wstring buffer = str;

	//Создаем контейнер для хранения регулярок и символов которые не прошли проверку регексом
	std::list<std::wstring> garbage;
	std::list<std::wstring> xpressions;

	//Пока регекс обнаруживает что-то похожее на нужную ссылку - выполняем из раза в раз
	for (std::wsregex_iterator it = std::wsregex_iterator(str.cbegin(), str.cend(), *regexHyperlink);it != std::wsregex_iterator();++it)
	{
		//Узнаем длину текста с мусором до регулярки
		range = it->position();
		//Копируем в буфер
		buffer = str.substr(index, range - index);
		//Отправляем в обработчик который чистит текст от символов для исключения ложного срабатывания
		buffer = symbolCleaner(buffer);
		//preprocessTrianlgeBrackets(buffer);
		//Сохраняем мусор в контейнер
		garbage.push_back(buffer);
		//Ищем закрывающую скобку
		for (int i = range;i < str.size();++i)
		{
			if (mdInput.at(i) == '>')
			{
				//Как только находим - записываем позицию и выходим из цикла
				index = i + 1;
				break;
			}
		}
		//Сохраням в буфер найденную конструкцию и в записываем в контейнер
		buffer = str.substr(range, index - range);
		xpressions.push_back(buffer);
	}
	//Если в конце что-то есть то выполняем
	if (index < mdInput.size())
	{
		//Помещяем в буфер всё остальное до конца, обрабатываем от мусора и помещаем результат в контейнер
		buffer = mdInput.substr(index, mdInput.size() - index);
		buffer = symbolCleaner(buffer);
		//preprocessTrianlgeBrackets(buffer);
		garbage.push_back(buffer);
	}
	//Чистим буфер
	buffer = std::wstring(L"");
	//Выполняем без конца
	for (;;)
	{
		//Если контейнер с посторонними символами не пуст то вставляем и чистим на 1 позицию
		if (garbage.size() != 0)
		{
			buffer += garbage.front();
			garbage.pop_front();
		}
		//Если контейнер с url ссылками не пуст то вставляем и чистим на 1 позицию
		if (xpressions.size() != 0)
		{
			buffer += xpressions.front();
			xpressions.pop_front();
		}
		//Если оба контейнера пусты то выходим
		if (garbage.size() == 0 && xpressions.size() == 0)
			break;
	}
	//Возвращаем пересобранную строку где лишние символы заменены эквивалентами в HTML коде
	return buffer;
}

//Простой слот - принимает сигнал и изменяет виджет
void mdScreen::slotSetText(const QString& str)
{
	//Преобразаем текст к 16 битному формату
	mdInput = str.toStdWString();
	//Копираем в поле вывода
	mdFormatted = QString::fromStdWString(mdInput);

	mdInput = symbolCleaner(mdInput);
	mdInput = basicSimplifiedUrlParser(mdInput);

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

	//Отправляем на показ
	this->setText(mdFormatted);
}
