#include <boost/container/string.hpp>
#include "urlBasicParser.h"
#include "global_definitions.h"
#include "exceptionHandler.h"
#include "logger_backend.h"

//std::string *simple_url_output;
boost::container::string *simple_url_output;

std::string basicUrlParser(std::string &rawInput)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	uint32_t* entry_list;	//Список с индексами, с которых начинаются обнаруженные символы
	uint32_t* entry_offset;	//Список со смещением до закрывающего знака
	// ^^^^^^^^^^^^^^^^^^^^ почистить в конце
	uint32_t entrys = 0;
	uint32_t offsets = 0;
	
	//simple_url_output = new std::string;
	simple_url_output = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	entry_list = (uint32_t*)calloc(1, sizeof(uint32_t));
	entry_offset = (uint32_t*)calloc(1, sizeof(uint32_t));

	//Дальше будет поиск признаков тега, пока что нужно считать что признаки есть
	//а объекта нет, здесь будет искаться выражение <url> где url - любой текст внутри
	//который будет принят за ссылку неважно что это за текст
	//Референс https://www.markdownguide.org/basic-syntax/#urls-and-email-addresses

	//std::string test;

	push_log("[urlBasicParser]Поиск вхождения по знаку '<'");

	try
	{
		//Ищем вхождения по знаку '<'
		for (volatile uint32_t _index = 0; _index < *buffer_size; ++_index)
		{
			if (buffer[_index] == '<')	//Любое найденое вхождение запоминаем на будущее
			{
				log_stroke->append("[urlBasicParser]Найдено вхождение в позиции ");
				log_stroke->append(std::to_string(_index + 1).c_str());
				push_log(log_stroke->c_str());
				++entrys;	//realloc требуемый размер + 1 чтобы не вылезать за пределы
				//strncpy(test, buffer, _index);
				entry_list = (uint32_t*)realloc(entry_list, sizeof(uint32_t) * (entrys + 1) + sizeof(uint32_t));
				entry_list[entrys - 1] = _index + 1;
				log_stroke->clear();
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlBasicParser.cpp 34:47")));
	}

	try
	{
		//Есть вероятность, что юзер оставит неполный тег, чтобы не вызвать
		//ошибку в памяти добавляется дополнительное вхождение равное позиции
		//последнего символа в тексте
		entry_list = (uint32_t*)realloc(entry_list, sizeof(uint32_t) * (entrys + 1));
		entry_list[entrys] = *buffer_size + 1;	//Костыль с фиксом последнего символа

		if (entry_list[0] != 0)	//Если первое вхождение не является началом блока то отмечаем 0 как смещение
		{
			log_stroke->append("[urlBasicParser]Добавлена зона поиска повторов (0-");
			++offsets;
			entry_offset = (uint32_t*)realloc(entry_offset, sizeof(uint32_t) * (offsets + 1) + sizeof(uint32_t));
			entry_offset[offsets - 1] = 0;
			log_stroke->append(std::to_string(entry_offset[offsets - 1]).c_str());
			log_stroke->append(")");
			push_log(log_stroke->c_str());
			log_stroke->clear();
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlBasicParser.cpp 53:67")));
	}

	push_log("[urlBasicParser]Поиск закрыющего знака '>'");

	try
	{
		//Ищем закрывающие знаки '>' от индекса вхождения
		for (volatile uint32_t _entry_idx = 0; _entry_idx < entrys; ++_entry_idx)
		{
			//Небольшая доработка предыдущего алгоритма. Для ускорения и предовтращения
			//натыканий на один и тот же знак, поиск будет проводиться с мест где был обнаружен
			//открывающий символ '<'
			for (uint32_t _index = entry_list[_entry_idx]; _index < entry_list[_entry_idx + 1]; ++_index)
			{
				if (buffer[_index] == '>')
				{
					log_stroke->append("[urlBasicParser]Найдено вхождение в позиции ");
					log_stroke->append(std::to_string(_index).c_str());
					push_log(log_stroke->c_str());
					++offsets;
					entry_offset = (uint32_t*)realloc(entry_offset, sizeof(uint32_t) * (offsets + 1));
					entry_offset[offsets - 1] = _index;
					log_stroke->clear();
				}
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlBasicParser.cpp 73:91")));
	}

	//Этап конвертации и сборки текста. Вместо символов '<' и '>' вставляется '<a href="'+текст+'">'+текст+'</a>'

	push_log("[urlBasicParser]Сборка документа со вставкой тегов");

	uint32_t entrys_cnt = 0;
	uint32_t blocks_cnt = 0;

	try
	{
		if (entry_list[entrys_cnt] == 0)	//Если тег начинается с первого символа то формируем строчку и +1 entrys_cnt
		{									//и выполняем сборку на месте
			push_log("[urlBasicParser]Обнаружено начало тега на 0 индексе");
			//Сборка
			for (volatile uint32_t _part_idx = entrys_cnt; _part_idx < entrys + offsets + 0;)
			{
				//Сборка в циклi
				for (volatile uint32_t _part_idx = 0; _part_idx < entrys; ++_part_idx)
				{
					log_stroke->append("[urlBasicParser]Вставка тега <");
					log_stroke->append(std::to_string(entry_list[entrys_cnt - 1]).c_str());
					log_stroke->append("-");
					log_stroke->append(std::to_string(entry_offset[entrys_cnt]).c_str());
					log_stroke->append(">");
					//Вставка тега <a href="
					simple_url_output->append(simple_url_iopenurl, simple_url_iopenurl_size);
					//Вставка текста-ссылки
					//testpoint1 = entry_offset[entrys_cnt];
					//testpoint2 = entry_list[entrys_cnt - 1];
					simple_url_output->append(&buffer[entry_list[entrys_cnt - 1]], entry_offset[entrys_cnt] - entry_list[entrys_cnt - 1]);
					//Вставка закрывающего ссылку ">
					simple_url_output->append(simple_url_icloseurl, simple_url_icloseurl_size);
					//Вставка кликабельного текста
					simple_url_output->append(&buffer[entry_list[entrys_cnt - 1]], entry_offset[entrys_cnt] - entry_list[entrys_cnt - 1]);
					//Вставка закрывающего тега
					simple_url_output->append(simple_url_iclosetext, simple_url_iclosetext_size);
					//Вставка текста между тегами
					simple_url_output->append(&buffer[entry_offset[entrys_cnt]] + 1, entry_list[entrys_cnt] - entry_offset[entrys_cnt] - 2);
					++entrys_cnt;
					push_log(log_stroke->c_str());
					log_stroke->clear();
				}
			}
		}
		else
		{
			log_stroke->append("[urlBasicParser]Обнаружено начало тега на индексе ");
			log_stroke->append(std::to_string(entry_list[entrys_cnt]).c_str());
			log_stroke->append(" сборка по альтернативному алгоритму");
			push_log(log_stroke->c_str());
			log_stroke->clear();
			//uint32_t testpoint1 = 0;
			//uint32_t testpoint2 = 0;

			std::string test;

			//Копирование текста до тега
			//testpoint1 = entry_offset[entrys_cnt];
			//testpoint2 = entry_list[entrys_cnt];
			simple_url_output->append(&buffer[entry_offset[entrys_cnt]], entry_list[entrys_cnt] - 1);

			//Сборка в циклi
			for (volatile uint32_t _part_idx = 0; _part_idx < entrys; ++_part_idx)
			{
				++entrys_cnt;
				log_stroke->append("[urlBasicParser]Вставка тега <");
				log_stroke->append(std::to_string(entry_list[entrys_cnt - 1]).c_str());
				log_stroke->append("-");
				log_stroke->append(std::to_string(entry_offset[entrys_cnt]).c_str());
				log_stroke->append(">");
				//Вставка тега <a href="
				simple_url_output->append(simple_url_iopenurl, simple_url_iopenurl_size);
				//Вставка текста-ссылки
				//testpoint1 = entry_offset[entrys_cnt];
				//testpoint2 = entry_list[entrys_cnt - 1];
				simple_url_output->append(&buffer[entry_list[entrys_cnt - 1]], entry_offset[entrys_cnt] - entry_list[entrys_cnt - 1]);
				//Вставка закрывающего ссылку ">
				simple_url_output->append(simple_url_icloseurl, simple_url_icloseurl_size);
				//Вставка кликабельного текста
				simple_url_output->append(&buffer[entry_list[entrys_cnt - 1]], entry_offset[entrys_cnt] - entry_list[entrys_cnt - 1]);
				//Вставка закрывающего тега
				simple_url_output->append(simple_url_iclosetext, simple_url_iclosetext_size);
				//Вставка текста между тегами
				simple_url_output->append(&buffer[entry_offset[entrys_cnt]] + 1, entry_list[entrys_cnt] - entry_offset[entrys_cnt] - 2);
				push_log(log_stroke->c_str());
				log_stroke->clear();
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlBasicParser.cpp 102:162")));
	}

	push_log("[urlBasicParser]Сборка завершена, чистка памяти");

	//Чистка указателей
	free(entry_list);
	free(entry_offset);
	free(buffer_size);
	free(buffer);
	delete(log_stroke);

	//return rawInput.c_str();
	return simple_url_output->c_str();
}
