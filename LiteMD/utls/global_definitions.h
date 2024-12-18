#pragma once
#ifndef GLOBAL_DEFINITIONS_H_
#define GLOBAL_DEFINITIONS_H_
//Глобальные определения, в т.ч версии сборки и ИД событий

//---------------------------------------------
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
//##[STAGE]
#define APP_STAGE " alpha"
//##[VER]
#define APP_VERSION " 0.2.8"
//##[BUILD]
#define BUILD_NUMBER 1957
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
//---------------------------------------------

//"Сигнатура" для настроек
const char appSign[] = "__Shani_basic";

//Количество читаемых параметров из конфига. Прибавлять каждый раз когда добавляется новая фича
#define PARAM_CNT	19

/*
* Таблица событий для обновления интерфейса
* Диапазон ID 34-101
* 
* ID 33 - событие обновления интерфейса(язык поменялся!)
* 
* ___________________________
* | ID |        Цель        |
* | 34 |LiteMD.cpp          |
* | 35 |mdEditor.cpp        |
* | 36 |mdScreen.cpp        |
* | 37 |GuiDownloader.cpp   |
* | 38 |dialogBoxes.cpp     |
* | 39 |appSettings.cpp     |
* | 40 |tab_basic.cpp       |
* | 41 |frameChangelog.cpp  |
*/

//Макисмум сообещений в логе, всё что выше вызовет очистку(tab_basic.cpp)
#define LOGS_LIMIT										16384
#define LOGS_MAXIMUM									16777216

#define SETTINGS_HEIGH									20
#define HINTS_WIDTH										400
#define INTERACT_WIDTH									200

//События смены языка
#define APP_EVENT_UI_UPDATE_EVENT						33
#define APP_EVENT_LiteMD_UPDATE_EVENT					34
#define APP_EVENT_mdEditor_UPDATE_EVENT					35
#define APP_EVENT_mdScreen_UPDATE_EVENT					36
#define APP_EVENT_GuiDownloader_UPDATE_EVENT			37
#define APP_EVENT_dialogBoxes_UPDATE_EVENT				38
#define APP_EVENT_appSettings_UPDATE_EVENT				39
#define APP_EVENT_tab_basic_UPDATE_EVENT				40
#define APP_EVENT_frame_changelog_UPDATE_EVENT			41
#define APP_EVENT_logger_UPDATE_EVENT					42
#define APP_EVENT_helpCenter_UPDATE_EVENT				43
//--------------------------------------------------------

//Событие обновления настроек после чтения конфига
//Обработчик лежит в appSettings_ui.cpp
#define APP_EVENT_UI_UPDATE_USER_SETTINGS				127

/*
* Блок шаблонов для парсеров. Когда парсер встречает свой тег, он подменяет тег на
* HTML-шаблон
*/
const char simple_url_iopenurl[] = "<A HREF=\"";
const char simple_url_icloseurl[] = "\">";
const char simple_url_iclosetext[] = "</A>";

const char header_lvl_iopenlvl[] = "<H";
const char header_lvl_icloselvl[] = ">";
const char header_lvl_iclosetext[] = "</H";

const char striked_txt_iopen[] = "<s>";
const char striked_txt_iclose[] = "</s>";

const char underlined_txt_iopen[] = "<ins>";
const char underlined_txt_iclose[] = "</ins>";

const char italic_txt_iopen[] = "<I>";
const char italic_txt_iclose[] = "</I>";

const char bold_txt_iopen[] = "<B>";
const char bold_txt_iclose[] = "</B>";

const uint8_t simple_url_iopenurl_size = 9;
const uint8_t simple_url_icloseurl_size = 2;
const uint8_t simple_url_iclosetext_size = 4;

const uint8_t header_lvl_iopenlvl_size = 2;
const uint8_t header_lvl_icloselvl_size = 1;
const uint8_t header_lvl_iclosetext_size = 3;

const uint8_t striked_txt_iopen_size = 3;
const uint8_t striked_txt_iclose_size = 4;

const uint8_t underlined_txt_iopen_size = 5;
const uint8_t underlined_txt_iclose_size = 6;

const uint8_t italic_txt_iopen_size = 3;
const uint8_t italic_txt_iclose_size = 4;

const uint8_t bold_txt_iopen_size = 3;
const uint8_t bold_txt_iclose_size = 4;
#endif
