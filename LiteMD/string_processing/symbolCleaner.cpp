#include "symbolCleaner.h"
#include "regex.h"
#include <map>

std::wstring symbolCleaner(std::wstring& rawInput)
{
	//Создаем буффер, в котором будем чистить спецсимволы
	std::wstring buffer = rawInput;

	//Создаем строку в которой будут искаться регулярки
	std::wstring regBuffer(buffer);

	//Контейнер спецсимвола, сюда помещается первый попавшийся спецсимвол
	std::wstring symbol;

	//Хранилище параметров обработки строки
	std::wsmatch wstrMatch;

	//Указатель на индекс символа из библиотеки замены
	uint8_t replaceIndex = 0;

	//Указатель на предыдущее вхождение
	uint32_t prevIndex = 0;

	//Маркер, по которому потом будет заменяться пустые символы
	//std::wstring marker(L"\u009C");
	std::wstring marker(L"Z");

	//Хранит значение предыдущего спецсимвола
	uint32_t symbolDuplicate = 0;

	//Хранит индексы заменяемых спецсимволов
	std::map<int, int> symbolIndexes;

	//0 - первым мусор
	//1 - первым полезный фрагмент
	bool firstOrder = 0;

	std::wstring temp;

	//Хранилище мусора
	std::vector<std::wstring> garbage;
	std::vector<std::wstring> xpression;

	//Нарезаем строку на "полезные" фрагменты и мусор
	//Полезные фрагменты - прошедшие проверку регексом - обработке не подлежат на данном этапе
	//Мусор - чистится от служебных символов
	for (uint32_t index = 0; index < buffer.size(); ++index)
	{
		//Пробегаемся регулярками определяя свой-чужой(полезное-мусор)
		regBuffer = buffer.substr(index, buffer.size() - index);
		if (std::regex_search(regBuffer, wstrMatch, regexHyperlink))
		{
			if (index == wstrMatch.position() + index)
			{
				index == 0 ? firstOrder = 1 : firstOrder = 0;
				garbage.push_back(buffer.substr(prevIndex, index - prevIndex));
				xpression.push_back(buffer.substr(index, wstrMatch.length()));
				index += wstrMatch.length();
				prevIndex = index;
			}
		}
		if (std::regex_search(regBuffer, wstrMatch, simplifiedRegexHyperlink))
		{
			if (index == wstrMatch.position() + index)
			{
				index == 0 ? firstOrder = 1 : firstOrder = 0;
				garbage.push_back(buffer.substr(prevIndex, index - prevIndex));
				xpression.push_back(buffer.substr(index, wstrMatch.length()));
				index += wstrMatch.length();
				prevIndex = index;
			}
		}
		if (std::regex_search(regBuffer, wstrMatch, advRegexHyperlink))
		{
			if (index == wstrMatch.position() + index)
			{
				index == 0 ? firstOrder = 1 : firstOrder = 0;
				garbage.push_back(buffer.substr(prevIndex, index - prevIndex));
				xpression.push_back(buffer.substr(index, wstrMatch.length()));
				index += wstrMatch.length();
				prevIndex = index;
			}
		}
	}

	for (uint32_t iters = 0; iters < garbage.size(); ++iters)
	{
		//Проходимся по фрагменту заменяя служебные символы маркерами
		for (uint32_t index = 0; index < garbage.at(iters).size(); ++index)
		{
			//Если встречаем повтор символа то удаляем с предыдущей позиции
			if (!symbol.empty())
				if (garbage.at(iters).at(index) == symbol.at(0))
				{
					symbolIndexes.insert(std::map<int, int>::value_type(symbolDuplicate, replaceIndex));
					temp = garbage.at(iters).substr(symbolDuplicate, 1);
					garbage.at(iters).replace(symbolDuplicate, 1, marker);
					symbolDuplicate = index;
				}

			//Если новый встречается новый символ то считаем что очистка до него завершена
			//и поэтому чистим повторы нового символа
			if (symbolCollection.find(garbage.at(iters).at(index)) <= symbolCollection.size() && !symbol.empty())
				if (garbage.at(iters).at(index) != symbol.at(0))
					symbol.clear();

			//Если спецсимвол первый раз попался то запоминаем его
			if (symbolCollection.find(garbage.at(iters).at(index)) <= symbolCollection.size() && symbol.empty())
			{
				replaceIndex = symbolCollection.find(garbage.at(iters).at(index));
				symbol = symbolCollection.at(replaceIndex);
				symbolDuplicate = index;
			}
		}

		//Ищем остатки проходясь с начала в конец(поиск закрывающих символов
		for (uint32_t index = 0; index < garbage.at(iters).size(); ++index)
		{
			//Как только находим первый попавшийся - убираем и выходим из цикла
			if (symbolClearanceBack.find(garbage.at(iters).at(index)) <= symbolClearanceBack.size())
			{
				symbolIndexes.insert(std::map<int, int>::value_type(index, symbolClearanceBack.find(garbage.at(iters).at(index))));
				garbage.at(iters).replace(index, 1, marker);
				break;
			}
		}

		//Ищем остатки проходясь с конца в начало(поиск открывающих символов)
		for (uint32_t index = garbage.at(iters).size() - 1; index > 0; --index)
		{
			//Как только находим первый попавшийся - убираем и выходим из цикла
			if (symbolClearanceFront.find(garbage.at(iters).at(index)) <= symbolClearanceFront.size())
			{
				symbolIndexes.insert(std::map<int, int>::value_type(index, symbolClearanceFront.find(garbage.at(iters).at(index))));
				garbage.at(iters).replace(index, 1, marker);
				break;
			}
		}

		//Заменяем маркеры на html имитаторы спецсимволов
		for (std::map<int, int>::reverse_iterator it = symbolIndexes.rbegin(); it != symbolIndexes.rend(); ++it)
		{
			//Заменяем лишние символы руководствуясь содержимым карты
			switch (it->second)
			{
				case 0:
				{
					garbage.at(iters).replace(it->first, 1, replaceSymbols[it->second]);
					break;
				}
				case 1:
				{
					garbage.at(iters).replace(it->first, 1, replaceSymbols[it->second]);
					break;
				}
			}
		}

		//Чистим позицию символа, чистим карту символов перед следующей итерацией
		symbolDuplicate = 0;
		symbolIndexes.clear();
		symbol.clear();	//Очищаем символ чтобы не было ложного срабатывания в некст итерации
	}

	//Чистим буффер т.к старые данные в нём больше не нужны
	buffer.clear();

	if (firstOrder)	//Если сначала у нас полезный контент то пихаем его
	{
		for (uint32_t index = 0; index < xpression.size(); ++index)
		{
			//Добавляем сначала полезный контент(он идёт первым) а потом мусор
			buffer += xpression.at(index);
			buffer += garbage.at(index);
		}
		//Если остался дополнительный мусор, то тоже добавляем
		if (xpression.size() < garbage.size())
			buffer += garbage.at(garbage.size());
	}
	else //Иначе считаем что мусор
	{
		for (uint32_t index = 0; index < garbage.size(); ++index)
		{
			//Тут наоборот - сначала мусор а потом полезное
			buffer += garbage.at(index);
			buffer += xpression.at(index);
		}
		//Если остался дополнительный контент, то добавляем, аналогично как с мусором
		if (garbage.size() < xpression.size())
			buffer += xpression.at(xpression.size());
	}

	//buffer = buffer;

	return buffer;
}
