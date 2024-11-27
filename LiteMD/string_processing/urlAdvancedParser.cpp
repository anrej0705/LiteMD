#include <boost/container/string.hpp>
#include "urlAdvancedParser.h"
#include "global_definitions.h"
#include "exceptionHandler.h"
#include "logger_backend.h"

//boost::container::string* format_url_output;
std::string* format_url_output;


std::string advancedUrlParser(std::string& rawInput)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	//Счетчики для каждого элемента тега
	uint32_t squ_brackets_entry = 0;
	uint32_t squ_brackets_offset = 0;
	uint32_t brackets_entry = 0;
	uint32_t brackets_offset = 0;

	//format_url_output = new boost::container::string;	//Бустовый буфер который быстрее STDшного
	format_url_output = new std::string;	//Бустовый буфер который быстрее STDшного

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	uint32_t* squ_entry_list;	//'['
	uint32_t* squ_offsets;		//']'
	uint32_t* entry_list;		//'('
	uint32_t* offsets;			//')'

	squ_entry_list = (uint32_t*)malloc(sizeof(uint32_t));
	squ_offsets = (uint32_t*)malloc(sizeof(uint32_t));
	entry_list = (uint32_t*)malloc(sizeof(uint32_t));
	offsets = (uint32_t*)malloc(sizeof(uint32_t));
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^почистить в конце

	//Дальше будет поиск признаков тега, пока что нужно считать что признаки есть
	//а объекта нет, здесь будет искаться выражение <url> где url - любой текст внутри
	//который будет принят за ссылку неважно что это за текст
	//Референс https://www.markdownguide.org/basic-syntax/#formatting-links

	push_log("[urlAdvancedParser]Поиск вхождения по знаку '['");

	try
	{
		//Ищем вхождения по знаку '['
		for (volatile uint32_t _index = 0; _index < *buffer_size; ++_index)
		{
			if (buffer[_index] == '[')	//Любое найденое вхождение запоминаем на будущее
			{
				log_stroke->append("[urlAdvancedParser]Найдено вхождение в позиции ");
				log_stroke->append(std::to_string(_index + 1).c_str());
				push_log(log_stroke->c_str());
				++squ_brackets_entry;	//realloc требуемый размер + 1 чтобы не вылезать за пределы
				squ_entry_list = (uint32_t*)realloc(squ_entry_list, sizeof(uint32_t) * (squ_brackets_entry + 1));
				squ_entry_list[squ_brackets_entry - 1] = _index + 1;
				log_stroke->clear();
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlAdvancedParser.cpp 40:52")));
	}

	try
	{
		//Есть вероятность, что юзер оставит неполный тег, чтобы не вызвать
		//ошибку в памяти добавляется дополнительное вхождение равное позиции
		//последнего символа в тексте
		squ_entry_list = (uint32_t*)realloc(squ_entry_list, sizeof(uint32_t) * (squ_brackets_entry + 1));
		squ_entry_list[squ_brackets_entry] = *buffer_size + 1;	//Костыль с фиксом последнего символа

		if (squ_entry_list[0] != 0)	//Если первое вхождение не является началом блока то отмечаем 0 как смещение
		{
			log_stroke->append("[urlAdvancedParser]Добавлена зона поиска повторов (0-");
			++squ_brackets_offset;
			squ_offsets = (uint32_t*)realloc(squ_offsets, sizeof(uint32_t) * (squ_brackets_offset + 1) + sizeof(uint32_t));
			squ_offsets[squ_brackets_offset - 1] = 0;
			log_stroke->append(std::to_string(squ_offsets[squ_brackets_offset]).c_str());
			log_stroke->append(")");
			push_log(log_stroke->c_str());
			log_stroke->clear();
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlAdvancedParser.cpp 58:72")));
	}

	push_log("[urlAdvancedParser]Поиск закрыющего знака ']'");

	try
	{
		//Ищем закрывающие знаки ']' от индекса вхождения
		for (volatile uint32_t _entry_idx = 0; _entry_idx < squ_brackets_entry; ++_entry_idx)
		{
			//Небольшая доработка предыдущего алгоритма. Для ускорения и предовтращения
			//натыканий на один и тот же знак, поиск будет проводиться с мест где был обнаружен
			//открывающий символ '['
			for (uint32_t _index = squ_entry_list[_entry_idx]; _index < squ_entry_list[_entry_idx + 1]; ++_index)
			{
				if (buffer[_index] == ']')
				{
					log_stroke->append("[urlAdvancedParser]Найдено вхождение в позиции ");
					log_stroke->append(std::to_string(_index).c_str());
					push_log(log_stroke->c_str());
					++squ_brackets_offset;
					squ_offsets = (uint32_t*)realloc(squ_offsets, sizeof(uint32_t) * (squ_brackets_offset + 1));
					squ_offsets[squ_brackets_offset - 1] = _index;
					log_stroke->clear();
				}
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlAdvancedParser.cpp 78:96")));
	}

	push_log("[urlAdvancedParser]Поиск вхождения по знаку '('");

	try
	{
		//Та же тема
		squ_offsets = (uint32_t*)realloc(squ_offsets, sizeof(uint32_t) * (squ_brackets_offset + 1));
		squ_offsets[squ_brackets_offset] = *buffer_size + 1;	//Костыль с фиксом последнего символа

		volatile uint32_t _entry_idx = 0;

		if (entry_list[0] != 0)	//Раннее было смещение, это нужно учесть и добавляется +1 к позиции текущего индекса
			_entry_idx = 1;

		//Ищем закрывающие знаки '(' от индекса вхождения
		for (; _entry_idx < squ_brackets_offset; ++_entry_idx)
		{
			//Небольшая доработка предыдущего алгоритма. Для ускорения и предовтращения
			//натыканий на один и тот же знак, поиск будет проводиться с мест где был обнаружен
			//открывающий символ ']'
			for (uint32_t _index = squ_offsets[_entry_idx]; _index < squ_offsets[_entry_idx + 1]; ++_index)
			{
				if (buffer[_index] == '(')
				{
					log_stroke->append("[urlAdvancedParser]Найдено вхождение в позиции ");
					log_stroke->append(std::to_string(_index + 1).c_str());
					push_log(log_stroke->c_str());
					++brackets_entry;
					entry_list = (uint32_t*)realloc(entry_list, sizeof(uint32_t) * (brackets_entry + 1));
					entry_list[brackets_entry - 1] = _index + 1;
					log_stroke->clear();
				}
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlAdvancedParser.cpp 102:129")));
	}

	push_log("[urlAdvancedParser]Поиск закрыющего знака ')'");

	try
	{
		//Та же тема только с другой парой
		entry_list = (uint32_t*)realloc(entry_list, sizeof(uint32_t) * (squ_brackets_entry + 1));
		entry_list[squ_brackets_entry] = *buffer_size + 1;	//Костыль с фиксом последнего символа

		//Ищем закрывающие знаки ')' от индекса вхождения
		for (volatile uint32_t _entry_idx = 0; _entry_idx < squ_brackets_entry; ++_entry_idx)
		{
			//Небольшая доработка предыдущего алгоритма. Для ускорения и предовтращения
			//натыканий на один и тот же знак, поиск будет проводиться с мест где был обнаружен
			//открывающий символ '('
			for (uint32_t _index = entry_list[_entry_idx]; _index < entry_list[_entry_idx + 1]; ++_index)
			{
				if (buffer[_index] == ')')
				{
					log_stroke->append("[urlAdvancedParser]Найдено вхождение в позиции ");
					log_stroke->append(std::to_string(_index).c_str());
					push_log(log_stroke->c_str());
					++brackets_offset;
					offsets = (uint32_t*)realloc(offsets, sizeof(uint32_t) * (brackets_offset + 1));
					offsets[brackets_offset - 1] = _index;
					log_stroke->clear();
				}
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlAdvancedParser.cpp 135:157")));
	}

	//Этап конвертации и сборки текста. Вместо символов '<' и '>' вставляется '<a href="'+текст+'">'+текст+'</a>'

	push_log("[urlAdvancedParser]Сборка документа со вставкой тегов");

	uint32_t entrys_cnt = 0;
	uint32_t blocks_cnt = 0;

	try
	{
		if (entry_list[entrys_cnt] == 0)	//Если тег начинается с первого символа то формируем строчку и +1 entrys_cnt
		{									//и выполняем сборку на месте
			push_log("[urlAdvancedParser]Обнаружено начало тега на 0 индексе");
			//Сборка
			for (volatile uint32_t _part_idx = entrys_cnt; _part_idx < squ_brackets_entry;)
			{
				
				//Сборка в циклi
				for (volatile uint32_t _part_idx = 0; _part_idx < squ_brackets_entry; ++_part_idx)
				{
					++entrys_cnt;
					log_stroke->append("[urlAdvancedParser]Вставка тега (");
					log_stroke->append(std::to_string(entry_list[entrys_cnt - 1]).c_str());
					log_stroke->append("-");
					log_stroke->append(std::to_string(offsets[entrys_cnt - 1]).c_str());
					log_stroke->append(")[");
					log_stroke->append(std::to_string(squ_entry_list[entrys_cnt - 1]).c_str());
					log_stroke->append("-");
					log_stroke->append(std::to_string(squ_offsets[entrys_cnt]).c_str());
					log_stroke->append("]");
					//Вставка тега <a href="
					format_url_output->append(simple_url_iopenurl, simple_url_iopenurl_size);
					//Вставка текста-ссылки
					format_url_output->append(&buffer[entry_list[entrys_cnt - 1]], offsets[entrys_cnt - 1] - entry_list[entrys_cnt - 1]);
					//Вставка закрывающего ссылку ">
					format_url_output->append(simple_url_icloseurl, simple_url_icloseurl_size);
					//Вставка кликабельного текста
					format_url_output->append(&buffer[squ_entry_list[entrys_cnt - 1]], squ_offsets[entrys_cnt] - squ_entry_list[entrys_cnt - 1]);
					//Вставка закрывающего тега
					format_url_output->append(simple_url_iclosetext, simple_url_iclosetext_size);
					//Вставка текста между тегами
					format_url_output->append(&buffer[offsets[entrys_cnt - 1]] + 1, squ_entry_list[entrys_cnt] - offsets[entrys_cnt - 1] - 2);
					push_log(log_stroke->c_str());
					log_stroke->clear();
				}
			}
		}
		else
		{
			log_stroke->append("[urlAdvancedParser]Обнаружено начало тега на индексе ");
			log_stroke->append(std::to_string(entry_list[entrys_cnt]).c_str());
			log_stroke->append(" сборка по альтернативному алгоритму");
			push_log(log_stroke->c_str());
			log_stroke->clear();
			//Копирование текста до тега
			format_url_output->append(&buffer[squ_offsets[entrys_cnt]], squ_entry_list[entrys_cnt] - 1);

			//Сборка в циклi
			for (volatile uint32_t _part_idx = 0; _part_idx < squ_brackets_entry; ++_part_idx)
			{
				++entrys_cnt;
				log_stroke->append("[urlAdvancedParser]Вставка тега (");
				log_stroke->append(std::to_string(entry_list[entrys_cnt - 1]).c_str());
				log_stroke->append("-");
				log_stroke->append(std::to_string(offsets[entrys_cnt - 1]).c_str());
				log_stroke->append(")[");
				log_stroke->append(std::to_string(squ_entry_list[entrys_cnt - 1]).c_str());
				log_stroke->append("-");
				log_stroke->append(std::to_string(squ_offsets[entrys_cnt]).c_str());
				log_stroke->append("]");
				//Вставка тега <a href="
				format_url_output->append(simple_url_iopenurl, simple_url_iopenurl_size);
				//Вставка текста-ссылки
				format_url_output->append(&buffer[entry_list[entrys_cnt - 1]], offsets[entrys_cnt - 1] - entry_list[entrys_cnt - 1]);
				//Вставка закрывающего ссылку ">
				format_url_output->append(simple_url_icloseurl, simple_url_icloseurl_size);
				//Вставка кликабельного текста
				format_url_output->append(&buffer[squ_entry_list[entrys_cnt - 1]], squ_offsets[entrys_cnt] - squ_entry_list[entrys_cnt - 1]);
				//Вставка закрывающего тега
				format_url_output->append(simple_url_iclosetext, simple_url_iclosetext_size);
				//Вставка текста между тегами
				format_url_output->append(&buffer[offsets[entrys_cnt - 1]] + 1, squ_entry_list[entrys_cnt] - offsets[entrys_cnt - 1] - 2);
				push_log(log_stroke->c_str());
				log_stroke->clear();
			}
		}
	}
	catch (exceptionHandler)
	{
		throw(exceptionHandler(exceptionHandler::FATAL, QString("Карма в говне! - Ошибка работы с памятью в urlAdvancedParser.cpp 168:218")));
	}

	push_log("[urlAdvancedParser]Сборка завершена, чистка памяти");

	//чистка указателей
	free(squ_entry_list);
	free(squ_offsets);
	free(entry_list);
	free(offsets);
	free(buffer_size);
	free(buffer);

	delete(log_stroke);
	//return rawInput.c_str();
	return format_url_output->c_str();
}
