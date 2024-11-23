#include "crlfProcessor.h"

boost::container::string* buffer;

std::string crlfProcessor(std::string& rawInput)
{
	//Буфер для операций внутри функций
	buffer = new boost::container::string(rawInput.c_str());

	//Тег переноса строки
	boost::container::string brTag("<BR>");

	//Проходим с конца
	for (volatile uint32_t index = buffer->size() - 1; index > 0; --index)
	{
		if (buffer->empty())
			break;
		if (buffer->at(index) == '\n')
		{
			buffer->replace(index, 1, brTag);
			if (index > 2)
				index -= 2;
		}
	}

	//Возвращаем значение
	return buffer->c_str();
}
