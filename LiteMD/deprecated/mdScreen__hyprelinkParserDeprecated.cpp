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
	for (std::wsregex_iterator it = std::wsregex_iterator(str.cbegin(), str.cend(), *regexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		//Узнаем длину текста с мусором до регулярки
		range = it->position();
		//Копируем в буфер
		buffer = str.substr(index, range - index);
		//Отправляем в обработчик который чистит текст от символов для исключения ложного срабатывания
		preprocessTrianlgeBrackets(buffer);
		//preprocessTrianlgeBrackets(buffer);
		//Сохраняем мусор в контейнер
		garbage.push_back(buffer);
		//Ищем закрывающую скобку
		for (int i = range; i < str.size(); ++i)
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
		preprocessTrianlgeBrackets(buffer);
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