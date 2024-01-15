#include "globalFlags.h"
#include <stdbool.h>

//settings settingsStruct;

bool fileOpenedState=0;
bool fileChangedState=0;
bool appTitleUpdated=0;
bool appClose = 0;
//Deprecated
bool allowHttpWarn = 1;
bool httpDerpWarned = 0;

//Настрйоки, они же настройки по умолчанию. Значения подменяются из файла конфига при чтении, если последний существует
bool logReadState = 0;

//Последние использованные файлы, под патч SilverWolf2k20 -> Последние файлы
char** lastFiles = NULL;
void newRecentFilesArray()
{
	lastFiles = (char**)malloc(NUMBER_OF_FILES * sizeof(char*));	//Инициализируем строки
	for (uint8_t index = 0; index < NUMBER_OF_FILES; ++index)	//Инициализируем ячейки в строках
	{
		lastFiles[index] = (char*)malloc(FILENAME_SIZE * sizeof(char));
		for (uint16_t colIndex = 0; colIndex < FILENAME_SIZE; ++colIndex)
			lastFiles[index][colIndex] = (char)NULL;  //Чистим от мусора
	}
}

void deleteOnExit()
{
	//Удаляем массив
	for (uint8_t index = 0; index < NUMBER_OF_FILES; ++index)
	{
		free(lastFiles[index]);
	}
	free(lastFiles);
}

void deleteName(uint8_t pos)
{
	//чистим массив по индексу, memset применять нельзя(удаляет указатели)
	for (uint16_t subIndex = 0; subIndex < FILENAME_SIZE; ++subIndex)
	{
		lastFiles[pos][subIndex] = (char)NULL;
	}
}

void setName(uint8_t pos, char* source)
{
	//Копируем строку
	strcpy(lastFiles[pos], source);
}
