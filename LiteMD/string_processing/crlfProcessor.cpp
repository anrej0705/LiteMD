#include "crlfProcessor.h"
#include "logger_backend.h"
#include "regex.h"
#include <boost/container/string.hpp>

boost::container::string* buffer;
boost::container::string* str_piece;

//std::string testpoint1;
//char testpoint2;

std::string crlfProcessor(std::string& rawInput)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	uint32_t crlfs = 0;
	uint32_t findPos = 0;

	push_log("[crlfProcessor]Поиск и замена символов переноса строки");

	//Буфер для операций внутри функций
	buffer = new boost::container::string(rawInput.c_str());

	//testpoint1 = buffer->c_str();

	//Буфер для кусочков текста для поиска тега <H
	str_piece = new boost::container::string;

	//Тег переноса строки
	boost::container::string brTag("<BR>");

	//Счётчик строк для дебага
	uint32_t strokes = 0;

	//Проходим с конца
	for (volatile uint32_t index = buffer->size() - 1; index > 0; --index)
	{
		//testpoint1 = buffer->c_str();
		if (buffer->empty())
			break;
		//testpoint2 = buffer->at(index);
		if (index > 0 && buffer->at(index - 1) == '\n' && (shieldingSymbolsSrc.find(buffer->at(index)) != -1))
		{
			//++strokes;
			//push_log(std::string("[crlfProcessor]Обнаружен служебный символ в начале строки " + std::to_string(strokes)));
			switch (buffer->at(index))
			{
				case '!':
				{
					++strokes;
					push_log(std::string("[crlfProcessor]Обнаружен признак изображения в начале строки"));
					break;
				}
				case '*':
				{
					++strokes;
					push_log(std::string("[crlfProcessor]Обнаружен признак тега форматирования в начале строки"));
					break;
				}
				case '<':
				{
					++strokes;
					push_log(std::string("[crlfProcessor]Обнаружен признак ссылки в начале строки"));
					break;
				}
				case '[':
				{
					++strokes;
					push_log(std::string("[crlfProcessor]Обнаружен признак ссылки в начале строки"));
					break;
				}
				case '-':
				{
					++strokes;
					push_log(std::string("[crlfProcessor]Обнаружен признак списка в начале строки"));
					buffer->replace(index - 1, 1, brTag);
					break;
				}
				default:
				{
					++strokes;
					push_log(std::string("[crlfProcessor]Обнаружен служебный символ в начале строки " + std::to_string(strokes)));
					buffer->replace(index - 1, 1, brTag);
					index -= 1;
					break;
				}
			}
		}
		else if (index > 0 && buffer->at(index) == '\n' && buffer->at(index - 1) == '\n')
		{
			++strokes;
			//testpoint1 = buffer->c_str();
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
				//testpoint1 = buffer->c_str();
				log_stroke->clear();
				findPos = index - 10 + str_piece->find("</H") ;
				//testpoint1 = buffer->c_str();
				buffer->erase(findPos + 5, 1);
				//testpoint1 = buffer->c_str();
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
						buffer->erase(_index, 1);
						//testpoint1 = buffer->c_str();
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
				//testpoint1 = buffer->c_str();
				if (index > 1)
					index -= 1;
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
