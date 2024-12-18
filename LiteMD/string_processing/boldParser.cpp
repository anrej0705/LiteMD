#include "boldParser.h"
#include <boost/container/string.hpp>
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
	#include "global_definitions.h"
}

//boost::container::string* head_lvl_url_output;
std::string* bold_output;

std::string boldParser(std::string& rawInput)
{
	//Вот отсюда --->
	boost::container::string* log_out = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	push_log(std::string("[ПАРСЕР]Размер блока " + std::to_string(*buffer_size)));

	bold_output = new std::string;
	//italic_output = new boost::container::string;

	int32_t stroke_start = -1;
	int32_t stroke_end = -1;

	uint32_t bolds = 0;
	//Счётчик найдёнышей

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	bold_output->assign(buffer);

	//---> и до сюда тупо копипиздинг из парсера в парсер, ничего практически менять не нужно

	//Как и в курсиве - тест новой методики
	for (volatile int32_t _index = *buffer_size - 1; _index > 0; --_index)
	{
		//Так как ищем жирный текст который состоит из '**' в начале и конце
		//то найти нужно каждой твари по одной паре
		if (buffer[_index] == '*' && buffer[_index - 1] == '*')
		{
			//Здесь уточнений не нужно в отличии от курсивного парсера
			stroke_end = _index - 1;

			//Теперь поиск начала
			for (volatile int32_t _idx = stroke_end; _idx > 0; --_idx)
			{
				if (buffer[_idx] == '*' && buffer[_idx - 1] == '*')
				{
					//Запоминаем начало строки
					stroke_start = _idx - 1;

					//Плюсуем счётчик найденных бомжей
					++bolds;

					//Кидаем лог
					push_log(std::string("[italicParser]Обработка пары **...** (" + std::to_string(stroke_start) + "-" + std::to_string(stroke_end) + ")"));

					//Подменяем звёздочки на теги
					bold_output->replace(stroke_end, 2, bold_txt_iclose);
					bold_output->replace(stroke_start, 2, bold_txt_iopen);

					//Перемещаемся на начало строчки которую обрабатывали
					_index = stroke_start;

					//Ногами вперёд вылетаем из цикла пока ещё чего не навертели
					break;
				}
			}
		}
		//После всей движухи сбрасываем указатели
		stroke_start = -1;
		stroke_end = -1;
	}

	push_log(std::string("[italicParser]Обработано пар **...**: " + std::to_string(bolds)));
	//После работы нужно убраться за собой
	free(buffer);
	free(buffer_size);

	return bold_output->c_str();
}
