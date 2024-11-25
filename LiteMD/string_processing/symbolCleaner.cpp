#include "symbolCleaner.h"
#include "regex.h"
#include "exceptionHandler.h"
#include <boost/thread/thread.hpp>
#include <boost/container/string.hpp>
#include <boost/container/vector.hpp>
#include <mutex>
#include <map>

//SYMPBOLCLEANER.CPP чистит дублирующие символы от повторов чтобы парсинг проходил без ошибок
/*************************************************************************************************************************
                                                                                                         ЮЗЕР            *
     ПАРСЕР   __                SYMBOLCLEANER.CPP                     buffer->replace '.\|/.'          .----.__          *
             /()\````\          ░░░░░░███████ ]▄▄▄▄▄▄▄▄              '.\|/.'          (\   /)         /---.__  \         *
            /____\____\         ▂▄▅█████████▅▄▃▂                  (\   /)          - -O- -       /       `\ |        *
            |n  n|.___|        [████###############                  - -O- -          (/   \)       | o     o  \|        *
    ..      | __ /_\___\        ◥⊙▲░░░░░░███████ ]▄▄▄▄▄▄▄▄          (/   \)|/.'  for ,'/|\'.     ./| .vvvvv.   |\       *
   ___|  `'::::. |n|n_n|            ▂▄▅█████████▅▄▃▂              '.\(/   \)             .    / /| |     |   | \      *
  /___/\  _____A_                  [█████##############                 -- -O- -                / /'| `^vvvv'   |/\\     *
  |' '|| /      /\                  ◥⊙▲ ░░░░░███████ ]▄▄▄▄▄▄▄▄          (/   \)                ~   \ ЕБЛАНСТВО |  \\    *
  `"""__/__/\__/  \__                   ▂▄▅█████████▅▄▃▂             '.\)|/.'   switch           |   ЮЗЕРА   |.       *
  ---/__|" '' "| /___/\---- 	       [███████████████████].                                    _л7|   ю  /     \х      *
     |''|"'||'"| |' '||                 ◥⊙▲⊙▲⊙▲⊙▲⊙▲⊙▲⊙◤..                     if           .1_ /  .   . у  | /юх    *
     `""`""))""`"`""""`                                                                     dькккк|_/\/ `--.| _ \юллл    *
                                                                                                                         *
**************************************************************************************************************************/

std::condition_variable condition;
std::mutex m_mut;

boost::container::string* clean_buffer;
boost::container::string* clean;

//Хранилище информации о теге
struct service_tags
{
	int32_t first_entry;	//Позиция открывающей скобки
	int32_t last_entry;		//Позиция закрывающей скобки
	uint32_t size;			//Размер(поз.закр - поз.откр)
	uint8_t type;			//Тип
};
/*
* Типы
* 0 - '}'
* 1 - ']'
* 2 - '>'
* 3 - ')'
*/

//Список тегов в тексте
service_tags* tag_list;
service_tags* new_list;

//Эту дичь пихать только в методы
#define THREAD_LOCK std::lock_guard<std::mutex> lg(m_mut);
#define NOTIFY_ALL_THREAD condition.notify_all();

std::string symbolCleaner(std::string& rawInput)
{
	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	int32_t* reverse_bump_cache;	//Кеш для закрывающих символов
	int32_t* new_bc;				//Временная переменная для фокусов
	reverse_bump_cache = (int32_t*)calloc(1, sizeof(int32_t));

	clean_buffer = new boost::container::string;
	clean = new boost::container::string;
	
	//std::string testpoint1;
	//char testpoint2;
	//int testpoint3;

	clean_buffer->assign(rawInput.c_str());

	//testpoint1 = clean_buffer->c_str();

	uint32_t tag_list_size = 0;
	uint32_t replace_offset = 0;
	uint16_t rb_cache_ptr = 0;
	int8_t char_type;
	/*register */ char compare_char;

	tag_list = (service_tags*)calloc(tag_list_size + 1, sizeof(service_tags));

	//Проход с конца. Ищётся закрывающая скобка, запоминается, ищется открывающая
	//в интервале ищутся повторы обоих символов, после завершения ищется следующая пара
	try
	{
		for (volatile int32_t _index = *buffer_size - 1; _index >= 0; --_index)
		{
			//Если при обратном поиске встречается что-то из этого:
			// '}'
			// ']'
			// ')'
			// '>'
			//То вписываем в структуру "досье" на тег
			char_type = reverse_bump.find(clean_buffer->at(_index));
			if (char_type != -1)
			{
				compare_char = clean_buffer->at(_index);
				switch (char_type)
				{
					case 2:
					{
						//Готовится участок памяти
						new_list = reinterpret_cast<service_tags*>(realloc(tag_list, sizeof(service_tags) * (tag_list_size + 1)));
						if (new_list != NULL)											//Проверка на карму
							tag_list = new_list;
						else
							throw(exceptionHandler(exceptionHandler::WARNING, QString("Карма в говне! - new_list вернул NULL")));
						tag_list[tag_list_size].type = char_type;						//Записывается тип найденого символа
						tag_list[tag_list_size].first_entry = -1;						//Позиция начала обозначается как неизвестная
						compare_char = forward_bump.at(char_type);				//Кешируется символ который ожидается встретить со стороны начала
						tag_list[tag_list_size].last_entry = _index;			//Текущая позиция записывается как конец тега
						for (volatile int32_t _idx = _index; _idx >= 0; --_idx)	//Ищется позиция начала
						{
							//testpoint2 = clean_buffer->at(_idx);
							//testpoint1.insert(0, 1, testpoint2);
							if (compare_char == clean_buffer->at(_idx))			//Если наход то ловим приход
							{
								tag_list[tag_list_size].first_entry = _idx;
								_index = _idx;									//Перемещаем указатель чтоб не было ложных срабатываний
								tag_list[tag_list_size].size = tag_list[tag_list_size].last_entry - tag_list[tag_list_size].first_entry;
								//testpoint1.clear();
								break;											//Чики брики цiкл выкинь
							}
							switch (bracketsSrc.find(clean_buffer->at(_idx)))
							{
								case -1:
								{
									break;
								}
								case 5:
								{
									tag_list[tag_list_size].last_entry = _idx;
									break;
								}
								default:
								{
									_idx = 0;
									break;
								}
							}
						}
						if (tag_list[tag_list_size].first_entry != -1)
							++tag_list_size;
						/*else if (tag_list[tag_list_size].first_entry == -1)
							--tag_list_size;*/
						break;
					}
					case 3:
					{
						//testpoint1.clear();
						//Готовится участок памяти
						new_list = reinterpret_cast<service_tags*>(realloc(tag_list, sizeof(service_tags) * (tag_list_size + 1)));
						if (new_list != NULL)											//Проверка на карму
							tag_list = new_list;
						else
							throw(exceptionHandler(exceptionHandler::WARNING, QString("Карма в говне! - new_list вернул NULL")));
						tag_list[tag_list_size].type = char_type;						//Записывается тип найденого символа
						tag_list[tag_list_size].first_entry = -1;						//Позиция начала обозначается как неизвестная
						compare_char = forward_bump.at(char_type);						//Кешируется символ который ожидается встретить со стороны начала
						tag_list[tag_list_size].last_entry = _index;					//Текущая позиция записывается как конец тега
						for (volatile int32_t _idx = _index; _idx >= 0; --_idx)			//Ищется позиция начала
						{																//Если наход то проверяем из чего сделан кокс
							//testpoint2 = clean_buffer->at(_idx);
							//testpoint1.insert(0, 1, testpoint2);
							if ((compare_char == clean_buffer->at(_idx)) && (clean_buffer->at(_idx - 1) == ']'))
							{
								compare_char = forward_bump.at(1);						//Снова кешируется но уже для пары '[]'
								tag_list[tag_list_size].first_entry = _idx;				//Кешируется
								tag_list[tag_list_size].size = tag_list[tag_list_size].last_entry - tag_list[tag_list_size].first_entry;
								++tag_list_size;
								//Готовится участок памяти
								new_list = reinterpret_cast<service_tags*>(realloc(tag_list, sizeof(service_tags) * (tag_list_size + 1)));
								if (new_list != NULL)									//Проверка на карму
									tag_list = new_list;
								else
									throw(exceptionHandler(exceptionHandler::WARNING, QString("Карма в говне! - new_list вернул NULL")));
								tag_list[tag_list_size].last_entry = _idx - 1;
								for (volatile int32_t _squ_idx = _idx; _squ_idx >= 0; --_squ_idx)
								{
									//testpoint2 = clean_buffer->at(_squ_idx);
									//testpoint1.insert(0, 1, testpoint2);
									if (compare_char == clean_buffer->at(_squ_idx))		//Пора сворачиваться
									{
										tag_list[tag_list_size].first_entry = _squ_idx;	//Сохраняется позиция начала
										tag_list[tag_list_size].size = tag_list[tag_list_size].last_entry - tag_list[tag_list_size].first_entry;
										++tag_list_size;
										_index = _squ_idx;
										//testpoint1.clear();
										break;
									}
								}
								break;
							}
							else if (clean_buffer->at(_idx) == '(')
							{
								for (volatile int32_t _search = _index; _index >= 0; --_index)
								{
									if (clean_buffer->at(_search) == ')')
									{
										tag_list[tag_list_size].last_entry = _search + 1;
										//testpoint1.clear();
										break;
									}
								}
								break;
							}
						}
						break;
					}
				}
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::WARNING, QString("Карма в говне! - Ошибка работы с памятью в symbolCleaner.cpp -> 88:217")));
	}
	
	/*testpoint1.clear();
	std::vector<std::string> testpoint_str;
	for (uint8_t _idx = 0; _idx < tag_list_size; ++_idx)
	{
		testpoint3 = tag_list[_idx].first_entry;
		testpoint3 = tag_list[_idx].last_entry;
		testpoint3 = tag_list[_idx].size;
		for (uint16_t a = tag_list[_idx].first_entry; a < tag_list[_idx].last_entry; ++a)
		{
			testpoint2 = clean_buffer->at(a);
			testpoint1.append(1, testpoint2);
		}
		testpoint_str.push_back(testpoint1);
		testpoint1.clear();
	}*/

	//Чистка мусорных символов. Пары символов, создающие цельные теги не будут затронуты
	for (volatile int32_t _index = *buffer_size - 1; _index >= 0; --_index)
	{
		if (_index == tag_list[rb_cache_ptr].last_entry)	//Поиск закрывающих символов
		{
			switch (tag_list[rb_cache_ptr].type)			//В зависимости от типа тегов, выбирается метод чистки
			{
				case 2:
				{	//Чистятся символы внутри тега чтобы исключить ложное срабатывание
					for (volatile int32_t _idx = tag_list[rb_cache_ptr].last_entry - 1; _idx >= tag_list[rb_cache_ptr].first_entry + 1; --_idx)
					{
						if (bracketsSrc.find(clean_buffer->at(_idx)) != -1)
							clean_buffer->replace(_idx, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_idx))).c_str());
					}
					_index = tag_list[rb_cache_ptr].first_entry - 1;
					if (rb_cache_ptr < tag_list_size)
						++rb_cache_ptr;
					break;
				}
				case 3:
				{	//Чистятся символы внутри тега чтобы исключить ложное срабатывание
					if (tag_list_size > 0)
					{
						//Обработка круглых скобок
						for (volatile int32_t _idx = tag_list[rb_cache_ptr].last_entry - 1; _idx >= tag_list[rb_cache_ptr].first_entry + 1; --_idx)
						{
							if (bracketsSrc.find(clean_buffer->at(_idx)) != -1)
								clean_buffer->replace(_idx, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_idx))).c_str());
						}
						_index = tag_list[rb_cache_ptr].first_entry;
						++rb_cache_ptr;

						//Обработка квадратных скобок
						for (volatile int32_t _idx = tag_list[rb_cache_ptr].last_entry - 1; _idx >= tag_list[rb_cache_ptr].first_entry + 1; --_idx)
						{
							if (bracketsSrc.find(clean_buffer->at(_idx)) != -1)
								clean_buffer->replace(_idx, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_idx))).c_str());
						}
						_index = tag_list[rb_cache_ptr].first_entry - 1;
						if (rb_cache_ptr < tag_list_size)
							++rb_cache_ptr;
						break;
					}
				}
			}
		}
		//Чистка служебных символов, находящихся между скобочками
		if (_index >= 0 && bracketsSrc.find(clean_buffer->at(_index)) != -1)
			clean_buffer->replace(_index, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_index))).c_str());
	}

	//testpoint1 = clean_buffer->c_str();

	//Чистка памяти
	free(tag_list);
	free(buffer_size);

	return clean_buffer->c_str();
}
