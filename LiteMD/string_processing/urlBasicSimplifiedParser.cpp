#include "regex.h";
#include "urlBasicSimplifiedParser.h"
#include <map>

std::wstring basicUrlParser(std::wstring& rawInput)
{
	//Буффер для работы с текстом
	std::wstring buffer = rawInput;

	//Временные переменные
	std::wstring _garbage;
	std::wstring _xpressions;

	//Карты для хранения переменных мусора и выражений
	std::map<uint32_t, std::wstring> garbageMap;
	std::map<uint32_t, std::wstring> xpressionsMap;

	//индекс вхождения и длина выражения
	uint32_t prevIndex = 0;
	uint32_t index = 0;
	uint32_t range = 0;

	for (std::wsregex_iterator it = std::wsregex_iterator(buffer.cbegin(), buffer.cend(), simplifiedRegexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		prevIndex = index + range;
		index = it->position();
		range = it->length();
		_garbage = buffer.substr(prevIndex, index);
		_xpressions = buffer.substr(index, range);
	}
	index = 0;
	range = 0;
	for (std::wsregex_iterator it = std::wsregex_iterator(buffer.cbegin(), buffer.cend(), regexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		prevIndex = index + range;
		index = it->position();
		range = it->length();
		_garbage = buffer.substr(prevIndex, index);
		_xpressions = buffer.substr(index, range);
	}

	return buffer;
}
