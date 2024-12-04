#include "symbolCleaner.h"
#include "regex.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
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

struct operating_vars
{
	volatile int32_t squ_brackets_entry;
	volatile int32_t squ_brackets_last;
	volatile int32_t brackets_entry;
	volatile int32_t brackets_last;
	volatile int32_t sym_entry;
	volatile int32_t sym_last;
}opvars;

//Список тегов в тексте
service_tags* tag_list;
service_tags* new_list;

//Эту дичь пихать только в методы
#define THREAD_LOCK std::lock_guard<std::mutex> lg(m_mut);
#define NOTIFY_ALL_THREAD condition.notify_all();

std::string symbolCleaner(std::string& rawInput)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

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

	push_log("[symbolCleaner]Фильтрация дубликатов");

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
						compare_char = forward_bump.at(char_type);						//Кешируется символ который ожидается встретить со стороны начала
						opvars.brackets_last = _index;									//Текущая позиция записывается как конец тега
						for (volatile int32_t _idx = _index; _idx >= 0; --_idx)			//Ищется позиция начала
						{																//Если наход то проверяем из чего сделан кокс
							//testpoint2 = clean_buffer->at(_idx);
							//testpoint1.insert(0, 1, testpoint2);
							if ((_idx > 0) && (compare_char == clean_buffer->at(_idx)) && (clean_buffer->at(_idx - 1) == ']'))
							{
								compare_char = forward_bump.at(1);						//Снова кешируется но уже для пары '[]'
								opvars.brackets_entry = _idx;							//Кешируется начало скобок
								opvars.squ_brackets_last = _idx - 1;
								for (volatile int32_t _squ_idx = _idx; _squ_idx >= 0; --_squ_idx)
								{
									//testpoint2 = clean_buffer->at(_squ_idx);
									//testpoint1.insert(0, 1, testpoint2);
									if (compare_char == clean_buffer->at(_squ_idx))		//Пора сворачиваться
									{
										opvars.squ_brackets_entry = _squ_idx;			//Сохраняется позиция начала
										new_list = reinterpret_cast<service_tags*>(realloc(tag_list, sizeof(service_tags) * (tag_list_size + 1)));
										if (new_list != NULL)							//Проверка на карму
											tag_list = new_list;
										else
											throw(exceptionHandler(exceptionHandler::WARNING, QString("Карма в говне! - new_list вернул NULL")));
										tag_list[tag_list_size].last_entry = opvars.brackets_last;
										tag_list[tag_list_size].first_entry = opvars.brackets_entry;
										tag_list[tag_list_size].size = tag_list[tag_list_size].last_entry - tag_list[tag_list_size].first_entry;
										tag_list[tag_list_size].type = char_type;
										++tag_list_size;
										new_list = reinterpret_cast<service_tags*>(realloc(tag_list, sizeof(service_tags) * (tag_list_size + 1)));
										if (new_list != NULL)							//Проверка на карму
											tag_list = new_list;
										else
											throw(exceptionHandler(exceptionHandler::WARNING, QString("Карма в говне! - new_list вернул NULL")));
										tag_list[tag_list_size].last_entry = opvars.squ_brackets_last;
										tag_list[tag_list_size].first_entry = opvars.squ_brackets_entry;
										tag_list[tag_list_size].size = tag_list[tag_list_size].last_entry - tag_list[tag_list_size].first_entry;
										++tag_list_size;								//Писать ещё раз char_type нет смысла
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
										//tag_list[tag_list_size].last_entry = _search + 1;
										opvars.brackets_last = _search + 1;
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
			//Поиск непарных служебных символов
			char_type = symbolCollection.find(clean_buffer->at(_index));
			if (char_type != -1)
			{
				compare_char = clean_buffer->at(_index);
				switch (char_type)
				{
					//Ищём ~
					case 0:
					{
						//--_index;	//По правилам зачёркнутого тега тильды должно быть две и ледующая тоже должна быть тильда
						if ((_index >= 1) && (clean_buffer->at(_index) == compare_char) && (clean_buffer->at(_index - 1) == compare_char))	
						{	//Если удалось найти тильды то ищём следующие
							opvars.sym_last = _index - 1;	//Смещение, так как тильды две
							for (volatile int32_t _idx = opvars.sym_last; _idx >= 0; --_idx)
							{	//По такому же сценарию
								if (_idx > 0 && clean_buffer->at(_idx) == compare_char && clean_buffer->at(_idx - 1) == compare_char)
								{
									opvars.sym_entry = _idx;
									//Если всё успешно то запоминаем
									new_list = reinterpret_cast<service_tags*>(realloc(tag_list, sizeof(service_tags) * (tag_list_size + 1)));
									if (new_list != NULL)							//Проверка на карму
										tag_list = new_list;
									else
										throw(exceptionHandler(exceptionHandler::WARNING, QString("Карма в говне! - new_list вернул NULL")));
									tag_list[tag_list_size].first_entry = opvars.sym_entry - 1;
									tag_list[tag_list_size].last_entry = opvars.sym_last + 1;
									tag_list[tag_list_size].size = opvars.sym_last - opvars.sym_entry;
									tag_list[tag_list_size].type = 4 + char_type;	//+4 потому что первые 4 позиции - пары скобок
									++tag_list_size;
									break;
								}
							}
							break;
						}
						else
							break;
					}
				}
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в symbolCleaner.cpp -> 88:217")));
	}
	
	push_log("[symbolCleaner]Обнаружены следующие теги");
	//testpoint1.clear();
	//std::vector<std::string> testpoint_str;
	for (uint8_t _idx = 0; _idx < tag_list_size; ++_idx)
	{
		log_stroke->append("[symbolCleaner]Тег (");
		log_stroke->append(std::to_string(tag_list[_idx].first_entry).c_str());
		log_stroke->append("-");
		log_stroke->append(std::to_string(tag_list[_idx].last_entry).c_str());
		log_stroke->append(") ");
		//testpoint3 = tag_list[_idx].first_entry;
		//testpoint3 = tag_list[_idx].last_entry;
		//testpoint3 = tag_list[_idx].size;
		for (uint16_t a = tag_list[_idx].first_entry; a <= tag_list[_idx].last_entry; ++a)
		{
			//testpoint2 = clean_buffer->at(a);
			log_stroke->append(1, clean_buffer->at(a));
			//testpoint1.append(1, testpoint2);
		}
		push_log(log_stroke->c_str());
		log_stroke->clear();
		//testpoint_str.push_back(testpoint1);
		//testpoint1.clear();
	}

	uint32_t cleans = 0;

	push_log("[symbolCleaner]Проход и очистка дубликатов и неправильных тегов");

	try
	{
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
						++cleans;
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
							++cleans;
							break;
						}
					}
					//Отсюда начинаются уже непарные теги
					case 4:
					{	//Чистятся символы внутри тега чтобы исключить ложное срабатывание
						for (volatile int32_t _idx = tag_list[rb_cache_ptr].last_entry - 2; _idx >= tag_list[rb_cache_ptr].first_entry + 2; --_idx)
						{
							if (bracketsSrc.find(clean_buffer->at(_idx)) != -1)
								clean_buffer->replace(_idx, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_idx))).c_str());
						}
						_index = tag_list[rb_cache_ptr].first_entry - 1;
						if (rb_cache_ptr < tag_list_size)
							++rb_cache_ptr;
						++cleans;
						break;
					}
				}
			}
			//Чистка служебных символов, находящихся между скобочками
			if (_index >= 0 && bracketsSrc.find(clean_buffer->at(_index)) != -1)
				clean_buffer->replace(_index, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_index))).c_str());
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью при фильтрации служебных символов(symbolCleaner.cpp 245:297)")));
	}

	log_stroke->append("[symbolCleaner]Очистка завершена, очищено ");
	log_stroke->append(std::to_string(cleans).c_str());
	log_stroke->append(" символов");
	push_log(log_stroke->c_str());

	//testpoint1 = clean_buffer->c_str();

	//Чистка памяти
	free(tag_list);
	free(buffer_size);
	delete(log_stroke);

	return clean_buffer->c_str();
}
