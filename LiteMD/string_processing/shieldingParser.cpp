#include "shieldingParser.h"

std::wstring shieldingParser(std::wstring& rawInput)
{
	//Создаем буффер в котором будем проводить обработку
	std::wstring buffer = rawInput;

	//Символ которым будет проводиться замена
	std::wstring replaceSymbol;

	//Символ-детектор экранирования
	std::wstring shieldDetector(L"\\");

	//Ищем спецсимволы, которые отмечены знаком экранирования
	for (uint32_t index = buffer.size() - 1; index > 0; --index)
	{
		if (buffer.empty())
			break;
		//Если находим то читаем символ в буфер, находим такой же в контейнере
		//затем превращаем спецсимвол в дрисню и проливаем обратно в буфер
		if (buffer.at(index) == L'\\')
		{
			replaceSymbol = buffer.at(index + 1);
			replaceSymbol = shieldingSymbols.at(shieldingSymbolsSrc.find(replaceSymbol));
			buffer.replace(index, 2, replaceSymbol);
		}
	}

	//Возвращаем буффер с дриснёй
	return buffer;
}
