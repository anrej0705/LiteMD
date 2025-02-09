#include "quoteParser.h"
//Вот отсюда --->
#include <boost/container/string.hpp>
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
#include "global_definitions.h"
}
//---> и до сюда тупо копипиздинг из парсера в парсер, ничего практически менять не нужно

struct quoteStr
{
	int32_t quote_str_start = -1;
	int32_t quote_str_end = -1;
};

//boost::container::string* quote_output;
std::string* quote_output;

std::string quoteParser(std::string& rawInput)
{
	//Вот отсюда --->
	boost::container::string* log_out = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	push_log(std::string("[ПАРСЕР]Размер блока " + std::to_string(*buffer_size)));

	quote_output = new std::string;
	//li_output = new boost::container::string;

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	quote_output->assign(buffer);

	//---> и до сюда тупо копипиздинг из парсера в парсер, ничего практически менять не нужно

	push_log("[quoteParser]Провожу подсчёт строк");

	uint32_t magic_counter = 0;

	std::deque<uint32_t> strokes;
	//Собираем указатели на строчки
	for (uint32_t _index = 0; _index < *buffer_size; ++_index)
	{
		if (_index == 0)
		{
			strokes.push_back(_index);
			++magic_counter;
		}
		if (buffer[_index] == '\n')
		{
			++magic_counter;
			strokes.push_back(_index + 1);
		}
		else if (_index == *buffer_size - 1)
		{
			strokes.push_back(_index);
			++magic_counter;
		}
	}

	push_log(std::string("[quoteParser]Найдено " + std::to_string(magic_counter) + " строчек"));
	push_log("[quoteParser]Генерирую цитаты");
	magic_counter = 0;

	bool quote_found = 0;

	quoteStr quote_str;
	std::deque<quoteStr> lists;

	//Ищем '>' в начале строчки
	for (volatile uint32_t _idx = 0; _idx < strokes.size(); ++_idx)
	{
		//Если символы закончились но флаг поднят то считаем что читата закончилась
		if (quote_found && buffer[strokes.at(_idx)] != '>')
		{
			quote_found = 0;							//Скидываем флаг
			quote_str.quote_str_end = _idx;				//Записываем строчку как последнюю
			lists.push_back(quote_str);					//Сохраняем структуру
			++magic_counter;							//Прирост счётчика
		}
		//Если найден символ '>' то подымаем флаг
		if (buffer[strokes.at(_idx)] == '>')
		{
			quote_found = 1;							//Подымаем флаг
			quote_str.quote_str_start = _idx - 1;		//Сохраняем строчку как начало
		}
	}

	push_log(std::string("[quoteParser]Найдено " + std::to_string(magic_counter) + " цитат"));
	
	int testpoint2 = 0;

	//Вставляем цитаты
	if (lists.size() > 0)
	{
		for (volatile int32_t _idx = lists.size() - 1; _idx >= 0; --_idx)
		{
			testpoint2 = strokes.at(lists.at(_idx).quote_str_start);
			testpoint2 = strokes.at(lists.at(_idx).quote_str_end);
			push_log(std::string("[quoteParser]Вставка (" + std::to_string(lists.at(_idx).quote_str_start) + "-" + std::to_string(lists.at(_idx).quote_str_end) + ")"));
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_end), "\n");
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_end), blockqoute_str_iclose);
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_start), "\n");
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_start), blockqoute_str_iopen);

		}
	}

	char testpoint1;

	//Вот отсюда --->
	free(buffer);
	free(buffer_size);

	//return li_output->c_str();
	//---> и до сюда тупо копипиздинг из парсера в парсер, ничего практически менять не нужно

	return rawInput;
}
