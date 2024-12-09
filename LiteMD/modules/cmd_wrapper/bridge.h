#pragma once
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0501
#include <SDKDDKVer.h>
///////////////////////////////////////////////
//2023 Anrej0705
//For absolutely free use!
//See me at github.com/anrej0705
///////////////////////////////////////////////
#ifndef _BRIDGE_H_
#define _BRIDGE_H_
#include <string>
#include "windows.h"
#include "gui.h"
#include "stdint.h"
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <ios>
#include <conio.h>
#include <iomanip>

enum itemNames
{
	ITEM_BASE		= 0x00,		//Фон окна
	ITEM_FRAME		= 0x01,		//Рамка окна
	ITEM_TABLE		= 0x02,		//Таблица
	ITEM_TEXT		= 0x03,		//Текст
	ITEM_LINE		= 0x04		//Графическая линия
};
enum itemSettings
{
	TEXT					= 0x00,	//Меняем только текст
	BACK					= 0x01,	//Меняем только фон
	BOTH					= 0x02,	//Меняем оба
	SINGLE_LINE				= 0x03,	//Одна линия, символы "─", "┐", "│", "┘", "└", "┌"(CP866)
	DOUBLE_LINE				= 0x04,	//Двойная линия, символы "═", "╗", "║", "╝", "╚", "╔"(CP866)
	HIDDEN_LINE				= 0x05,	//Скрытая линия
	FRAME_ENABLE			= 0x06,	//Рамка включена
	FRAME_DISABLE			= 0x07,	//Рамка отключена
	F_HEADER_ENABLE			= 0x08,	//Заголовок рамки включен
	F_HEADER_DISABLE		= 0x09,	//Заголовок рамки выключен
	F_CONTROLS_ENABLE		= 0x0A,	//Кнопки управления(_,O,X) включены
	F_CONTROLS_DISABLE		= 0x0B,	//Кнопки управления(_,O,X) выключены
	F_CONNECT_DISABLE		= 0x0C,	//Стыковка рамок отключена, применимо к таблице
	F_CONNECT_SIMPLE		= 0x0D,	//Упрощённая стыковка(только углы), применимо к таблице
	F_CONNECT_ADVANCED		= 0x0E,	//Стыковка с любой точкой поверхности, применимо к таблице
	T_ENABLE_SCROLL			= 0x0F,	//Включает отрисовку и функционал скроллера
	T_DISABLE_SCROLL		= 0x10,	//Выключает отрисовку и функционал скроллера
	T_BUFFER				= 0x11,	//Пишет в буфер
	T_DIRECT				= 0x12,	//Пишет напрямую(вызывает метод draw()) !!!!!!(НЕ РЕАЛИЗОВАНО)!!!!!!!
	ALIGN_LEFT				= 0x13,	//Выравнивание по левому краю !!!!!!(НЕ РЕАЛИЗОВАНО)!!!!!!!
	ALIGN_CENTER			= 0x14,	//Выравнивание по центру
	ALIGN_RIGHT				= 0x15,	//Выравнивание по правому краю !!!!!!(НЕ РЕАЛИЗОВАНО)!!!!!!!
	L_INVERT_ENABLE			= 0x16,	//Инвертирует цвета сканируемой строки
	L_INVERT_DISABLE		= 0x17,	//Без инвертации
	F_SCROLL_ENABLE			= 0x18,	//Включает отображение полосы прокрутки
	F_SCROLL_DISABLE		= 0x19,	//Выключает полосу прокрутки
	F_SHADOW_ENABLE			= 0x1A,	//Включает отрисовку тени окошка
	F_SHADOW_DISABLE		= 0x1B,	//Выключает тень
	SINGLE_SEPARATOR		= 0x1C,	//"│" CP866
	DOUBLE_SEPARATOR		= 0x1D,	//"║" CP866
	THICK_SEPARATOR			= 0x1E,	//"█" CP866
	NO_SEPARATOR			= 0x1F,	//Без разделителей RAZ`EBAL
	CLEAR_LINE_DISABLE		= 0xED,	//Выкл очистку строки
	CLEAR_LINE_ENABLE		= 0xEE, //Вкл очистку строки
	COORD_DISABLE			= 0xEF,	//Отключение привязки к координатам(первый аргумент в addItem)
	COORD_ENABLE			= 0xF0, //Задание координат текста(первый аргумент в addItem)(синтаксис[COORD_ENABLE, {int16_t X, int16_t Y}])
	DRAW_EDITED				= 0xF1, //Отрисовка элементов с флагом "изменён"
	DRAW_GLOBAL				= 0xF2,	//Глобальная отрисовка
	DRAW_LOCAL				= 0xF3,	//Локальная отрисовка(синтаксис [DRAW_LOCAL, startPriopity(uint8_t])
	DRAW_TARGET				= 0xF4,	//Целевая отрисовка(синтаксис [DRAW_TARGET, targetItemId(uint8_t])
	GET_HSEP_INFO			= 0xF5, //Получение координат разделителя по горизонтали(синтаксис [GET_HSEP_INFO, info(uint8_t)])
	LINE_AUTO				= 0xF6, //Автоматическая вставка !!!!!!(НЕ РЕАЛИЗОВАНО)!!!!!!!
	TABLE_LINE				= 0xF7,	//Строка таблицы(синтаксис [TABLE_LINE, lineNo(uint8_t), "абв"(string), "где"(string), ..., "икл"(string), $])
	ITEM_NAME				= 0xF8,	//Пользовательское имя сущности(синтаксис [ITEM_NAME, "текст вопрос"])
	ROWS_CONFIG				= 0xF9,	//Настройка столбцов(ссылка на таблицу с шириной, синтаксис [ROWS_CONFIG, COORD* tablePtr])
	H_SEPARATOR				= 0xFA, //Горизонтальный разделитель(синтаксис [H_SEPARATOR, DOUBLE_LINE, 2])
	TABLE_ROW				= 0xFB,	//Параметр столбца таблицы - расстояние от левого края до столбца(синтаксис [TABLE_ROW, SINGLE_LINE, 14(int16_t)])
	HEADER_TEXT				= 0xFC,	//Текст заголовка окна
	TABLE_TEXT				= 0xFD,	//Текст на печать в таблицу(синтаксис [TABLE_TEXT, TABLE_ID, "ячейка 1"(char*), TABLE_TEXT, TABLE_ID, "ячейка 2"(char*)])
	BODY_TEXT				= 0xFE,	//Текст на печать в тело окна
	S_NULL					= 0xFF	//Затычка
};
using namespace std;

class shadowCache
{
	private:
		COORD cacheFrameStart;
		COORD cacheFrameSize;
		uint8_t modeByte;
		uint8_t cacheBuffer[64][256];	//Кэш окна
		uint8_t atrCacheBuf[64][256];	//Кэш аттрибутов символов окна
		uint8_t frameCache[64][256];	//Кэш области под окном
		uint8_t atrCacheFr[64][256];	//Кэш аттрибутов символов фона
		bool screenshot;
	public:
		shadowCache();
		shadowCache(COORD startFrame, COORD sizeFrame);
		shadowCache(CMDcolor tColor, CMDcolor bColo, COORD startFrame, COORD sizeFrame);
		~shadowCache();
		void setCacheCoord(COORD start, COORD size);
		void scan();	//Заносит в кеш содержимое под окном
		void hide();	//Заносит в кеш содержимое окна и убирает его
		void show();	//Рисует окно из кеша
};

class frameBuilder
{
	private:
		stringstream logCout;
		string itemUserName;
		CmdWinGUI* guiCollection[4096];	//UPD 11.10.2023 21:33 количество элементов коллекции увеличено до 4096
		list<CmdWinGUI*> guiCollectionPtr;
		uint16_t colCur;
		static uint8_t itemId;
		int16_t *rTable;	//UPD 11.10.2023 21:3 количество элементов коллекции увеличено до 4096
		//int16_t rTable[4096];	//UPD 11.10.2023 21:3 количество элементов коллекции увеличено до 4096
	public:
		frameBuilder();
		~frameBuilder();
		void addItem(itemNames newItem, CMDcolor backColor, CMDcolor textColor, COORD startCoord, COORD sizeCoord, ...);
		void remove_item_back();
		void clearCollection();
		void render(itemSettings itSet, ...);
		void heapSort();
		void getHeapInfo();
		void printItemText(uint16_t itemId, char* input, itemSettings itSet);
		void printItemHeader(uint16_t itemId, char* input, itemSettings itSet);
		void printTabLine(uint16_t itemId, uint8_t lineNo, char* str);
		void lineCtrReset(uint16_t itemId);
		void lineErase(uint16_t itemId, uint16_t tableId, uint8_t lineNo);	//itemId - указатель на ИД текстовой линии, tableId - указатель на ИД таблицы, lineNo - указатель но номер строки
		void itemTextErase(uint16_t itemId);
		void itemHeaderErase(uint16_t itemId);
		void setFrame(uint16_t itemId, itemSettings itSet, ...);
		COORD getTextPos(uint16_t itemId);
		COORD getTextSize(uint16_t itemId);
		int16_t *getRowConfig(uint16_t itemId);
		uint8_t getHsepShift(uint16_t itemId);
		uint16_t searchByName(string targetName);
};


string cpTranslate(char *input, int srcCp);

#endif // ! _BRIDGE_H_

