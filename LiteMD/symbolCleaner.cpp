#include "symbolCleaner.h"
#include "regex.h"
#include <map>

std::wstring symbolCleaner(std::wstring& rawInput)
{
	//Создаем буффер, в котором будем чистить спецсимволы
	std::wstring buffer = rawInput;

	//Контейнер спецсимвола, сюда помещается первый попавшийся спецсимвол
	std::wstring symbol;

	//Указатель на индекс символа из библиотеки замены
	uint8_t replaceIndex = 0;

	//Маркер, по которому потом будет заменяться пустые символы
	std::wstring marker(L"\u009C");

	//Хранит значение предыдущего спецсимвола
	uint32_t symbolDuplicate = 0;

	//Хранит индексы заменяемых спецсимволов
	std::map<int, int> symbolIndexes;

	//Очистка от повторов спецсимволов
	for (uint32_t index = 0; index < buffer.size(); ++index)
	{
		//Если встречаем повтор символа то удаляем с предыдущей позиции
		if(!symbol.empty())
			if (buffer.at(index) == symbol.at(0))
			{
				symbolIndexes.insert(std::map<int, int>::value_type(symbolDuplicate,replaceIndex));
				buffer.replace(symbolDuplicate, 1, marker);
				symbolDuplicate = index;
			}

		//Если новый встречается новый символ то считаем что очистка до него завершена
		//и поэтому чистим повторы нового символа
		if (symbolCollection.find(buffer.at(index)) <= symbolCollection.size() && !symbol.empty())
			if (buffer.at(index) != symbol.at(0))
				symbol.clear();

		//Если спецсимвол первый раз попался то запоминаем его
		if (symbolCollection.find(buffer.at(index)) <= symbolCollection.size() && symbol.empty())
		{
			replaceIndex = symbolCollection.find(buffer.at(index));
			symbol = symbolCollection.at(replaceIndex);
			symbolDuplicate = index;
		}
	}

	//Очистка от закрывающих спецсимволов
	for (uint32_t index = 0; index < buffer.size(); ++index)
	{
		//Как только находим первый попавшийся - убираем и выходим из цикла
		if (symbolClearance.find(buffer.at(index)) <= symbolClearance.size())
		{
			symbolIndexes.insert(std::map<int, int>::value_type(index, symbolClearance.find(buffer.at(index))));
			buffer.replace(index, 1, marker);
			break;
		}
	}

	//Заменяем лишние и повторяющиеся спецсимволы
	for (std::map<int, int>::reverse_iterator it = symbolIndexes.rbegin(); it != symbolIndexes.rend(); ++it)
	{
		//Заменяем лишние символы руководствуясь содержимым карты
		switch (it->second)
		{
			case 0:
			{
				buffer.replace(it->first, 1, replaceSymbols[it->second]);
				break;
			}
			case 1:
			{
				buffer.replace(it->first, 1, replaceSymbols[it->second]);
				break;
			}
		}
	}

	return buffer;
}
