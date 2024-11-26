#include "crlfProcessor.h"
#include "logger_backend.h"
#include <boost/container/string.hpp>

boost::container::string* buffer;

std::string crlfProcessor(std::string& rawInput)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	uint32_t crlfs = 0;

	push_log("[crlfProcessor]Поиск и замена символов переноса строки");

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
			++crlfs;
		}
	}

	//Отправляем лог
	push_log("[crlfProcessor]Завершено, обработано ");
	log_stroke->append(std::to_string(crlfs).c_str());
	log_stroke->append(" символов переноса строки");

	delete(log_stroke);
	//Возвращаем значение
	return buffer->c_str();
}
