#include "italicParser.h"
#include <boost/container/string.hpp>
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
	#include "global_definitions.h"
}

//boost::container::string* head_lvl_url_output;
std::string* italic_output;

std::string italicParser(std::string& rawInput)
{
	//Вот отсюда --->
	boost::container::string* log_out = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	push_log(std::string("[ПАРСЕР]Размер блока " + std::to_string(*buffer_size)));

	italic_output = new std::string;
	//italic_output = new boost::container::string;

	int32_t stroke_start = -1;
	int32_t stroke_end = -1;

	uint32_t italics = 0;
	//Счётчик найдёнышей

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	italic_output->assign(buffer);

	//---> и до сюда тупо копипиздинг из парсера в парсер, ничего практически менять не нужно

	//Здесь попробую новую технику поиска признаков
	for (volatile int32_t _index = *buffer_size - 1; _index >= 0; --_index)
	{
		//Так как ищем курсив который состоит из '*' в начале и конце
		//то найти нужно каждой твари по одной шт
		if (buffer[_index] == '*')
		{
			//Если нашли с конца, теперь надо искать в начале НО...

			//Проверяем для начала, по адресу ли мы попали
			if ((_index > 2 )&& (buffer[_index] == '*' && buffer[_index - 1] == '*' && buffer[_index - 2] != '*'))
			{
				//Если звездочки две, значит это другой тег
				_index -= 2;
			}
			else
			{
				for (volatile int32_t _srch = _index; _srch >= 0; --_srch)
				{
					//...из тех чиркашей состоит и признак тега жирного текста, а их трогать не надо
					_index = _srch;
					if (buffer[_srch] != '*')
						break;
				}

				//Запоминаем позицию конца
				stroke_end = _index + 1;

				//Если юзер на рофлянчиках просто тыкнул '*' в начале то ничего не делаем дальше
				//делаем вид что мы тут мебель
				if (_index != 0)
				{
					//Теперь можно искать начало, и по той же дорожке дальше - с доводкой
					for (volatile int32_t _idx = stroke_end - 1; _idx >= 0; --_idx)
					{
						//Если нашли начало, то теперь такая же тема
						if (buffer[_idx] == '*')
						{
							for (volatile int32_t _srch = _idx; _srch >= 0; --_srch)
							{
								//...из тех чиркашей состоит и признак тега жирного текста, а их трогать не надо
								_idx = _srch;
								if (buffer[_srch] != '*')
									break;
							}

							//Небольшая поправОЧКА - если курсив начинается в начале то смещение не делаем
							_idx == 0 ? stroke_start = _idx : stroke_start = _idx + 1;

							//Плюсуем счётчик найденных бомжей
							++italics;

							//Кидаем лог
							push_log(std::string("[italicParser]Обработка пары *...* (" + std::to_string(stroke_start) + "-" + std::to_string(stroke_end) + ")"));

							//Теперь можно вкорячивать теги
							italic_output->replace(stroke_end, 1, italic_txt_iclose);
							italic_output->replace(stroke_start, 1, italic_txt_iopen);

							_index = stroke_start;

							//Ногами вперёд вылетаем из цикла пока ещё чего не навертели
							break;
						}
					}
				}
			}
		}
		//После всей движухи сбрасываем указатели
		stroke_start = -1;
		stroke_end = -1;
	}

	push_log(std::string("[italicParser]Обработано пар *...*: " + std::to_string(italics)));

	free(buffer);
	free(buffer_size);

	return italic_output->c_str();
}
