#include "crlfProcessor.h"
#include "logger_backend.h"
#include <boost/container/string.hpp>

boost::container::string* buffer;
boost::container::string* str_piece;

std::string testpoint1;

std::string crlfProcessor(std::string& rawInput)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	uint32_t crlfs = 0;
	uint32_t findPos = 0;

	push_log("[crlfProcessor]Поиск и замена символов переноса строки");

	//Буфер для операций внутри функций
	buffer = new boost::container::string(rawInput.c_str());

	//Буфер для кусочков текста для поиска тега <H
	str_piece = new boost::container::string;

	//Тег переноса строки
	boost::container::string brTag("<BR>");

	//Проходим с конца
	for (volatile uint32_t index = buffer->size() - 1; index > 0; --index)
	{
		if (buffer->empty())
			break;
		if (buffer->at(index) == '\n')
		{
			//Копируем кусочек текста для анализа только в случае найденого символа переноса
			if (buffer->size() > 10)
				str_piece->assign(&rawInput.c_str()[index - 10], 10);
			else
				str_piece->assign(&rawInput.c_str()[0], 10);
			//Если в кусочке есть нужный тег то обрабатываем
			if (str_piece->find("</H") != -1)
			{
				//Кидаем в лог
				log_stroke->append("[crlfProcessor]Обнаружен признак заголовка на индексе ");
				log_stroke->append(std::to_string(index).c_str());
				log_stroke->append(", замена не проводится");
				push_log(log_stroke->c_str());
				log_stroke->clear();
				findPos = index - 10 + str_piece->find("</H") ;
				testpoint1 = buffer->c_str();
				buffer->erase(findPos + 5, 2);
				testpoint1 = buffer->c_str();
				//Теперь проход до начала строки
				for (volatile int32_t _index = findPos; _index >= 0; --_index)
				{
					//Как только наткнулись на начало строки - чистим знаки переноса
					if ((_index >= 1) && (buffer->at(_index) == '\n'))
					{
						//Кидаем в лог
						log_stroke->append("[crlfProcessor]Удалены знаки переноса на индексах ");
						log_stroke->append(std::to_string(index).c_str());
						log_stroke->append(" и ");
						buffer->erase(_index - 1, 2);
						index = _index;
						log_stroke->append(std::to_string(_index).c_str());
						push_log(log_stroke->c_str());
						log_stroke->clear();
						break;
					}
					//Если ничего не нашли то индекс ровняем с текущей позицией
					if (_index == 0)
						index = _index + 1; log_stroke->clear();
				}
			}
			else
			{
				buffer->replace(index, 1, brTag);
				if (index > 2)
					index -= 2;
				++crlfs;
			}

		}
	}

	//Отправляем лог
	log_stroke->append("[crlfProcessor]Завершено, обработано ");
	log_stroke->append(std::to_string(crlfs).c_str());
	log_stroke->append(" символов переноса строки");
	push_log(log_stroke->c_str());

	delete(log_stroke);
	//Возвращаем значение
	return buffer->c_str();
}
