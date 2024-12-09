#pragma once
//#pragma message("gui.h included")
#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0501
#include <SDKDDKVer.h>
///////////////////////////////////////////////
//2023 Anrej0705
//For absolutely free use!
//See me at github.com/anrej0705
///////////////////////////////////////////////
#ifndef _GUI_H_
#define _GUI_H_

using namespace std;

#pragma once
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <stdarg.h>

enum CMDcolor
{
	BLACK				 = 0x00,		//Чёрный
	BLUE				 = 0x01,		//Синий
	GREEN				 = 0x02,		//Зелёный
	CYAN				 = 0x03,		//Бирюзовый
	RED					 = 0x04,		//Красный
	MAGENTA				 = 0x05,		//Фиолетовый
	YELLOW				 = 0x06,		//Жёлтый
	LIGHT_GRAY			 = 0x07,		//Светло-серый
	GRAY				 = 0x08,		//Серый
	LIGHT_BLUE			 = 0x09,		//Светло-синий
	LIGHT_GREEN			 = 0x0A,		//Светло-зелёный
	LIGHT_CYAN			 = 0x0B,		//Светло-бирюзовый
	LIGHT_RED			 = 0x0C,		//Светло-красный
	LIGHT_MAGENTA		 = 0x0D,		//Светло-фиолетовый
	LIGHT_YELLOW		 = 0x0E,		//Светло-жёлтый
	WHITE				 = 0x0F,		//Белый
};
enum fillMode
{
	TXT				 = 0x00,		//Меняем только текст
	BCK				 = 0x01,		//Меняем только фон
	BTH				 = 0x02,		//Меняем оба
};
enum frameStyle
{
	SINGLE_LIN			 = 0x00,		//Одна линия, символы "─", "┐", "│", "┘", "└", "┌"
	DOUBLE_LIN			 = 0x01,		//Двойная линия, символы "═", "╗", "║", "╝", "╚", "╔"
	HIDDEN_LIN			 = 0xFF,		//Скрытая линия
};
enum renderParams
{
	FRAME_EN			 = 0x01,		//Рамка включена
	FRAME_DIS			 = 0x00,		//Рамка отключена
	F_HEADER_EN			 = 0x03,		//Заголовок рамки включен
	F_HEADER_DIS		 = 0x02,		//Заголовок рамки выключен
	F_CONTROLS_EN		 = 0x05,		//Кнопки управления(_,O,X) включены
	F_CONTROLS_DIS		 = 0x04,		//Кнопки управления(_,O,X) выключены
	F_CONNECT_DIS		 = 0x06,		//Стыковка рамок отключена, применимо к таблице
	F_CONNECT_SIMPL		 = 0x07,		//Упрощённая стыковка(только углы), применимо к таблице
	F_CONNECT_ADV		 = 0x08,		//Стыковка с любой точкой поверхности, применимо к таблице
	T_ENABLE_SCRL		 = 0x09,		//Включает отрисовку и функционал скроллера
	T_DISABLE_SCRL		 = 0x0A,		//Выключает отрисовку и функционал скроллера
	T_BUFR				 = 0x0B,		//Пишет в буфер
	T_DIR				 = 0x0C,		//Пишет напрямую(вызывает метод draw())
	ALIGN_L				 = 0x0D,		//Выравнивание по левому краю
	ALIGN_C				 = 0x0E,		//Выравнивание по центру
	ALIGN_R				 = 0x0F,		//Выравнивание по правому краю
	L_INVERT_EN			 = 0x10,		//Инвертирует цвета сканируемой строки
	L_INVERT_DIS		 = 0x11,		//Без инвертации
	F_SCROLL_EN			 = 0x12,		//Включает отображение полосы прокрутки
	F_SCROLL_DIS		 = 0x13,		//Выключает полосу прокрутки
	F_SHADOW_EN			 = 0x14,		//Включает отрисовку тени окошка
	F_SHADOW_DIS		 = 0x15,		//Выключает тень
};
enum rowSeparators
{
	SINGLE_SEP			 = 0x00,		//"│"
	DOUBLE_SEP			 = 0x01,		//"║"
	THICK_SEP			 = 0xFE,		//"█"
	NO_SEP				 = 0xFF			//Без разделителей RAZ`EBAL
};
class CmdWinGUI
{
	private:
	protected:
		COORD rectShapeMain;
		CMDcolor textColor;
		CMDcolor backColor;
		uint8_t modeByte;
		fillMode fMod;
		uint8_t renderPriority;		//Приоритет отрисовки
		uint8_t itemId;				//ИД элемента
		string itmName;				//Имя элемента
		static uint8_t itemCnt;		//Счетчик элементов
		bool hiddenItem;			//Спрятанный элемент
		bool isChanged;				//Измененный элемент(1 - изменен, 0 - без изменений, см парам. DRAW_EDITED bridge.h)
	public:
		CmdWinGUI(void);
		CmdWinGUI(int16_t cX, int16_t cY, fillMode mode, CMDcolor textFill, CMDcolor backFill);
		virtual ~CmdWinGUI();
		virtual void setFrame(renderParams state, ...) = 0;
		virtual void setStyle(frameStyle style) = 0;
		virtual void setHeader(string headerName)=0;
		virtual void setRows(rowSeparators separ, SHORT length, ...)=0;
		virtual void setHorizontalSeparator(SHORT yCoord, frameStyle fs)=0;
		virtual void setTableRows(SHORT *table)=0;
		virtual void printText(string input)=0;
		virtual void printTable(string input, uint8_t position)=0;
		virtual void setAlign(renderParams pAlign) = 0;
		virtual void scanLine(renderParams invertEnable)=0;
		virtual uint8_t getRowCount() = 0;
		virtual void draw() = 0;
		virtual void hide(bool enable);
		virtual void setFillMode(fillMode fiMode) = 0;
		virtual uint8_t getPriority()=0;
		virtual COORD CmdWinGUI::getStartPos() = 0;
		virtual COORD getTextPos()=0;
		virtual COORD getTextSize()=0;
		virtual int16_t *getRowSize() = 0;
		virtual int16_t **getRTptr() = 0;
		virtual uint8_t getItemId();
		virtual void setName(string name) = 0;
		virtual string getItemName() = 0;
		virtual uint8_t getLinesCount() = 0;
		virtual COORD getLinesStart() = 0;
		virtual void placeLine(uint8_t lineNo, string userInput, uint8_t position) = 0;
		virtual uint8_t getHsepShift() = 0;
		virtual void setLineShift(uint8_t shiftN) = 0;
		virtual void filledRowsReset() = 0;
		virtual void editFlagSwitch() = 0;
		virtual void setHideFlag(bool input) = 0;
		virtual bool getEditFlagStatus() = 0;
		virtual bool getHideFlagStatus() = 0;
		virtual uint8_t *readLineBuffer() = 0;
		virtual uint8_t getLineSize(uint8_t lineNo) = 0;
		virtual void eraseLine(uint8_t lineNo, uint8_t position) = 0;
		virtual void setTextCoord(bool coordEn, int16_t X, int16_t Y) = 0;
		virtual void setClearLine(bool input) = 0;
};
class mainWindow : public CmdWinGUI
{
	private:
		COORD sizeWindow;
	public:
		mainWindow(void);
		mainWindow(CMDcolor inputColor);
		~mainWindow();
		void setFrame(renderParams state, ...);
		void setStyle(frameStyle style);
		void setHeader(string headerName);
		void setRows(rowSeparators separ, int16_t length, ...);
		void setHorizontalSeparator(int16_t yCoord, frameStyle fs);
		void setTableRows(SHORT *table);
		void printText(string input);
		void printTable(string input, uint8_t position);
		void setAlign(renderParams pAlign);
		void scanLine(renderParams invertEnable);
		uint8_t getRowCount();
		void hide(bool enable);
		void draw();
		void setFillMode(fillMode fiMode);
		uint8_t getPriority();
		COORD CmdWinGUI::getStartPos();
		COORD getTextPos();
		COORD getTextSize();
		int16_t *getRowSize();
		int16_t **getRTptr();
		uint8_t getItemId();
		void setName(string name);
		string getItemName();
		uint8_t getLinesCount();
		COORD getLinesStart();
		void placeLine(uint8_t lineNo, string userInput, uint8_t position);
		uint8_t getHsepShift();
		void setLineShift(uint8_t shiftN);
		void filledRowsReset();
		void editFlagSwitch();
		void setHideFlag(bool input);
		bool getEditFlagStatus();
		bool getHideFlagStatus();
		uint8_t *readLineBuffer();
		uint8_t getLineSize(uint8_t lineNo);
		void eraseLine(uint8_t lineNo, uint8_t position);
		void setTextCoord(bool coordEn, int16_t X, int16_t Y);
		void setClearLine(bool input);
};
class winFrame : public CmdWinGUI
{
	private:
		COORD frameSizeStart;
		COORD frameSizeEnd;
		int16_t *cachedYatr;
		int16_t *cachedXatr;
		frameStyle fStyle;
		bool frameEnable;
		bool headerEnable;
		bool controlEnable;
		bool overlap;			//флаг перекрытия, 1 - перекрыл
		bool overlapPos;		//0 - перекрыл другое, 1 - перекрыт другим
		bool newLayer;			//Новый слой
		bool hCollision;		//Обнаружение коллизии по горизонтали
		bool vCollision;		//Обнаружение коллизии по вертикали
		bool frameShadow;		//Флаг отрисовки тени у окна
		char *frameHeader;
	public:
		winFrame(void);
		winFrame(CMDcolor inputBackCol, CMDcolor inputFrameColor, COORD inputSize, COORD inputSizeEnd);
		~winFrame();
		void setFrame(renderParams state, ...);
		void setStyle(frameStyle style);
		void setHeader(string headerName);
		void setRows(rowSeparators separ, int16_t length, ...);
		void setHorizontalSeparator(int16_t yCoord, frameStyle fs);
		void setTableRows(int16_t *table);
		void printText(string input);
		void printTable(string input, uint8_t position);
		void setAlign(renderParams pAlign);
		void scanLine(renderParams invertEnable);
		uint8_t getRowCount();
		void hide(bool enable);
		void draw();
		void setFillMode(fillMode fiMode);
		uint8_t getPriority();
		COORD CmdWinGUI::getStartPos();
		COORD getTextPos();
		COORD getTextSize();
		int16_t *getRowSize();
		int16_t **getRTptr();
		uint8_t getItemId();
		void setName(string name);
		string getItemName();
		uint8_t getLinesCount();
		COORD getLinesStart();
		void placeLine(uint8_t lineNo, string userInput, uint8_t position);
		uint8_t getHsepShift();
		void setLineShift(uint8_t shiftN);
		void filledRowsReset();
		void editFlagSwitch();
		void setHideFlag(bool input);
		bool getEditFlagStatus();
		bool getHideFlagStatus();
		uint8_t *readLineBuffer();
		uint8_t getLineSize(uint8_t lineNo);
		void eraseLine(uint8_t lineNo, uint8_t position);
		void setTextCoord(bool coordEn, int16_t X, int16_t Y);
		void setClearLine(bool input);
};
class line : public CmdWinGUI
{
	private:
		COORD lineStart;
		COORD lineEnd;
		frameStyle lineStyle;
		uint8_t *lineSymBuffer;
		uint8_t *lineAtrBuffer;
		char lineSym;
		bool scan;
	public:
		line(void);
		line(CMDcolor bColor, CMDcolor tColor, COORD start, COORD end);
		~line();
		void setFrame(renderParams state, ...);
		void setStyle(frameStyle style);
		void setHeader(string headerName);
		void setRows(rowSeparators separ, int16_t length, ...);
		void setHorizontalSeparator(int16_t yCoord, frameStyle fs);
		void setTableRows(int16_t *table);
		void printText(string input);
		void printTable(string input, uint8_t position);
		void setAlign(renderParams pAlign);
		void scanLine(renderParams invertEnable);
		uint8_t getRowCount();
		void hide(bool enable);
		void draw();
		void setFillMode(fillMode fiMode);
		uint8_t getPriority();
		COORD CmdWinGUI::getStartPos();
		COORD getTextPos();
		COORD getTextSize();
		int16_t *getRowSize();
		int16_t **getRTptr();
		uint8_t getItemId();
		void setName(string name);
		string getItemName();
		uint8_t getLinesCount();
		COORD getLinesStart();
		void placeLine(uint8_t lineNo, string userInput, uint8_t position);
		uint8_t getHsepShift();
		void setLineShift(uint8_t shiftN);
		void filledRowsReset();
		void editFlagSwitch();
		void setHideFlag(bool input);
		bool getEditFlagStatus();
		bool getHideFlagStatus();
		uint8_t *readLineBuffer();
		uint8_t getLineSize(uint8_t lineNo);
		void eraseLine(uint8_t lineNo, uint8_t position);
		void setTextCoord(bool coordEn, int16_t X, int16_t Y);
		void setClearLine(bool input);
};
class table : public CmdWinGUI
{
	private:
		COORD tableStart;
		COORD tableEnd;
		COORD scrollerPos;
		int16_t *sepShift;
		int16_t sepYCoord;
		frameStyle fStyle;
		frameStyle hFStyle;
		uint8_t *separatorsList;
		uint8_t arrCur;
		bool showLineSeparator;
		bool scrollEnFlag;
	public:
		table(void);
		table(CMDcolor bColor, CMDcolor tColor, COORD start, COORD end);
		~table();
		void setFrame(renderParams state, ...);
		void setStyle(frameStyle style);
		void setHeader(string headerName);
		void setRows(rowSeparators separ, int16_t length, ...);
		void setHorizontalSeparator(int16_t yCoord, frameStyle fs);
		void setTableRows(int16_t *table);
		void printText(string input);
		void printTable(string input, uint8_t position);
		void setAlign(renderParams pAlign);
		void scanLine(renderParams invertEnable);
		uint8_t getRowCount();
		void draw();
		void hide(bool enable);
		void setFillMode(fillMode fiMode);
		uint8_t getPriority();
		COORD CmdWinGUI::getStartPos();
		COORD getTextPos();
		COORD getTextSize();
		int16_t *getRowSize();	//int16_t == SHORT
		int16_t **getRTptr();
		uint8_t getItemId();
		void setName(string name);
		string getItemName();
		uint8_t getLinesCount();
		COORD getLinesStart();
		void placeLine(uint8_t lineNo, string userInput, uint8_t position);
		uint8_t getHsepShift();
		void setLineShift(uint8_t shiftN);
		void filledRowsReset();
		void editFlagSwitch();
		void setHideFlag(bool input);
		bool getEditFlagStatus();
		bool getHideFlagStatus();
		uint8_t *readLineBuffer();
		uint8_t getLineSize(uint8_t lineNo);
		void eraseLine(uint8_t lineNo, uint8_t position);
		void setTextCoord(bool coordEn, int16_t X, int16_t Y);
		void setClearLine(bool input);
};
class text : public CmdWinGUI
{
	private:
		renderParams alignSetting;
		int16_t *tableRowSize;	//int16_t == SHORT
		uint8_t lineShift;
		uint8_t sepShift;
		uint8_t **tableText;
		uint8_t *textBuffer;
		uint8_t filledRows;
		uint8_t lineSize;
		bool tableFormat;
		bool tableLine;
		bool coordText;
		bool clearLine;
		COORD lineStart;
		COORD startCo;
		COORD sizeCo;
		COORD textCoord;
	public:
		text(void);
		text(CMDcolor bColor, CMDcolor tColor, COORD start, COORD end);
		~text();
		void setFrame(renderParams state, ...);
		void setStyle(frameStyle style);
		void setHeader(string headerName);
		void setRows(rowSeparators separ, int16_t length, ...);
		void setHorizontalSeparator(int16_t yCoord, frameStyle fs);
		void setTableRows(int16_t *table);
		void printText(string input);
		void printTable(string input, uint8_t position);
		void setAlign(renderParams pAlign);
		void scanLine(renderParams invertEnable);
		uint8_t getRowCount();
		void draw();
		void hide(bool enable);
		void setFillMode(fillMode fiMode);
		uint8_t getPriority();
		COORD CmdWinGUI::getStartPos();
		COORD getTextPos();
		COORD getTextSize();
		int16_t *getRowSize();
		int16_t **getRTptr();
		uint8_t getItemId();
		void setName(string name);
		string getItemName();
		uint8_t getLinesCount();
		COORD getLinesStart();
		void placeLine(uint8_t lineNo, string userInput, uint8_t position);
		uint8_t getHsepShift();
		void setLineShift(uint8_t shiftN);
		void filledRowsReset();
		void editFlagSwitch();
		void setHideFlag(bool input);
		bool getEditFlagStatus();
		bool getHideFlagStatus();
		uint8_t *readLineBuffer();
		uint8_t getLineSize(uint8_t lineNo);
		void eraseLine(uint8_t lineNo, uint8_t position);
		void setTextCoord(bool coordEn, int16_t X, int16_t Y);
		void setClearLine(bool input);
};

void setupWindow();
COORD get_position();
COORD get_mouseEventCoord();
void set_pos(COORD coord);

#endif
