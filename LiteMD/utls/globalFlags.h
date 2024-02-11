#pragma once
#ifndef GLOBALFLAGS_H_
#define GLOBALFLAGS_H_
#include <stdbool.h>
#include <stdint.h>
//Флаги состояний
extern bool fileOpenedState;
extern bool fileChangedState;
extern bool appTitleUpdated;
extern bool appClose;
//Deprecated
extern bool allowHttpWarn;
extern bool httpDerpWarned;

//Настройки
extern bool logReadState;
extern bool enableDeprFeatures;
extern bool enableIndevFeatures;
extern int langCode;

//Хранилище последних открытых файлов(коммит SilverWolf2k20)
#define NUMBER_OF_FILES	4	//Строки
#define FILENAME_SIZE	256	//Столбцы
extern char** lastFiles;
void newRecentFilesArray();
void deleteOnExit();
void deleteName(uint8_t pos);
void setName(uint8_t pos, char*);
#endif
