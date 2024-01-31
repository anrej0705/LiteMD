#include "regex.h";
#include "urlBasicParser.h"
#include <map>

std::wstring basicUrlParser(std::wstring& rawInput)
{
	//Буффер для работы с текстом
	std::wstring buffer = rawInput;

	//Временные переменные
	std::wstring _garbage;
	std::wstring _xpressions;

	//Карты для хранения переменных мусора и выражений
	std::map<int, std::wstring> garbageMap;
	std::map<int, std::wstring> xpressionsMap;

	//индекс вхождения и длина выражения
	int index = 0;
	int range = 0;

	for (std::wsregex_iterator it = std::wsregex_iterator(buffer.cbegin(), buffer.cend(), regexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		range = it->position();
	}

	return buffer;
}
