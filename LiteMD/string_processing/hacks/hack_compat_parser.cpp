#include "hack_compat_parser.h"
#include <boost/container/string.hpp>
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
	#include "global_definitions.h"
}

//boost::container::string* head_lvl_url_output;
std::string* compat_output;

std::string compatParser(std::string rawInput)
{
	boost::container::string* log_out = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	push_log(std::string("[ПАРСЕР]Размер блока " + std::to_string(*buffer_size)));

	compat_output = new std::string;
	//head_lvl_url_output = new boost::container::string;

	int32_t stroke_start = -1;
	int32_t stroke_end = -1;

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	compat_output->assign(buffer);

	bool valid = 0;

	uint32_t replaces = 0;

	push_log("[compatParser]Проводится замена тега <ins> на тег <u> для совместимости с рендером Qt");

	for (volatile int32_t _index = *buffer_size; _index >= 0; --_index)
	{
		if (buffer[_index] == '>')
		{	//Если обнаруживается закрывающая треугольная скобка, то проверяем чтобы дальше было что-то типа "ins>"
			stroke_end = _index - 1;
			for (volatile int32_t _check = _index; _check > _index - 4; --_check)
			{
				if (buffer[_check] == underlined_txt_iopen[_check - _index + underlined_txt_iopen_size - 1])
				{	//Если проверка пройдена 4 раза - по разу на знак, то значит тег правильный, наверное, и можно заменять
					valid = 1;
					stroke_start = _check;
				}
				else
					valid = 0;
			}
			if (valid)
			{	//Заменяем ins на u, в итоге тег превращается в пару <u>...</u>
				compat_output->replace(stroke_start, 3, "u");
				++replaces;
				valid = 0;
			}
		}
	}

	push_log(std::string("[compatParser]Проведено " + std::to_string(replaces) + " замен"));

	return compat_output->c_str();
}
