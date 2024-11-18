#include <boost/container/string.hpp>
#include "urlAdvancedParser.h"
#include "global_definitions.h"

boost::container::string* format_url_output;

std::string advancedUrlParser(std::string& rawInput)
{
	//Счетчики для каждого элемента тега
	uint32_t squ_brackets_entry = 0;
	uint32_t squ_brackets_offset = 0;
	uint32_t brackets_entry = 0;
	uint32_t brackets_offset = 0;

	format_url_output = new boost::container::string;	//Бустовый буфер который быстрее STDшного

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	char* buffer = (char*)malloc(*buffer_size);
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

	//Ищем вхождения по знаку '<'
	for (volatile uint32_t _index = 0; _index < *buffer_size; ++_index)
	{
		if (buffer[_index] == '[')	//Любое найденое вхождение запоминаем на будущее
		{
			++squ_brackets_entry;	//realloc требуемый размер + 1 чтобы не вылезать за пределы
			squ_entry_list = (uint32_t*)realloc(squ_entry_list, sizeof(uint32_t) * (squ_brackets_entry + 1));
			squ_entry_list[squ_brackets_entry - 1] = _index + 1;
		}
	}

	//Есть вероятность, что юзер оставит неполный тег, чтобы не вызвать
	//ошибку в памяти добавляется дополнительное вхождение равное позиции
	//последнего символа в тексте
	squ_entry_list = (uint32_t*)realloc(squ_entry_list, sizeof(uint32_t) * (squ_brackets_entry + 1));
	squ_entry_list[squ_brackets_entry] = *buffer_size + 1;	//Костыль с фиксом последнего символа

	if (entry_list[0] != 0)	//Если первое вхождение не является началом блока то отмечаем 0 как смещение
	{
		++squ_brackets_offset;
		squ_offsets = (uint32_t*)realloc(squ_offsets, sizeof(uint32_t) * (squ_brackets_offset + 1) + sizeof(uint32_t));
		squ_offsets[squ_brackets_offset - 1] = 0;
	}

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
				++squ_brackets_offset;
				squ_offsets = (uint32_t*)realloc(squ_offsets, sizeof(uint32_t) * (squ_brackets_offset + 1));
				squ_offsets[squ_brackets_offset - 1] = _index;
			}
		}
	}

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
				++brackets_entry;
				entry_list = (uint32_t*)realloc(entry_list, sizeof(uint32_t) * (brackets_entry + 1));
				entry_list[brackets_entry - 1] = _index + 1;
			}
		}
	}

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
				++brackets_offset;
				offsets = (uint32_t*)realloc(offsets, sizeof(uint32_t) * (brackets_offset + 1));
				offsets[brackets_offset - 1] = _index;
			}
		}
	}

	//Этап конвертации и сборки текста. Вместо символов '<' и '>' вставляется '<a href="'+текст+'">'+текст+'</a>'

	uint32_t entrys_cnt = 0;
	uint32_t blocks_cnt = 0;

	if (entry_list[entrys_cnt] == 0)	//Если тег начинается с первого символа то формируем строчку и +1 entrys_cnt
	{									//и выполняем сборку на месте
		//Сборка
		for (volatile uint32_t _part_idx = entrys_cnt; _part_idx < squ_brackets_entry;)
		{
			//Сборка в циклi
			for (volatile uint32_t _part_idx = 0; _part_idx < squ_brackets_entry; ++_part_idx)
			{
				++entrys_cnt;
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
			}
		}
	}
	else
	{

		//Копирование текста до тега
		format_url_output->append(&buffer[squ_offsets[entrys_cnt]], squ_entry_list[entrys_cnt] - 1);

		//Сборка в циклi
		for (volatile uint32_t _part_idx = 0; _part_idx < squ_brackets_entry; ++_part_idx)
		{
			++entrys_cnt;
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
		}
	}

	//чистка указателей
	free(squ_entry_list);
	free(squ_offsets);
	free(entry_list);
	free(offsets);

	return format_url_output->c_str();
}
