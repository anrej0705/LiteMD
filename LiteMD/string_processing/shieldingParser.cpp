#include "shieldingParser.h"

boost::container::string* shielding_buffer;
boost::container::string* replaceSymbol;

std::string shieldingParser(std::string& rawInput)
{
	//Создаем буффер в котором будем проводить обработку
	shielding_buffer = new boost::container::string(rawInput.c_str());

	//Символ которым будет проводиться замена
	replaceSymbol = new boost::container::string;

	//Ищем спецсимволы, которые отмечены знаком экранирования
	for (uint32_t index = shielding_buffer->size() - 1; index > 0; --index)
	{
		if (shielding_buffer->empty())
			break;
		//Если находим то читаем символ в буфер, находим такой же в контейнере
		//затем превращаем спецсимвол в дрисню и проливаем обратно в буфер
		if (shielding_buffer->at(index) == '\\')
		{
			replaceSymbol[0] = shielding_buffer->at(index + 1);
			if (shieldingSymbolsSrc.find(replaceSymbol[0]) != -1)
			{
				replaceSymbol[0] = shieldingSymbols.at(shieldingSymbolsSrc.find(replaceSymbol[0])).c_str();
				shielding_buffer->replace(index, 2, replaceSymbol[0]);
			}
		}
	}

	//Возвращаем буффер с дриснёй
	return shielding_buffer->c_str();
}
