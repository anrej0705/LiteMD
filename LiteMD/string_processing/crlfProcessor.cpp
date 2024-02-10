#include "crlfProcessor.h"

std::wstring crlfProcessor(std::wstring& rawInput)
{
	//Буфер для операций внутри функций
	std::wstring buffer = rawInput;

	//Тег переноса строки
	std::wstring brTag(L"<BR>");

	//Проходим с конца
	for (uint32_t index = buffer.size() - 1; index > 0; --index)
	{
		if (buffer.at(index) == L'\n')
		{
			buffer.replace(index, 1, brTag);
		}
	}

	//Возвращаем значение
	return buffer;
}
