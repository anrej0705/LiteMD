﻿#include "headerLvlParser.h"
#include <boost/container/string.hpp>
#include "global_definitions.h"
#include "exceptionHandler.h"

//boost::container::string* head_lvl_url_output;
std::string* head_lvl_url_output;

std::string headerLvlParser(std::string& rawInput)
{
	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//Создаём переменную с количеством символов

	head_lvl_url_output = new std::string;
	//head_lvl_url_output = new boost::container::string;

	int32_t stroke_start = -1;
	int32_t stroke_end = -1;
	uint8_t header_size = 0;

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//Медленно, //Создаём буфер с размером входящего блока и копируем его туда

	head_lvl_url_output->assign(buffer);

	for (volatile int32_t _index = *buffer_size; _index >= 0; --_index)
	{
		//Если найден символ переноса то считаем за конец строки
		if (buffer[_index] == '\n')
		{
			//Поиск окончания служебных символов
			for (volatile int32_t _idx = _index; _idx >= 0; --_idx)
			{
				if ((buffer[_idx] != '\r') && (buffer[_idx] != '\n') && (buffer[_idx] != '\0'))
				{
					stroke_end = _idx;
					break;
				}
			}
			//Начинаем искать начало строки
			for (volatile int32_t _idx = stroke_end; _idx >= 0; --_idx)
			{
				if ((buffer[_idx] == '\n') || (_idx == 0))
				{
					//Если найдено начало или индекст дошёл до нуля то сохраняем
					(buffer[_idx] == '\n') ? stroke_start = _idx + 1 : stroke_start = 0;
					break;
				}
			}

			//Проверяем первые 5 символов, если символов решётки не найдено или ряд сбит то сохраняется
			//количество символов до сбития ряда или 0
			for (volatile int32_t _idx = stroke_start; _idx < stroke_start + 5; ++_idx)
			{
				if (buffer[_idx] == '#')
					++header_size;
				else
					break;
			}

			//Если символы не найдены то считается что строка не содержит символов заголовка
			//индекс начала сохраняется как индекс конца и цикл идёт дальше
			if ((header_size == 0) && (_index > 0))
				_index = stroke_start - 1;
			else
			{	//Строится заголовок из данных header_size;
				switch (header_size)
				{
					case 1:
					{
						//Вставка тега в конце
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_end + 1, "1");
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_iclosetext);
						//Вставка тега в начале
						//head_lvl_url_output->insert(stroke_start, header_lvl_icloselvl);
						head_lvl_url_output->replace(stroke_start, 1, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_start, "1");
						//head_lvl_url_output->replace(stroke_start, 1, header_lvl_iopenlvl);
						head_lvl_url_output->insert(stroke_start, header_lvl_iopenlvl);
						break;
					}
					case 2:
					{
						//Вставка тега в конце
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_end + 1, "2");
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_iclosetext);
						//Вставка тега в начале
						//head_lvl_url_output->insert(stroke_start, header_lvl_icloselvl);
						head_lvl_url_output->replace(stroke_start, 2, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_start, "2");
						//head_lvl_url_output->replace(stroke_start, 2, header_lvl_iopenlvl);
						head_lvl_url_output->insert(stroke_start, header_lvl_iopenlvl);
						break;
					}
					case 3:
					{
						//Вставка тега в конце
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_end + 1, "3");
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_iclosetext);
						//Вставка тега в начале
						//head_lvl_url_output->insert(stroke_start, header_lvl_icloselvl);
						head_lvl_url_output->replace(stroke_start, 3, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_start, "3");
						//head_lvl_url_output->replace(stroke_start, 3, header_lvl_iopenlvl);
						head_lvl_url_output->insert(stroke_start, header_lvl_iopenlvl);
						break;
					}
					case 4:
					{
						//Вставка тега в конце
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_end + 1, "4");
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_iclosetext);
						//Вставка тега в начале
						//head_lvl_url_output->insert(stroke_start, header_lvl_icloselvl);
						head_lvl_url_output->replace(stroke_start, 4, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_start, "4");
						//head_lvl_url_output->replace(stroke_start, 4, header_lvl_iopenlvl);
						head_lvl_url_output->insert(stroke_start, header_lvl_iopenlvl);
						break;
					}
					case 5:
					{
						//Вставка тега в конце
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_end + 1, "5");
						head_lvl_url_output->insert(stroke_end + 1, header_lvl_iclosetext);
						//Вставка тега в начале
						//head_lvl_url_output->insert(stroke_start, header_lvl_icloselvl);
						head_lvl_url_output->replace(stroke_start, 5, header_lvl_icloselvl);
						head_lvl_url_output->insert(stroke_start, "5");
						//head_lvl_url_output->replace(stroke_start, 5, header_lvl_iopenlvl);
						head_lvl_url_output->insert(stroke_start, header_lvl_iopenlvl);
						break;
					}
				}
			}
			//Сброс для работы со следующей строкой
			header_size = 0;
			//Перемещение указателя на позицию строки с которой шла работа
			_index = stroke_start;
			//Сброс значений так как новая строчка и пока ничего не понятно
			stroke_start = -1;
			stroke_end = 1;
		}
	}

	free(buffer_size);
	free(buffer);

	return head_lvl_url_output->c_str();
}
