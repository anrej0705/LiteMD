#pragma once
#ifndef GLOBAL_DEFINITIONS_H_
#define GLOBAL_DEFINITIONS_H_
//Глобальные определения, в т.ч версии сборки и ИД событий

//---------------------------------------------
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
#define BUILD_NUMBER 1006
//!!!НЕ ИЗМЕНЯЙ И НЕ ПЕРЕНОСИ ДАННУЮ СТРОЧКУ!!!
//---------------------------------------------

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
*/

#define SETTINGS_HEIGH									20

//События смены языка
#define APP_EVENT_UI_UPDATE_EVENT						33
#define APP_EVENT_LiteMD_UPDATE_EVENT					34
#define APP_EVENT_mdEditor_UPDATE_EVENT					35
#define APP_EVENT_mdScreen_UPDATE_EVENT					36
#define APP_EVENT_GuiDownloader_UPDATE_EVENT			37
#define APP_EVENT_dialogBoxes_UPDATE_EVENT				38
#define APP_EVENT_appSettings_UPDATE_EVENT				39
#define APP_EVENT_tab_basic_UPDATE_EVENT				40
//--------------------------------------------------------

#endif
