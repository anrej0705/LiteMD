#include "symbolCleaner.h"
#include "regex.h"
#include "exceptionHandler.h"
#include <boost/thread/thread.hpp>
#include <boost/container/string.hpp>
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

//Эту дичь пихать только в методы
#define THREAD_LOCK std::lock_guard<std::mutex> lg(m_mut);
#define NOTIFY_ALL_THREAD condition.notify_all();

std::string symbolCleaner(std::string& rawInput)
{
	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	char* buffer = (char*)malloc(*buffer_size);
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	clean_buffer = new boost::container::string;
	clean = new boost::container::string;
	
	std::string testpoint1;
	char testpoint2;
	int testpoint3;

	clean_buffer->assign(buffer);

	testpoint1 = clean_buffer->c_str();

	volatile int32_t last_entry = -1;
	volatile int32_t first_entry = -1;
	volatile int32_t _index = *buffer_size;
	volatile int32_t size_offset = 0;
	volatile int32_t squ_first_entry = -1;
	volatile int32_t squ_last_entry = -1;
	//Проход с конца. Ищётся закрывающая скобка, запоминается, ищется открывающая
	//в интервале ищутся повторы обоих символов, после завершения ищется следующая пара
	while (1)
	{
		switch (buffer[_index])
		{
			//Отключено
			/*case '}':
			{
				//Ищутся вхождения
				last_entry = _index;
				for (volatile uint32_t _idx = _index; _idx > 0; --_idx)
				{
					if (buffer[_idx] == '{')
					{
						first_entry = _idx + 1;
						clean->assign(&buffer[first_entry], last_entry - first_entry);
						testpoint1 = clean->c_str();
						break;
					}
				}
				break;
			}
			case ']':
			{
				//Ищутся вхождения
				last_entry = _index;
				for (volatile uint32_t _idx = _index; _idx > 0; --_idx)
				{
					if (buffer[_idx] == '[')
					{
						first_entry = _idx + 1;
						clean->assign(&buffer[first_entry], last_entry - first_entry);
						testpoint1 = clean->c_str();
						break;
					}
				}
				break;
			}*/
			case '>':
			{
				//Ищутся вхождения
				last_entry = _index;
				for (volatile int32_t _idx = _index; _idx >= 0; --_idx)
				{
					if (buffer[_idx] == '<')
					{
						first_entry = _idx;
						break;
					}
				}
				//Выход если вхождения больше не найдены
				if (first_entry == -1)
					break;
				//Поправка - ищется '<', ближайший к '>' в сторону уменьшения
				for (volatile int32_t _idx = first_entry; _idx >= 0; --_idx)
				{
					if (buffer[_idx] == '<')
					{
						first_entry = _idx + 1;
					}
					if ((buffer[_idx] == '>') || (_idx == 0))
					{
						testpoint3 = first_entry;
						testpoint3 = last_entry;
						clean->assign(&buffer[first_entry], last_entry - first_entry);
						testpoint1 = clean->c_str();
						break;
					}
				}
				//Ищется наличие повторов внутри, все повторы заменяются на html коды
				testpoint1 = clean->c_str();
				for (volatile int32_t _idx = clean->size() - 1; _idx >= 0; --_idx)
				{
					if (bracketsSrc.find(clean->at(_idx)) != -1)
					{
						clean->replace(_idx, 1, bracketsTable.at(bracketsSrc.find(clean->at(_idx))).c_str());
						//size_offset += 4;
						testpoint1 = clean->c_str();
						testpoint1 = testpoint1;
					}
				}
				testpoint1 = clean_buffer->c_str();
				clean_buffer->replace(first_entry, last_entry - first_entry, clean->c_str());
				testpoint1 = clean_buffer->c_str();

				//Поправка положения указателя
				if (first_entry >= 1)
					_index = first_entry - 1;
				else
					_index = first_entry;

				//Очистка перед следующим заходом
				clean->clear();
				size_offset = 0;
				first_entry = -1;
				last_entry = -1;

				break;
			}
			//Отключено
			case ')':
			{
				//Ищутся вхождения
				last_entry = _index;
				for (volatile int32_t _idx = _index; _idx >= 0; --_idx)
				{
					if (buffer[_idx] == '(')
					{
						first_entry = _idx + 1;
						//clean->assign(&buffer[first_entry], last_entry - first_entry);
						//testpoint1 = clean->c_str();
						break;
					}
				}
				//testpoint1 = clean_buffer->c_str();
				//clean_buffer->replace(squ_last_entry + size_offset + 2, last_entry - first_entry, clean->c_str());
				//testpoint1 = clean_buffer->c_str();

				//Поправка положения указателя
				if (first_entry >= 1)
					_index = first_entry - 1;
				else
					_index = first_entry;

				//Очистка перед следующим заходом
				clean->clear();
				size_offset = 0;
				first_entry = -1;
				last_entry = -1;

				break;
			}
		}
		//Выход если достигнут 0
		if (_index <= 0)
			break;
		else
			--_index;
	}
	testpoint1 = clean_buffer->c_str();
	testpoint1 = testpoint1;

	//Очистка закрывающих символов которые были до первого открывающего

	//Очистка '>'
	for (volatile int32_t _idx = 0; _idx < clean_buffer->size(); ++_idx)
	{
		if (clean_buffer->at(_idx) == '<')
		{
			first_entry = _idx - 1;
			for (volatile int32_t _index = first_entry; _index >= 0; --_index)
			{
				if (bracketsSrc.find(clean_buffer->at(_index)) != -1)
				{
					clean_buffer->replace(_index, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_index))).c_str());
					//size_offset += 4;
					//testpoint1 = clean->c_str();
					//testpoint1 = testpoint1;
				}
			}
			break;
		}
	}
	testpoint1 = clean_buffer->c_str();
	testpoint1 = testpoint1;

	//Оистка '<'
	for (volatile int32_t _idx = clean_buffer->size() - 1; _idx >= 0; --_idx)
	{
		if (clean_buffer->at(_idx) == '>')
		{
			//last_entry = clean_buffer->size();
			last_entry = _idx + 1;
			for (volatile int32_t _index = last_entry; _index < clean_buffer->size(); ++_index)
			{
				if (bracketsSrc.find(clean_buffer->at(_index)) != -1)
				{
					clean_buffer->replace(_index, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_index))).c_str());
					//size_offset += 4;
					//testpoint1 = clean->c_str();
					//testpoint1 = testpoint1;
				}
			}
			break;
		}
	}
	testpoint1 = clean_buffer->c_str();
	testpoint1 = testpoint1;

	//Очистка ')'
	for (volatile int32_t _idx = 0; _idx < clean_buffer->size(); ++_idx)
	{
		if (clean_buffer->at(_idx) == '(')
		{
			first_entry = _idx - 1;
			for (volatile int32_t _index = first_entry; _index >= 0; --_index)
			{
				if (clean_buffer->at(_index) == ')')
				{
					clean_buffer->replace(_index, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_index))).c_str());
					//size_offset += 4;
					//testpoint1 = clean->c_str();
					//testpoint1 = testpoint1;
				}
			}
			break;
		}
	}
	testpoint1 = clean_buffer->c_str();
	testpoint1 = testpoint1;

	//Оистка '<'
	for (volatile int32_t _idx = clean_buffer->size() - 1; _idx >= 0; --_idx)
	{
		if (clean_buffer->at(_idx) == ')')
		{
			//last_entry = clean_buffer->size();
			last_entry = _idx + 1;
			for (volatile int32_t _index = last_entry; _index < clean_buffer->size(); ++_index)
			{
				if (clean_buffer->at(_index) == '(')
				{
					clean_buffer->replace(_index, 1, bracketsTable.at(bracketsSrc.find(clean_buffer->at(_index))).c_str());
					//size_offset += 4;
					//testpoint1 = clean->c_str();
					//testpoint1 = testpoint1;
				}
			}
			break;
		}
	}
	testpoint1 = clean_buffer->c_str();
	testpoint1 = testpoint1;

	//std::string old_buffer = rawInput;

	return clean_buffer->c_str();
}
