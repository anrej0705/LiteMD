#include "globalFlags.h"
#include <stdbool.h>

//settings settingsStruct;

bool fileOpenedState=0;
bool fileChangedState=0;
bool appTitleUpdated=0;
bool appClose = 0;
bool enableDeprFeatures = 0;
bool enableIndevFeatures = 0;
int langCode = 0;
//Deprecated
bool allowHttpWarn = 1;
bool httpDerpWarned = 0;
bool settingChanged = 0;
//bool uiChanged = 0;

//globalFlaghs.h 23:29
int dataLocation = 127;
//Выбранная тема
char* chosenTheme;

//Настрйоки, они же настройки по умолчанию. Значения подменяются из файла конфига при чтении, если последний существует
bool logReadState = 0;

char** lastFiles = NULL;
//Последние использованные файлы, под патч SilverWolf2k20 -> Последние файлы
void newRecentFilesArray()
{
	//lastFiles = (char**)malloc(NUMBER_OF_FILES * sizeof(char*));	//Инициализируем строки
	lastFiles = (char**)calloc(NUMBER_OF_FILES, sizeof(char*));
	for (uint8_t index = 0; index < NUMBER_OF_FILES; ++index)	//Инициализируем ячейки в строках
	{
		lastFiles[index] = (char*)calloc(FILENAME_SIZE * sizeof(char));
	}
}

void deleteOnExit()
{
	//Удаляем массив
	if (lastFiles != NULL)
	{
		for (uint8_t index = 0; index < NUMBER_OF_FILES; ++index)
		{
			if(lastFiles[index] != NULL)	//Исправлена ошибка в журналах windows после закрытия
				free(lastFiles[index]);
		}
		free(lastFiles);
	}
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
