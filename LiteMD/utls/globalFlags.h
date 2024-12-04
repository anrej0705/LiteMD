﻿#pragma once
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
extern bool settingChanged;

//Настройки
extern bool logReadState;
extern bool enableDeprFeatures;
extern bool enableIndevFeatures;
extern int langCode;

//Список файлов
extern char** lastFiles;

//Настройки битов парсинга - биты по умолчанию заданы в appSettings.cpp
struct parser_switchers
{
	bool en_simple_url;
	bool en_adv_url;
	bool en_header_lvl;
	bool en_ex_strkthg;
};

struct depr_paerser_switchers
{
	bool en_t_prep;
	bool en_t_post;
	bool en_url_bas_simple;
	bool en_url_bas;
	bool en_url_adv;
};

//Хранилище последних открытых файлов(коммит SilverWolf2k20)
#define NUMBER_OF_FILES	4	//Строки
#define FILENAME_SIZE	256	//Столбцы
extern char** lastFiles;
void newRecentFilesArray();
void deleteOnExit();
void deleteName(uint8_t pos);
void setName(uint8_t pos, char*);
#endif
