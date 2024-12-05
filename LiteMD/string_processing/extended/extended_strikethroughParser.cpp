#include "extended_strikethroughParser.h"
#include <boost/container/string.hpp>
#include <boost/stacktrace/stacktrace.hpp>
#include <qdebug.h>
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
	#include "global_definitions.h"
}

//boost::container::string* head_lvl_url_output;
std::string* tild_lvl_url_output;

std::string extended_strikethroughParser(std::string& rawInput)
{
	boost::container::string* log_out = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	push_log(std::string("[ПАРСЕР]Размер блока " + std::to_string(*buffer_size)));

	tild_lvl_url_output = new std::string;
	//head_lvl_url_output = new boost::container::string;

	int32_t stroke_start = -1;
	int32_t stroke_end = -1;

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	tild_lvl_url_output->assign(buffer);

	//Дальше будет идти поиск по буферу так как он быстрее
	for (volatile int32_t _index = *buffer_size; _index >= 0; --_index)
	{
		//Если находим тильду в конце то ищём начало
		if (_index >= 1 && (buffer[_index] == '~') && (buffer[_index - 1] == '~'))
		{
			stroke_end = _index - 1;
			for (volatile int32_t _idx = stroke_end - 1; _idx >= 0; --_idx)
			{
				//Если находим начало то считается что сформирована строка для обработки
				if (_idx >= 1 && (buffer[_idx] == '~') && (buffer[_idx + 1] == '~'))
				{
					stroke_start = _idx;
					push_log(std::string("[ПАРСЕР]Найдена строка (" + std::to_string(stroke_start) + "-" + std::to_string(stroke_end) + ")"));
					//Приравниваем индекс к началу обработанной строки
					_index = stroke_start - 1;
					//Вкорячиваем тег с конца и затем с начала, тильды нахуй
					tild_lvl_url_output->replace(stroke_end, 2, striked_txt_iclose);
					tild_lvl_url_output->replace(stroke_start, 2, striked_txt_iopen);
					break;
				}
			}
		}
	}

	free(buffer_size);
	free(buffer);

	delete(log_out);
	
	return tild_lvl_url_output->c_str();
}
