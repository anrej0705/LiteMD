#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0501
#include <SDKDDKVer.h>
///////////////////////////////////////////////
//2023 Anrej0705
//For absolutely free use!
//See me at github.com/anrej0705
//ГЕТСУГА ТЕНШОУ
///////////////////////////////////////////////
#include "gui.h"
#include "bridge.h"
#include <ios>
#include <iomanip>
#include <cstdio>
#include <string>
#include <stdint.h>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <stack>
#include <iomanip>
#include <stdarg.h>

using namespace std;

string dummyHeader = "NULL";
unsigned char frameStylesSet[2][9]=
{
	{0xC4,0xB3,0xDA,0xBF,0xC0,0xD9,0xB4,0xC3,0xB3},
	{0xCD,0xBA,0xC9,0xBB,0xC8,0xBC,0xB5,0xC6,0xB3}
};
unsigned char collisionStyleSet[2][15]=
{
	{0xC1, 0xC2, 0xB4, 0xC3, 0xC5, 0xD7, 0xC7, 0xB6, 0xD1, 0xD0, 0xC1, 0xCF, 0xC2, 0xC6, 0xB5},
	{0xCA, 0xCB, 0xB9, 0xCC, 0xCE, 0xD8, 0xC6, 0xB5, 0xD2, 0xCD, 0xD0, 0xCA, 0xCB, 0xC6, 0xB5}
};

HWND hwnd;
HANDLE hCon;
HANDLE hInput;
CONSOLE_SCREEN_BUFFER_INFO csbi;
uint8_t CmdWinGUI::itemCnt = 255;

CmdWinGUI::CmdWinGUI(void)
{
	rectShapeMain = { 0,0 };
	fMod = BTH;
	textColor = WHITE;
	backColor = BLACK;
	modeByte = 0;
	++itemCnt;
	itemId = itemCnt;
	hiddenItem = 0;
	//cout << cpTranslate("Создан объект с ИД=", 1251) << setw(4) << setfill('0') << to_string(itemId);
}
CmdWinGUI::CmdWinGUI(int16_t cX, int16_t cY, fillMode mode, CMDcolor textFill, CMDcolor backFill)
{
	rectShapeMain.X = cX;
	rectShapeMain.Y = cY;
	fMod = mode;
	textColor = textFill;
	backColor = backFill;
	modeByte = 0;
	//cout << "test" << endl;
	//printf("0x%02X", modeByte);
	cout << endl;
	switch (mode)
	{
		case TXT:
		{
			modeByte |= textColor;
			break;
		}
		case BCK:
		{
			modeByte |= backColor << 4;
			break;
		}
		case BTH:
		{
			modeByte |= textColor;
			modeByte |= backColor << 4;
			break;
		}
	}

	//cout << "test" << endl;
	//printf("0x%02X", modeByte);
	cout << endl;
	++itemCnt;
	hiddenItem = 0;
	//cout << cpTranslate("Создан объект с ИД=", 1251) << setw(4) << setfill('0') << to_string(itemId);
}
CmdWinGUI::~CmdWinGUI()
{
	--itemCnt;
}
void CmdWinGUI::setFrame(renderParams state, ...){return;}
void CmdWinGUI::setStyle(frameStyle style) { return; }
void CmdWinGUI::setHeader(string headerName) { return; }
void CmdWinGUI::setRows(rowSeparators separ, int16_t length, ...) { return; }
void CmdWinGUI::setHorizontalSeparator(int16_t yCoord, frameStyle fs) { return; }
void CmdWinGUI::setTableRows(int16_t *table) { return; }
void CmdWinGUI::printText(string input) { return; }
void CmdWinGUI::printTable(string input, uint8_t position) { return; }
void CmdWinGUI::setAlign(renderParams pAlign) { return; }
void CmdWinGUI::scanLine(renderParams invertEnable) { return; }
uint8_t CmdWinGUI::getRowCount() { return 0; }
void CmdWinGUI::hide(bool enable) { return; }
void CmdWinGUI::draw()
{
	/*if (dynamic_cast<CmdWinGUI*>(this))	//Проверяем из какого класса был вызван виртуальный метод
	{
		cout << "Вызван метод " << typeid(this->draw()).name()
			<< " " << __FUNCTION__ << " из " << typeid(*this).name()
			<< " файл " << __FILE__ << " строка " << __LINE__ - 2 << endl;;
	}*/
	switch (fMod)
	{
		case TXT:
		{
			SetConsoleTextAttribute(hCon, modeByte);
			//cout << "textCol" << endl;
			break;
		}
		case BCK:
		{
			SetConsoleTextAttribute(hCon, modeByte);
			//cout << "backCol" << endl;
			break;
		}
		case BTH:
		{
			SetConsoleTextAttribute(hCon, modeByte);
			//cout << "textCol & backCol" << endl;
			break;
		}
	}
}
void CmdWinGUI::setFillMode(fillMode fiMode) { fMod = fiMode; }
uint8_t CmdWinGUI::getPriority()
{
	return renderPriority;
}
COORD CmdWinGUI::getStartPos(){return { 0,0 };}
COORD CmdWinGUI::getTextPos() {return {0, 0};}
COORD CmdWinGUI::getTextSize() { return { 0, 0 }; }
int16_t *CmdWinGUI::getRowSize() { return 0; }
int16_t **CmdWinGUI::getRTptr() { return NULL; }
uint8_t CmdWinGUI::getItemId()
{
	return itemId;
}
void CmdWinGUI::setName(string name){itmName = name;}
string CmdWinGUI::getItemName(){return itmName;}
uint8_t CmdWinGUI::getLinesCount() { return 0; }
COORD CmdWinGUI::getLinesStart() { return { 0,0 }; }
void CmdWinGUI::placeLine(uint8_t lineNo, string userInput, uint8_t position){}
uint8_t CmdWinGUI::getHsepShift() { return 0; }
void CmdWinGUI::setLineShift(uint8_t shiftN) {}
void CmdWinGUI::filledRowsReset() {}
void CmdWinGUI::editFlagSwitch()
{
	isChanged=!isChanged;
}
void CmdWinGUI::setHideFlag(bool input)
{
	hiddenItem = input;
}
bool CmdWinGUI::getEditFlagStatus()
{
	return isChanged;
}
bool CmdWinGUI::getHideFlagStatus()
{
	return hiddenItem;
}
uint8_t *CmdWinGUI::readLineBuffer() { return NULL; }
uint8_t CmdWinGUI::getLineSize(uint8_t lineNo) { return 0; }
void CmdWinGUI::eraseLine(uint8_t lineNo, uint8_t position){}
void CmdWinGUI::setTextCoord(bool coordEn, int16_t X, int16_t Y) {}
void CmdWinGUI::setClearLine(bool input) {}


mainWindow::mainWindow(void)
{
	rectShapeMain = { 0,0 };
	fMod = BTH;
	textColor = BLACK;
	backColor = BLACK;
	modeByte = 0;
	renderPriority = 0; //Для фона всегда самый высокий
	//++itemId;
	//cout << cpTranslate("Создан объект с ИД=", 1251) << setw(4) << setfill('0') << itemId;
	//cout << cpTranslate(" с приоритетом ",1251) << to_string(renderPriority) << endl;
}
mainWindow::mainWindow(CMDcolor inputColor)
{
	rectShapeMain = { 0,0 };
	fMod = BCK;
	textColor = BLACK;
	backColor = inputColor;
	modeByte |= 0x0F;
	modeByte |= inputColor << 4;
	renderPriority = 0; //Для фона всегда самый высокий
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
	//--itemId;
}
mainWindow::~mainWindow()
{
	CmdWinGUI::~CmdWinGUI();
}
void mainWindow::setFrame(renderParams state, ...){return;}
void mainWindow::setStyle(frameStyle style) { return; }
void mainWindow::setHeader(string headerName) { return; }
void mainWindow::setRows(rowSeparators separ, int16_t length, ...) { return; }
void mainWindow::setHorizontalSeparator(int16_t yCoord, frameStyle fs) { return; }
void mainWindow::setTableRows(int16_t *table) { return; }
void mainWindow::printText(string input) { return; }
void mainWindow::printTable(string input, uint8_t position) { return; }
void mainWindow::setAlign(renderParams pAlign) { return; }
void mainWindow::scanLine(renderParams invertEnable) { return; }
uint8_t mainWindow::getRowCount() { return 0; }
void mainWindow::hide(bool enable) { return; }
void mainWindow::draw()
{
	COORD temp = { 0, 0 };
	COORD start = { 0, 0 };
	CmdWinGUI::draw();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	sizeWindow.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	sizeWindow.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	SetConsoleCursorPosition(hCon, start);
	//Заполнение
	for (SHORT b = 0;b < sizeWindow.Y;++b)
	{
		for (SHORT a = 0;a < sizeWindow.X;++a)
		{
			printf("%c", 0x00);
		}
		++temp.Y;
		SetConsoleCursorPosition(hCon, temp);
	}
	SetConsoleCursorPosition(hCon, start);
	if(isChanged)
		this->editFlagSwitch();
	//cout << "columns=" << sizeWindow.Y << " rows=" << sizeWindow.X << endl;
	//cout << "test main window frame" << endl;
}
void mainWindow::setFillMode(fillMode fiMode) { fMod = fiMode; }
uint8_t mainWindow::getPriority()
{
	return renderPriority;
}
COORD mainWindow::getStartPos(){return { 0,0 };}
COORD mainWindow::getTextPos() { return { 0, 0 }; }
COORD mainWindow::getTextSize() { return { 0, 0 }; }
int16_t *mainWindow::getRowSize() { return 0; }
int16_t **mainWindow::getRTptr() { return NULL; }
uint8_t mainWindow::getItemId()
{
	return itemId;
}
void mainWindow::setName(string name) { itmName = name; }
string mainWindow::getItemName() { return itmName; }
uint8_t mainWindow::getLinesCount() { return 0; }
COORD mainWindow::getLinesStart() { return { 0,0 }; }
void mainWindow::placeLine(uint8_t lineNo, string userInput, uint8_t position){}
uint8_t mainWindow::getHsepShift() { return 0; }
void mainWindow::setLineShift(uint8_t shiftN) {}
void mainWindow::filledRowsReset() {}
void mainWindow::editFlagSwitch()
{
	isChanged = !isChanged;
}
void mainWindow::setHideFlag(bool input)
{
	hiddenItem = input;
}
bool mainWindow::getEditFlagStatus()
{
	return isChanged;
}
bool mainWindow::getHideFlagStatus()
{
	return hiddenItem;
}
uint8_t *mainWindow::readLineBuffer() { return NULL; }
uint8_t mainWindow::getLineSize(uint8_t lineNo) { return 0; }
void mainWindow::eraseLine(uint8_t lineNo, uint8_t position) {}
void mainWindow::setTextCoord(bool coordEn, int16_t X, int16_t Y) {}
void mainWindow::setClearLine(bool input) {}



winFrame::winFrame(void)
{
	cachedYatr = new int16_t[64];
	cachedXatr = new int16_t[256];
	frameHeader = new char[40];
	rectShapeMain = { 0,0 };
	fMod = BTH;
	textColor = BLACK;
	backColor = BLACK;
	modeByte = 0;
	frameShadow = 0;
	newLayer = 0;
	renderPriority = 1;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
}
winFrame::winFrame(CMDcolor inputBackCol, CMDcolor inputFrameColor, COORD inputSizeStart, COORD inputSizeEnd)
{
	cachedYatr = new int16_t[64];
	cachedXatr = new int16_t[256];
	frameHeader = new char[40];
	memset(frameHeader, NULL, 40);
	hCollision = 0;
	vCollision = 0;
	modeByte |= inputFrameColor;
	modeByte |= inputBackCol << 4;
	frameSizeStart = inputSizeStart;
	frameSizeEnd = inputSizeEnd;
	fStyle = SINGLE_LIN;
	frameEnable = 0;
	headerEnable = 0;
	controlEnable = 0;
	frameShadow = 0;
	newLayer = 0;
	renderPriority = 1;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
	std::copy(dummyHeader.begin(), dummyHeader.end(), frameHeader);
}
winFrame::~winFrame()
{
	delete[] cachedYatr;
	delete[] cachedXatr;
	delete[] frameHeader;
	CmdWinGUI::~CmdWinGUI();
}
void winFrame::setFrame(renderParams state, ...)
{
	renderParams *P = &state;
	while (*P<0xFF)
	{
		switch (*P)
		{
			case FRAME_EN:
			{
				//cout << "Frame enable" << endl;
				frameEnable = 1;
				break;
			}
			case FRAME_DIS:
			{
				//cout << "Frame disable" << endl;
				frameEnable = 0;
				break;
			}
			case F_HEADER_EN:
			{
				//cout << "Frame header enable" << endl;
				headerEnable = 1;
				break;
			}
			case F_HEADER_DIS:
			{
				//cout << "Frame header disable" << endl;
				headerEnable = 0;
				break;
			}
			case F_CONTROLS_EN:
			{
				//cout << "Frame control enable" << endl;
				controlEnable = 1;
				break;
			}
			case F_CONTROLS_DIS:
			{
				//cout << "Frame control disable" << endl;
				controlEnable = 0;
				break;
			}
			case F_SHADOW_EN:
			{
				//cout << "Frame shadow enable" << endl;
				frameShadow = 1;
				break;
			}
			case F_SHADOW_DIS:
			{
				//cout << "Frame shadow disable" << endl;
				frameShadow = 0;
				break;
			}
		}
		++P;
	}
	//cout << endl;
	isChanged = 1;
}
void winFrame::setStyle(frameStyle style)
{
	switch(style)
	{
		case SINGLE_LIN:
		{
			//cout << "Set single line border" << endl;
			fStyle = style;
			break;
		}
		case DOUBLE_LIN:
		{
			//cout << "Set double line border" << endl;
			fStyle = style;
			break;
		}
	}
	isChanged = 1;
}
void winFrame::setHeader(string headerName)
{
	memset(frameHeader, 0x00, 40);
	string temp(headerName);
	std::copy(temp.begin(), temp.end(), frameHeader);
	isChanged = 1;
}
void winFrame::setRows(rowSeparators separ, int16_t length, ...) { return; }
void winFrame::setHorizontalSeparator(int16_t yCoord, frameStyle fs) { return; }
void winFrame::setTableRows(int16_t *table) { return; }
void winFrame::printText(string input) { return; }
void winFrame::printTable(string input, uint8_t position) { return; }
void winFrame::setAlign(renderParams pAlign) { return; }
void winFrame::scanLine(renderParams invertEnable) { return; }
uint8_t winFrame::getRowCount() { return 0; }
void winFrame::hide(bool enable) { return; }
void winFrame::draw()
{
	SHORT headerWidth = 0;
	COORD temp = { 0,0 };
	COORD frameStart = { 0,0 };
	COORD frameEnd = { 0,0 };
	TCHAR symbolBuffer;
	DWORD dwRead;
	uint8_t toChar;
	frameStart = frameSizeStart;
	frameEnd = frameSizeEnd;
	temp.X = frameSizeStart.X;
	temp.Y = frameSizeStart.Y;
	headerWidth = (SHORT)strlen(frameHeader)+1;
	CmdWinGUI::draw();
	SetConsoleCursorPosition(hCon, frameSizeStart);
	for (SHORT b = 0;b < frameSizeEnd.Y; ++b)
	{
		for (SHORT a = 0;a < frameSizeEnd.X;++a)
		{
			printf("%c", 0x00);
		}
		++temp.Y;
		SetConsoleCursorPosition(hCon, temp);
	}
	SetConsoleCursorPosition(hCon, frameSizeStart);

	frameStart = frameSizeStart;
	frameEnd.X = frameSizeStart.X;
	frameEnd.Y = frameSizeEnd.Y+frameSizeStart.Y-1;
	for (SHORT a = 0;a < frameSizeEnd.X - 2;++a)
	{
		frameStart.X += 1;
		frameEnd.X += 1;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][0]);
		SetConsoleCursorPosition(hCon, frameEnd);
		printf("%c", frameStylesSet[fStyle][0]);
	}
	frameStart = frameSizeStart;
	frameEnd = frameSizeStart;
	frameEnd.X += frameSizeEnd.X - 1;
	for (SHORT a = 0;a < frameSizeEnd.Y-2;++a)
	{
		frameStart.Y += 1;
		frameEnd.Y += 1;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][1]);
		SetConsoleCursorPosition(hCon, frameEnd);
		printf("%c", frameStylesSet[fStyle][1]);
	}
	frameStart = frameSizeStart;
	frameEnd = frameSizeEnd;
	temp.X = frameSizeStart.X;
	temp.Y = frameSizeStart.Y;
	SetConsoleCursorPosition(hCon, temp);
	printf("%c", frameStylesSet[fStyle][2]);
	temp.X = frameSizeEnd.X+frameSizeStart.X-1;
	temp.Y = frameSizeStart.Y;
	SetConsoleCursorPosition(hCon, temp);
	printf("%c", frameStylesSet[fStyle][3]);
	temp.X = frameSizeStart.X;
	temp.Y = frameSizeStart.Y+frameSizeEnd.Y-1;
	SetConsoleCursorPosition(hCon, temp);
	printf("%c", frameStylesSet[fStyle][4]);
	temp.X = frameSizeStart.X+frameSizeEnd.X-1;
	temp.Y = frameSizeStart.Y + frameSizeEnd.Y - 1;
	SetConsoleCursorPosition(hCon, temp);
	printf("%c", frameStylesSet[fStyle][5]);

	if (headerEnable)
	{
		frameStart = frameSizeStart;
		frameStart.X += 2;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][6]);
		cout << frameHeader;
		frameStart.X += headerWidth;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][7]);
	}
	if (controlEnable)
	{
		frameStart = frameSizeStart;
		frameStart.X += frameEnd.X-2;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][7]);
		--frameStart.X;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("X");
		--frameStart.X;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][8]);
		--frameStart.X;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("O");
		--frameStart.X;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][8]);
		--frameStart.X;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("_");
		--frameStart.X;
		SetConsoleCursorPosition(hCon, frameStart);
		printf("%c", frameStylesSet[fStyle][6]);
	}
	
	if (!newLayer)
	{
		//Проверяем левый верхний угол на наличие соседних элементов окна
		//если они есть - заменяем символ для "присоединения" окна сохраняя
		//стиль обоих окон
		SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
		GetConsoleScreenBufferInfo(hCon, &csbi);
		temp.X = csbi.dwCursorPosition.X;
		temp.Y = csbi.dwCursorPosition.Y;


		//Читаем символ сверху
		ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X,frameSizeStart.Y - 1 }, &dwRead);
		toChar = (uint8_t)symbolBuffer;
		if (toChar == frameStylesSet[fStyle][1])
			vCollision = 1;

		//Читаем символ сбоку
		ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X - 1,frameSizeStart.Y }, &dwRead);
		toChar = (uint8_t)symbolBuffer;
		if (toChar == frameStylesSet[fStyle][0])
			hCollision = 1;

		if (!hCollision&vCollision)
		{
			//cout << " v only" << endl;
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			printf("%c", collisionStyleSet[fStyle][3]);
			SetConsoleCursorPosition(hCon, { (frameSizeStart.X + frameSizeEnd.X - 1),frameSizeStart.Y });
			printf("%c", collisionStyleSet[fStyle][2]);
		}
		if (hCollision & !vCollision)
		{
			//cout << " h only" << endl;
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			printf("%c", collisionStyleSet[fStyle][1]);
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,(frameSizeStart.Y + frameSizeEnd.Y - 1) });
			printf("%c", collisionStyleSet[fStyle][0]);
		}
		if (hCollision&vCollision)
		{
			hCollision = 0;
			vCollision = 0;
			//При обнаружении коллизии по двум плоскостям, используем дополнительные проверки
			//cout << " h and v" << endl;
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			printf("%c", collisionStyleSet[fStyle][4]);

			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X + frameSizeEnd.X,frameSizeStart.Y }, &dwRead);
			toChar = (uint8_t)symbolBuffer;
			if (toChar == frameStylesSet[fStyle][0])
				hCollision = 1;
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X + frameSizeEnd.X - 1,frameSizeStart.Y - 1 }, &dwRead);
			toChar = (uint8_t)symbolBuffer;
			if (toChar == frameStylesSet[fStyle][1])
				vCollision = 1;
			if (!hCollision&vCollision)
			{
				//cout << "v-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X + frameSizeEnd.X - 1,frameSizeStart.Y });
				printf("%c", collisionStyleSet[fStyle][2]);
			}
			if (hCollision & !vCollision)
			{
				//cout << "h-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X + frameSizeEnd.X - 1,frameSizeStart.Y });
				printf("%c", collisionStyleSet[fStyle][1]);
			}
			if (hCollision&vCollision)
			{
				//cout << "hv-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X + frameSizeEnd.X - 1,frameSizeStart.Y });
				printf("%c", collisionStyleSet[fStyle][4]);
			}
			hCollision = 0;
			vCollision = 0;

			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X - 1,frameSizeStart.Y + frameSizeEnd.Y - 1 }, &dwRead);
			toChar = (uint8_t)symbolBuffer;
			if (toChar == frameStylesSet[fStyle][0])
				hCollision = 1;
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X,frameSizeStart.Y + frameSizeEnd.Y }, &dwRead);
			toChar = (uint8_t)symbolBuffer;
			if (toChar == frameStylesSet[fStyle][1])
				vCollision = 1;
			if (!hCollision&vCollision)
			{
				//cout << "v-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y + frameSizeEnd.Y - 1 });
				printf("%c", collisionStyleSet[fStyle][2]);
			}
			if (hCollision & !vCollision)
			{
				//cout << "h-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y + frameSizeEnd.Y - 1 });
				printf("%c", collisionStyleSet[fStyle][0]);
			}
			if (hCollision&vCollision)
			{
				//cout << "hv-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y + frameSizeEnd.Y - 1 });
				printf("%c", collisionStyleSet[fStyle][4]);
			}
			hCollision = 0;
			vCollision = 0;

			//SetConsoleCursorPosition(hCon, { frameSizeStart.X + frameSizeEnd.X - 1,frameSizeStart.Y + frameSizeEnd.Y - 1 });
			//printf("%c", collisionStyleSet[fStyle][4]);
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X + frameSizeEnd.X,frameSizeStart.Y + frameSizeEnd.Y - 1 }, &dwRead);
			toChar = (uint8_t)symbolBuffer;
			if (toChar == frameStylesSet[fStyle][0])
				hCollision = 1;
			SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y });
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X + frameSizeEnd.X - 1,frameSizeStart.Y + frameSizeEnd.Y }, &dwRead);
			toChar = (uint8_t)symbolBuffer;
			if (toChar == frameStylesSet[fStyle][1])
				vCollision = 1;
			if (!hCollision&vCollision)
			{
				//cout << "v-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y + frameSizeEnd.Y - 1 });
				printf("%c", collisionStyleSet[fStyle][2]);
			}
			if (hCollision & !vCollision)
			{
				//cout << "h-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y + frameSizeEnd.Y - 1 });
				printf("%c", collisionStyleSet[fStyle][0]);
			}
			if (hCollision&vCollision)
			{
				//cout << "hv-c" << endl;
				SetConsoleCursorPosition(hCon, { frameSizeStart.X,frameSizeStart.Y + frameSizeEnd.Y - 1 });
				printf("%c", collisionStyleSet[fStyle][4]);
			}
			hCollision = 0;
			vCollision = 0;
		}
		//cout << "X=" << temp.X << " Y=" << temp.Y << " SYM=";
		//cout << " SYM=";
		//printf("0x%02X", toChar);
		//cout << " " << endl;
	}

	WORD attBuffer;
	uint8_t atByte=0;
	uint8_t chByte=0;
	uint8_t temp1 = 0;
	headerWidth = 1;
	if (frameShadow) //Рендер горизонтальной нижней тени
	{
		for (SHORT a = 0;a < frameSizeEnd.X;++a)
		{
			//Ставим курсор
			SetConsoleCursorPosition(hCon, { frameSizeStart.X+ headerWidth,frameSizeStart.Y + frameSizeEnd.Y });
			//Сканируем содержимое по координатам
			ReadConsoleOutputAttribute(hCon, &attBuffer, 1, { frameSizeStart.X + headerWidth,frameSizeStart.Y + frameSizeEnd.Y }, &dwRead);
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X + headerWidth,frameSizeStart.Y + frameSizeEnd.Y }, &dwRead);
			//Помещаем в контейнер
			atByte = (uint8_t)attBuffer & 0xF0;
			chByte = (uint8_t)symbolBuffer;
			//Детектор светлой заливки
			if ((atByte & 0x80) > 1)
				atByte -= 0x80;
			++headerWidth;
			SetConsoleTextAttribute(hCon, atByte);
			//Детектор тени на предыдущем(нижнем) слое
			chByte == 0xB2 ? printf("%c", 0xDB) : printf("%c", 0xB2);
		}
	}
	atByte = 0;
	headerWidth = 1;
	if (frameShadow) //Рендер вертикальной нижней тени
	{
		for (SHORT a = 0;a < frameSizeEnd.Y-1;++a)
		{
			//Ставим курсор
			SetConsoleCursorPosition(hCon, { frameSizeStart.X+frameSizeEnd.X,frameSizeStart.Y + headerWidth });
			//Сканируем содержимое по координатам
			ReadConsoleOutputAttribute(hCon, &attBuffer, 1, { frameSizeStart.X + frameSizeEnd.X,frameSizeStart.Y + headerWidth }, &dwRead);
			ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { frameSizeStart.X + frameSizeEnd.X,frameSizeStart.Y + headerWidth }, &dwRead);
			//Помещаем в контейнер
			atByte = (uint8_t)attBuffer & 0xF0;
			chByte = (uint8_t)symbolBuffer;
			//Детектор светлой заливки
			temp1 = atByte & 0x80;
			if ((atByte & 0x80) > 1)
				atByte -= 0x80;
			++headerWidth;
			SetConsoleTextAttribute(hCon, atByte);
			//Детектор тени на предыдущем(нижнем) слое
			chByte == 0xB2 ? printf("%c", 0xDB) : printf("%c", 0xB2);
		}
	}

	frameStart = frameSizeStart;
	frameStart.X += 1;
	frameStart.Y += 1;
	SetConsoleCursorPosition(hCon, frameStart);
	if (isChanged)
		this->editFlagSwitch();
	//cout << "frame start: X:" << frameSizeStart.X << " Y:" << frameSizeStart.Y << " frame size: X:" << frameSizeEnd.X << " Y:" << frameSizeEnd.Y << " " << endl;
}
void winFrame::setFillMode(fillMode fiMode) { fMod = fiMode; }
uint8_t winFrame::getPriority()
{
	return renderPriority;
}
COORD winFrame::getStartPos()
{
	return frameSizeStart;
}
COORD winFrame::getTextPos()
{
	//cout << "X=" << frameSizeStart.X << " Y=" << frameSizeStart.Y << endl;
	return { frameSizeStart.X + 1,frameSizeStart.Y + 1 };
}
COORD winFrame::getTextSize()
{
	//cout << "X=" << frameSizeEnd.X << " Y=" << frameSizeEnd.Y << endl;
	return{frameSizeEnd.X - 1, frameSizeEnd.Y - 1};
}
int16_t *winFrame::getRowSize() { return 0; }
int16_t **winFrame::getRTptr() { return NULL; }
uint8_t winFrame::getItemId()
{
	return itemId;
}
void winFrame::setName(string name) { itmName = name; }
string winFrame::getItemName() { return itmName; }
uint8_t winFrame::getLinesCount() { return 0; }
COORD winFrame::getLinesStart() { return { 0,0 }; }
void winFrame::placeLine(uint8_t lineNo, string userInput, uint8_t position) {}
uint8_t winFrame::getHsepShift() { return 0; }
void winFrame::setLineShift(uint8_t shiftN) {}
void winFrame::filledRowsReset() {}
void winFrame::editFlagSwitch()
{
	isChanged = !isChanged;
}
void winFrame::setHideFlag(bool input)
{
	hiddenItem = input;
}
bool winFrame::getEditFlagStatus()
{
	return isChanged;
}
bool winFrame::getHideFlagStatus()
{
	return hiddenItem;
}
uint8_t *winFrame::readLineBuffer() { return NULL; }
uint8_t winFrame::getLineSize(uint8_t lineNo) { return 0; }
void winFrame::eraseLine(uint8_t lineNo, uint8_t position) {}
void winFrame::setTextCoord(bool coordEn, int16_t X, int16_t Y) {}
void winFrame::setClearLine(bool input) {}



line::line(void)
{
	lineSymBuffer = new uint8_t[256];
	lineAtrBuffer = new uint8_t[256];
	lineStart = { 0,0 };
	lineEnd = { 0,0 };
	modeByte = 0x0F;
	lineStyle = SINGLE_LIN;
	lineSym = 0x00;
	scan = 0;
	renderPriority = 4;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
}
line::line(CMDcolor bColor, CMDcolor tColor, COORD start, COORD end)
{
	lineSymBuffer = new uint8_t[256];
	lineAtrBuffer = new uint8_t[256];
	modeByte |= tColor;
	modeByte |= bColor << 4;
	lineStart = start;
	lineEnd = end;
	lineSym = 0x00;
	scan = 0;
	renderPriority = 4;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
}
line::~line()
{
	delete[] lineSymBuffer;
	delete[] lineAtrBuffer;
	CmdWinGUI::~CmdWinGUI();
}
void line::setFrame(renderParams state, ...) { return; }
void line::setStyle(frameStyle style)
{
	lineStyle = style;
	isChanged = 1;
}
void line::setHeader(string headerName){return;}
void line::setRows(rowSeparators separ, int16_t length, ...) { return; }
void line::setHorizontalSeparator(int16_t yCoord, frameStyle fs) { return; }
void line::setTableRows(int16_t *table) { return; }
void line::printText(string input) { return; }
void line::printTable(string input, uint8_t position) { return; }
void line::setAlign(renderParams pAlign) { return; }
void line::scanLine(renderParams invertEnable)
{
	uint8_t _cX = 0;
	uint8_t _cY = 0;
	TCHAR symbolBuffer;
	WORD attBuffer;
	DWORD dwRead;
	uint8_t chByte;
	uint8_t atByte;
	uint8_t xShift = 0;
	switch (invertEnable)
	{
		case L_INVERT_EN:
		{
			for (uint8_t a = 0;a < lineEnd.X;++a)
			{
				//Устанавливаем курсор на координаты окна
				//SetConsoleCursorPosition(hCon, { lineStart.X + a,lineStart.Y});
				ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { lineStart.X + a,lineStart.Y }, &dwRead);
				ReadConsoleOutputAttribute(hCon, &attBuffer, 1, { lineStart.X + a,lineStart.Y}, &dwRead);
				//Преобразуем в 8 битный формат
				chByte = (uint8_t)symbolBuffer;
				atByte = (uint8_t)attBuffer;
				//chByte = ~chByte;
				atByte = ~atByte;
				//Записываем в кеш и приращимваем курсор
				lineSymBuffer[xShift] = chByte;
				lineAtrBuffer[xShift] = atByte;
				++xShift;
			}
			scan = 1;
			break;
		}
		case L_INVERT_DIS:
		{
			for (SHORT a = 0;a < lineEnd.X;++a)
			{
				//Устанавливаем курсор на координаты окна
				//SetConsoleCursorPosition(hCon, { lineStart.X + a,lineStart.Y});
				ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { lineStart.X + a,lineStart.Y }, &dwRead);
				ReadConsoleOutputAttribute(hCon, &attBuffer, 1, { lineStart.X + a,lineStart.Y }, &dwRead);
				//Преобразуем в 8 битный формат
				chByte = (uint8_t)symbolBuffer;
				atByte = (uint8_t)attBuffer;
				//chByte = ~chByte;
				//atByte = ~atByte;
				//Записываем в кеш и приращимваем курсор
				lineSymBuffer[xShift] = chByte;
				lineAtrBuffer[xShift] = atByte;
				++xShift;
			}
			scan = 1;
			break;
		}
	}
	isChanged = 1;
}
uint8_t line::getRowCount() { return 0; }
void line::hide(bool enable) { return; }
void line::draw()
{
	CmdWinGUI::draw();
	uint8_t modB = 0; //modeByte
	COORD tempSt = lineStart;
	COORD tempEn = lineEnd;
	SHORT dX = abs(lineEnd.X - lineStart.X);
	SHORT dY = abs(lineEnd.Y - lineStart.Y);
	SHORT sX = lineStart.X < lineEnd.X ? 1 : -1;
	SHORT sY = lineStart.Y < lineEnd.Y ? 1 : -1;
	SHORT dErr = dX - dY;
	SHORT dErr2 = 0;
	if (scan)
	{
		SetConsoleCursorPosition(hCon, lineStart);
		for (uint8_t a = 0;a <= lineEnd.X-lineStart.X;++a)
		{
			modB = lineAtrBuffer[a];
			SetConsoleTextAttribute(hCon, modB);
			printf("%c", lineSymBuffer[a]);
		}
		if (isChanged)
			this->editFlagSwitch();
		return;
	}
	//отрисова спизжена с инета
	for (;;)
	{
		SetConsoleCursorPosition(hCon, tempSt);
		printf("%c", lineSym);
		if (tempSt.X == tempEn.X&&tempSt.Y == tempEn.Y)
			break;
		dErr2 = dErr * 2;
		if (dErr2 > -dY)
		{
			dErr -= dY;
			tempSt.X += sX;
		}
		if (dErr2 < dX)
		{
			dErr += dX;
			tempSt.Y += sY;
		}
	}
	if (isChanged)
		this->editFlagSwitch();
}
void line::setFillMode(fillMode fiMode) { fMod = fiMode; }
uint8_t line::getPriority()
{
	return renderPriority;
}
COORD line::getStartPos()
{
	return lineStart;
}
COORD line::getTextPos() { return { 0, 0 };}
COORD line::getTextSize() { return { 0, 0 }; }
int16_t *line::getRowSize() { return 0; }
int16_t **line::getRTptr() { return NULL; }
uint8_t line::getItemId()
{
	return itemId;
}
void line::setName(string name) { itmName = name; }
string line::getItemName() { return itmName; }
uint8_t line::getLinesCount() { return 0; }
COORD line::getLinesStart() { return { 0,0 }; }
void line::placeLine(uint8_t lineNo, string userInput, uint8_t position) {}
uint8_t line::getHsepShift() { return 0; }
void line::setLineShift(uint8_t shiftN) {}
void line::filledRowsReset() {}
void line::editFlagSwitch()
{
	isChanged = !isChanged;
}
void line::setHideFlag(bool input)
{
	hiddenItem = input;
}
bool line::getEditFlagStatus()
{
	return isChanged;
}
bool line::getHideFlagStatus()
{
	return hiddenItem;
}
uint8_t *line::readLineBuffer() { return NULL; }
uint8_t line::getLineSize(uint8_t lineNo) { return 0; }
void line::eraseLine(uint8_t lineNo, uint8_t position) {}
void line::setTextCoord(bool coordEn, int16_t X, int16_t Y) {}
void line::setClearLine(bool input) {}



table::table(void)
{
	sepShift = new int16_t[64];
	separatorsList = new uint8_t[64];
	tableStart = { 0,0 };
	tableEnd = { 0,0 };
	modeByte = 0x0F;
	separatorsList[0] = SINGLE_SEP;
	arrCur = 0;
	sepYCoord = 0;
	scrollEnFlag = 0;
	renderPriority = 2;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
}
table::table(CMDcolor bColor, CMDcolor tColor, COORD start, COORD end)
{
	sepShift = new int16_t[64];
	separatorsList = new uint8_t[64];
	modeByte |= tColor;
	modeByte |= bColor << 4;
	tableStart = start;
	tableEnd = end;
	separatorsList[0] = SINGLE_SEP;
	arrCur = 0;
	sepYCoord = 0;
	scrollEnFlag = 0;
	renderPriority = 2;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
	//cout << "X=" << tableStart.X << " Y=" << tableStart.Y << endl;
	//cout << "X=" << tableEnd.X << " Y=" << tableEnd.Y << endl;
}
table::~table()
{
	delete[] sepShift;
	delete[] separatorsList;
	CmdWinGUI::~CmdWinGUI();
}
void table::setFrame(renderParams state, ...) 
{
	renderParams *P = &state;
	while (*P < 0xFF)
	{
		switch (*P)
		{
			case F_SCROLL_EN:
			{
				//cout << "Frame enable" << endl;
				scrollEnFlag = 1;
				break;
			}
			case F_SCROLL_DIS:
			{
				//cout << "Frame disable" << endl;
				scrollEnFlag = 0;
				break;
			}
		}
		++P;
	}
	isChanged = 1;
}
void table::setStyle(frameStyle style) { return; }
void table::setHeader(string headerName) { return; }
void table::setRows(rowSeparators separ, int16_t length, ...)
{
	SHORT sL;
	rowSeparators rS;
	//cout << "set table" << endl;
	va_list sepPtr;
	va_list lenPtr;
	va_start(sepPtr, separ);
	va_start(lenPtr, length);
	rS = separ;
	sL = length;
	while (rS <= 0xFF && sL <= 0xFF && rS >=0x00 && sL >=0x00)
	{
		separatorsList[arrCur] = rS;
		sepShift[arrCur]=sL;
		//cout << "set params " << separatorsList[arrCur] << " " << sepShift[arrCur] << " iteration: " << to_string(arrCur) << endl;
		++arrCur;
		va_arg(sepPtr, rowSeparators);
		va_arg(lenPtr, SHORT);
		rS = va_arg(sepPtr, rowSeparators);
		sL = va_arg(lenPtr, SHORT);
	}
	va_end(sepPtr);
	va_end(lenPtr);
	isChanged = 1;
}
void table::setHorizontalSeparator(int16_t yCoord, frameStyle fs)
{
	sepYCoord = yCoord;
	hFStyle = fs;
	//cout << "Set separator=" << yCoord << endl;
	isChanged = 1;
}
void table::setTableRows(int16_t *table) { return; }
void table::printText(string input) { return; }
void table::printTable(string input, uint8_t position) { return; }
void table::setAlign(renderParams pAlign) { return; }
void table::scanLine(renderParams invertEnable) { return; }
uint8_t table::getRowCount() { return 0; }
void table::hide(bool enable) { return; }
void table::draw()
{
	TCHAR symbolBuffer;
	DWORD dwRead;
	uint8_t toChar;
	uint8_t sepCur = 0;
	//cout << "render table";
	CmdWinGUI::draw();
	COORD nullPoint = tableStart;
	SetConsoleCursorPosition(hCon, tableStart);
	for (SHORT a = 0;a < tableEnd.Y-1;++a)
	{
		SetConsoleCursorPosition(hCon, nullPoint);
		for (SHORT b = 0;b < tableEnd.X - 1;++b)
		{
			SetConsoleCursorPosition(hCon, nullPoint);
			if (a == sepYCoord&&sepYCoord!=0)
			{
				//Коррекция стыков разделителя строк с внешней рамкой
				if (b == 0)
				{
					SetConsoleCursorPosition(hCon, { tableStart.X - 1,nullPoint.Y });
					switch (hFStyle)
					{
						case SINGLE_LIN:
						{
							ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { tableStart.X - 1,nullPoint.Y }, &dwRead);
							toChar = (uint8_t)symbolBuffer;
							switch (toChar)
							{
								case 0xB3:
								{
									printf("%c", collisionStyleSet[hFStyle][3]);
									break;
								}
								case 0xBA:
								{
									printf("%c", collisionStyleSet[hFStyle][6]);
									break;
								}
							}
							break;
						}
						case DOUBLE_LIN:
						{
							ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { tableStart.X - 1,nullPoint.Y }, &dwRead);
							toChar = (uint8_t)symbolBuffer;
							switch (toChar)
							{
								case 0xB3:
								{
									printf("%c", collisionStyleSet[hFStyle][6]);
									break;
								}
								case 0xBA:
								{
									printf("%c", collisionStyleSet[hFStyle][3]);
									break;
								}
							}
							break;
						}
					}
					SetConsoleCursorPosition(hCon, nullPoint);
				}
				if (b == tableEnd.X - 2)
				{
					//SetConsoleCursorPosition(hCon, { tableEnd.X + 1,nullPoint.Y });
					SetConsoleCursorPosition(hCon, { tableEnd.X + tableStart.X-1,nullPoint.Y });	//Хотфикс отрисовки правого края
					switch (hFStyle)
					{
						case SINGLE_LIN:
						{
							ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { tableStart.X+tableEnd.X-1,nullPoint.Y }, &dwRead);
							toChar = (uint8_t)symbolBuffer;
							switch (toChar)
							{
								case 0xB3:
								{
									printf("%c", collisionStyleSet[hFStyle][2]);
									break;
								}
								case 0xBA:
								{
									printf("%c", collisionStyleSet[hFStyle][7]);
									break;
								}
							}
							break;
						}
						case DOUBLE_LIN:
						{
							ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { tableStart.X+tableEnd.X-1,nullPoint.Y }, &dwRead);
							toChar = (uint8_t)symbolBuffer;
							switch (toChar)
							{
								case 0xB3:
								{
									printf("%c", collisionStyleSet[hFStyle][7]);
									break;
								}
								case 0xBA:
								{
									printf("%c", collisionStyleSet[hFStyle][2]);
									break;
								}
							}
							break;
						}
					}
					SetConsoleCursorPosition(hCon, nullPoint);
				}
				//Вставка разделителя а иначе коррекция стыков с разделителями колонок
				//b == sepShift[sepCur] ? printf("%c", collisionStyleSet[hFStyle][4]) : printf("%c", frameStylesSet[hFStyle][0]);
				if (b == sepShift[sepCur])
				{
					switch (hFStyle)
					{
						case SINGLE_LIN:
						{
							switch (separatorsList[sepCur])
							{
								case SINGLE_SEP:
								{
									printf("%c", collisionStyleSet[hFStyle][4]);
									break;
								}
								case DOUBLE_SEP:
								{
									printf("%c", collisionStyleSet[hFStyle][5]);
									break;
								}
							}
							break;
						}
						case DOUBLE_LIN:
						{
							switch (separatorsList[sepCur])
							{
								case SINGLE_SEP:
								{
									printf("%c", collisionStyleSet[hFStyle][5]);
									break;
								}
								case DOUBLE_SEP:
								{
									printf("%c", collisionStyleSet[hFStyle][4]);
									break;
								}
							}
							break;
						}
					}
				}
				if (b != sepShift[sepCur])
				{
					printf("%c", frameStylesSet[hFStyle][0]);
				}
			}
			if (b == sepShift[sepCur])
			{
				printf("%c", frameStylesSet[separatorsList[sepCur]][1]);	//Сначала вставляем символ
				//Затем корректируем стыки с границами колонок
				if (a == 0)
				{
					ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { nullPoint.X,tableStart.Y - 1 }, &dwRead);
					toChar = (uint8_t)symbolBuffer;
					switch (toChar)
					{
						case 0xCD:
						{
							switch (separatorsList[sepCur])
							{
								case SINGLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][8]);
									break;
								}
								case DOUBLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][12]);
									break;
								}
							}
							break;
						}
						case 0xC4:
						{
							switch (separatorsList[sepCur])
							{
								case SINGLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][12]);
									break;
								}
								case DOUBLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][8]);
									break;
								}
							}
							break;
						}
					}
				}
				if (a == tableEnd.Y - 2)
				{
					ReadConsoleOutputCharacter(hCon, &symbolBuffer, 1, { nullPoint.X,tableStart.Y+tableEnd.Y-1}, &dwRead);
					toChar = (uint8_t)symbolBuffer;
					switch (toChar)
					{
						case 0xCD:
						{
							switch (separatorsList[sepCur])
							{
								case SINGLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y + tableEnd.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][11]);
									break;
								}
								case DOUBLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y + tableEnd.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][11]);
									break;
								}
							}
							break;
						}
						case 0xC4:
						{
							switch (separatorsList[sepCur])
							{
								case SINGLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y + tableEnd.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][10]);
									break;
								}
								case DOUBLE_SEP:
								{
									SetConsoleCursorPosition(hCon, { nullPoint.X,tableStart.Y + tableEnd.Y - 1 });
									printf("%c", collisionStyleSet[separatorsList[sepCur]][10]);
									break;
								}
							}
							break;
						}
					}
				}
				++sepCur;
			}
			nullPoint.X=b+tableStart.X;
		}
		sepCur = 0;
		++nullPoint.Y;
	}
	if (scrollEnFlag)
	{
		for (uint8_t a = sepYCoord + 1;a < tableEnd.Y-1;++a)
		{
			SetConsoleCursorPosition(hCon, { tableEnd.X,tableStart.Y + a });
			printf("%c",0xB2);
		}
		SetConsoleCursorPosition(hCon, { tableEnd.X,tableStart.Y+1 + sepYCoord });
		printf("%c", 0xDB);
		SetConsoleCursorPosition(hCon, { tableEnd.X,tableStart.Y + 2 + sepYCoord });
		printf("%c", 0xDB);
		scrollerPos = { tableEnd.X,tableStart.Y + 1 + sepYCoord };
	}
	if (isChanged)
		this->editFlagSwitch();
	//cout << "X=" << tableStart.X << " Y=" << tableStart.Y << " -> X=" << tableEnd.X << " Y=" << tableEnd.Y << endl;
}
void table::setFillMode(fillMode fiMode) { fMod = fiMode; }
uint8_t table::getPriority()
{
	return renderPriority;
}
COORD table::getStartPos() { return { 0,0 }; }
COORD table::getTextPos() 
{ 
	return { tableStart.X, tableStart.Y };
}
COORD table::getTextSize() 
{ 
	return { tableEnd.X-1, tableEnd.Y-1 };
}
int16_t *table::getRowSize(){return sepShift;}
int16_t **table::getRTptr() { return NULL;}
uint8_t table::getItemId()
{
	return itemId;
}
void table::setName(string name) { itmName = name; }
string table::getItemName() { return itmName; }
uint8_t table::getLinesCount() { return arrCur+1; }
COORD table::getLinesStart() { return { 0,0 }; }
void table::placeLine(uint8_t lineNo, string userInput, uint8_t position) {}
uint8_t table::getHsepShift() 
{ 
	return (uint8_t)sepYCoord;
}
void table::setLineShift(uint8_t shiftN) {}
void table::filledRowsReset() {}
void table::editFlagSwitch()
{
	isChanged = !isChanged;
}
void table::setHideFlag(bool input)
{
	hiddenItem = input;
}
bool table::getEditFlagStatus()
{
	return isChanged;
}
bool table::getHideFlagStatus()
{
	return hiddenItem;
}
uint8_t *table::readLineBuffer() { return NULL; }
uint8_t table::getLineSize(uint8_t lineNo) { return 0; }
void table::eraseLine(uint8_t lineNo, uint8_t position) {}
void table::setTextCoord(bool coordEn, int16_t X, int16_t Y) {}
void table::setClearLine(bool input) {}



text::text(void)
{
	tableRowSize = new int16_t[64];
	textBuffer = new uint8_t[256];
	tableText = new uint8_t*[64];
	for (uint8_t a = 0;a < 64;++a)
		tableText[a] = new uint8_t[256];
	for (uint8_t a = 0;a < 64;++a)
		for (uint8_t b = 0;b < 0xFF;++b)
			tableText[a][b] = 0x00;
	memset(textBuffer, 0x00, sizeof(textBuffer));
	startCo = { 0,0 };
	sizeCo = { 0,0 };
	textCoord = { 0,0 };
	modeByte |= WHITE;
	modeByte |= BLACK << 4;
	filledRows = 0;
	lineSize = 0;
	coordText = 0;
	clearLine = 0;
	alignSetting = ALIGN_L;
	renderPriority = 3;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
	lineShift = 0;
	sepShift = 0;
}
text::text(CMDcolor bColor, CMDcolor tColor, COORD startPos, COORD endPos)
{
	tableRowSize = new int16_t[64];
	textBuffer = new uint8_t[256];
	tableText = new uint8_t*[64];
	for (uint8_t a = 0;a < 64;++a)
		tableText[a] = new uint8_t[256];
	for (uint8_t a = 0;a < 64;++a)
		for (uint8_t b = 0;b < 0xFF;++b)
			tableText[a][b] = 0x00;
	memset(textBuffer, 0x00, sizeof(textBuffer));
	textCoord = { 0,0 };
	modeByte |= tColor;
	modeByte |= bColor << 4;
	startCo = startPos;
	sizeCo = endPos;
	tableText[0][0] = 0xFF;
	filledRows = 0;
	lineSize = 0;
	coordText = 0;
	clearLine = 0;
	alignSetting = ALIGN_L;
	renderPriority = 3;
	//cout << cpTranslate(" с приоритетом ", 1251) << to_string(renderPriority) << endl;
	lineShift = 0;
	sepShift = 0;
}
text::~text()
{	//Чистим память за собой
	delete[] tableRowSize;
	for (uint8_t a = 0;a < 64;++a)
			delete[] tableText[a];
	delete[] textBuffer;
	delete[] tableText;
	CmdWinGUI::~CmdWinGUI();
}
void text::setFrame(renderParams state, ...) { return; }
void text::setStyle(frameStyle style) { return; }
void text::setHeader(string headerName) { return; }
void text::setRows(rowSeparators separ, int16_t length, ...) { return; }
void text::setHorizontalSeparator(int16_t yCoord, frameStyle fs) { return; }
void text::setTableRows(int16_t *table)
{
	uint8_t siz = 0;
	SHORT _tA = 0;
	SHORT _tB = 0;
	//cout << "Row size=" << table[0] - startCo.X << " (left collision to row separator)" <<endl;
	tableRowSize[0] = table[0]+1;
	++filledRows;
	for (uint8_t a = 1;a < 255;++a)
	{
		if (table[a] < 0x00 || table[a]>0xFF)
		{
			siz = a;
			break;
		}
		++filledRows;
		//cout << "Row size=" << table[a] - table[a-1] << endl;
		_tA = table[a];
		_tB = table[a - 1];
		tableRowSize[a] = _tA - _tB + 1;
		//cout << "Row size=" << tableRowSize[a] << endl;
	}
	//cout << "Row size=" << sizeCo.X-table[siz] << " (row separator to rigth collision)" << endl;
	tableRowSize[siz] = sizeCo.X - table[siz-1];
	//cout << "Row size=" << tableRowSize[siz] << " (row separator to rigth collision)" << endl;
	filledRows = 0;
	isChanged = 1;
}
void text::printText(string input)
{
	uint8_t a = 0;
	memset(textBuffer, 0x00, 256);
	//cout << "input text size=" << to_string(sizeof(input)) << endl;
	for (;;)
	{
		if (input[a] == NULL)
			break;
		textBuffer[a] = input[a];
		++a;
	}
	//cout << "input text size=" << to_string(a) << endl;
	tableFormat = 0;
	isChanged = 1;
}
void text::printTable(string input, uint8_t position)
{
	//cout << "load=";
	for (uint8_t a = 0;a < 0xFF;++a)
	{
		if (input[a] == NULL)
		{
			//tableRowSize[position] = a-1;
			break;
		}
		tableText[position][a] = input[a];
		//printf("%c", tableText[position][a]);
	}
	//filledRows = position;
	tableFormat = 1;
	tableLine = 0;
	++filledRows;
	isChanged = 1;
}
void text::setAlign(renderParams pAlign)
{
	alignSetting = pAlign;
	isChanged = 1;
}
void text::scanLine(renderParams invertEnable) { return; }
uint8_t text::getRowCount() 
{ 
	return filledRows;
}
void text::hide(bool enable) { return; }
void text::draw()
{
	CmdWinGUI::draw();
	COORD temp = startCo;		//Разный мусор
	COORD tempEnd = startCo;
	int16_t eraseCnt = startCo.X;
	int16_t _al = 0;
	int16_t _an = 0;
	int16_t remain = 0;
	uint8_t a = 0;
	uint8_t coordFlag = coordText;
	bool fullRow = 1;	//Флаги, не изменять
	bool alignPrepare = 0;
	if (tableLine)
	{
		temp.Y = temp.Y + lineShift + sepShift;
	}
	if (coordText)
	{
		temp = startCo;
		temp.Y = textCoord.Y;
		tempEnd = { sizeCo.X + startCo.X, sizeCo.Y };

		//Проверяем выход за предел заданной рамки
		if (textCoord.X < startCo.X)	//+1 - см коммент блок for
			textCoord.X = startCo.X;	//+1 - см коммент блок for
		if (textCoord.Y < startCo.Y)
			textCoord.Y = startCo.Y;
		if (textCoord.X > startCo.X + sizeCo.X - 3)	//2 - см коммент блок for
			textCoord.X = startCo.X + sizeCo.X - 3;	//2 - см коммент блок for
		if (textCoord.Y > startCo.Y + sizeCo.Y - 3)
			textCoord.Y = startCo.Y + sizeCo.Y - 3;

		/*for (uint8_t a = 0;a < 0xFF;++a)
		{
			if (textBuffer[a] == NULL)
			{
				textCoord.X = textCoord.X;
				//textCoord.X = textCoord.X - a;
				break;
			}
		}*/

		//SetConsoleCursorPosition(hCon, startCo);
		if (clearLine)
		{
			__asm
			{
				JMP ERASE_BLOCK
			}
		}
		COORD_BLOCK:
		SetConsoleCursorPosition(hCon, textCoord);
		for (uint8_t a = 0;a < (uint8_t)(sizeCo.X - startCo.X);++a)
		{
			if (textBuffer[a] == NULL)
			{
				if (isChanged)
					this->editFlagSwitch();
				return;
			}
			printf("%c", textBuffer[a]);
		}
	}
	if (!tableFormat)
	{
		if (alignSetting == ALIGN_C)
		{
			_al = sizeCo.X/2;
			for (uint8_t a = 0;a < 0xFF;++a)	//Измеряем длину текстового поля
			{
				if (textBuffer[a] == NULL)
					break;
				++remain;
			}
			temp.X=startCo.X+ _al-remain/2;	//Корректируем курсор в соответствии с расчётами
			if (temp.X < startCo.X)			//Если предыдущие вычисления дали ошибку - выход за предел рамки слева
				temp.X = startCo.X;			//Корректируем
		}

		//Хотфикс 09.10.2023 19:44
		//Следы предыдущей записи в блоке
		for (uint8_t a = 0;a < 0xFF;++a)
		{
			if (eraseCnt == startCo.X + sizeCo.X - 1) //Проверяем достигло ли рамки, иначе обрываем цикл
				break;
			SetConsoleCursorPosition(hCon, {eraseCnt,temp.Y});
			printf("%c", 0x20);
			eraseCnt++;
		}
		//Конец блока

		for (;;)
		{
			SetConsoleCursorPosition(hCon, temp);
			if (textBuffer[a] == NULL)
				break;
			printf("%c", textBuffer[a]);
			++temp.X;
			++a;
			if (temp.X == startCo.X + sizeCo.X - 1) //Проверяем достигло ли рамки, иначе обрываем цикл
				break;
		}
		return;
	}
	for (uint8_t b = 0;b<=filledRows;++b)
	{
		if (!fullRow)
		{
			temp.X = tempEnd.X;
		}
		tempEnd.X += tableRowSize[b] - 1;

		ERASE_BLOCK:
		//Хотфикс 09.10.2023 15:08
		//Остаток предыдущей информации после стирания строки
		eraseCnt = temp.X;
		//Хотфикс 11.10.2023 14:35
		//Вычисление координаты коллизии даёт отрицательный результат
		for (uint8_t a = 0;a < abs(tempEnd.X - temp.X) - 2;++a)
		//for (uint8_t a = 0;a < tempEnd.X - temp.X-1;++a)
		//Конец блока
		{
			SetConsoleCursorPosition(hCon, { eraseCnt,temp.Y });	//Ставим курсор в начало/середину/конец поля
			printf("%c", 0x20);
			eraseCnt++;
		}

		//Конец блока
		__asm
		{
			MOV DH, 0x01		//Копируем в регистр константу
			MOV DL, coordFlag	//Копируем в регистр флаг режима привязки
			CMP DH, DL			//Сравниваем
			JZ COORD_BLOCK		//Если рег.статус Z=1 то переходим на метку
		}

		if (alignSetting == ALIGN_C)
		{
			--temp.X;
			//Предварительные расчеты смещения
			if (!alignPrepare)
			{
				for (SHORT a = temp.X;a < 0xFF;++a)
				{
					if (a == tempEnd.X) //Проверяем достигло ли рамки, иначе обрываем цикл
					{
						--_an;
						break;		//половина длины поля таблицы
					}
					++_an;
				}
				for (uint8_t a = 0;a <= 0xFF;++a)
				{
					if (tableText[b][a] == NULL)
					//if (tableText[b][a] == NULL || a>=(tempEnd.X-temp.X))
					{
						_al = a;	//половина длины слова
						break;
					}
				}
				alignPrepare = 1;	//Подымаем флаг готовности
			}
			//Коррекция положения курсора в соответствии с результатами блока предварительных расчётов
			temp.X += abs(_an / 2) - abs(_al / 2)+1;
			if (temp.X < startCo.X)
				temp.X = startCo.X;
			_an = 0;
		}
		//++temp.X; //Коррекция выхода на 1 позицию за рамку слева

		for (;;)
		{
			SetConsoleCursorPosition(hCon, temp);	//Ставим курсор в начало/середину/конец поля
			if (tableText[b][a] == NULL)
			{
				fullRow = 0;	//При окончании поля текста до заполнения поля снимаем флаг заполнения поля
				break;
			}
			++temp.X;
			if (temp.X >= tempEnd.X) //Проверяем достигло ли рамки, иначе обрываем цикл
			{
				fullRow = 1;	//При окончании поля в таблице до завершения поля текста прекращаем вывод и выходим из цикла
				break;
			}
			printf("%c", tableText[b][a]);
			++a;
		}
		a = 0;
		alignPrepare = 0;	//Сбрасываем флаг для следующей итерации
	}
	if (isChanged)
		this->editFlagSwitch();
}
void text::setFillMode(fillMode fiMode) { fMod = fiMode; }
uint8_t text::getPriority()
{
	return renderPriority;
}
COORD text::getStartPos() { return { 0,0 }; }
COORD text::getTextPos() { return { 0, 0 }; }
COORD text::getTextSize() { return { 0, 0 }; }
int16_t *text::getRowSize() { return 0; }
int16_t **text::getRTptr()
{ 
	return NULL;
}
uint8_t text::getItemId()
{
	return itemId;
}
void text::setName(string name) { itmName = name; }
string text::getItemName() { return itmName; }
uint8_t text::getLinesCount() { return 0; }
COORD text::getLinesStart() { return { 0,0 }; }
void text::placeLine(uint8_t lineNo, string userInput, uint8_t position) 
{
	//cout << "load=";
	for (uint8_t a = 0;a < 0xFF;++a)
	{
		if (userInput[a] == NULL)
		{
			//tableRowSize[position] = a-1;
			break;
		}
		if (tableText[position][a] == NULL)
		{
			lineSize=a;
		}
		tableText[position][a] = userInput[a];
		//printf("%c", tableText[position][a]);
	}
	//filledRows = position;
	tableFormat = 1;
	tableLine = 1;
	++filledRows;
	lineShift = lineNo;
	isChanged = 1;
}
uint8_t text::getHsepShift() { return 0; }
void text::setLineShift(uint8_t shiftN) 
{
	sepShift = shiftN+1;
	isChanged = 1;
}
void text::filledRowsReset() { filledRows = 0; }
void text::editFlagSwitch()
{
	isChanged = !isChanged;
}
void text::setHideFlag(bool input)
{
	hiddenItem = input;
}
bool text::getEditFlagStatus()
{
	return isChanged;
}
bool text::getHideFlagStatus()
{
	return hiddenItem;
}
uint8_t *text::readLineBuffer() 
{ 
	return textBuffer; 
}
uint8_t text::getLineSize(uint8_t lineNo) 
{ 
	return lineSize;
}
void text::eraseLine(uint8_t lineNo, uint8_t position)
{
	uint8_t writed_size = 0;
	for(uint8_t a=0;a<0xFF;++a)
		if (tableText[position][a] == NULL)
		{
			writed_size = a;
			break;
		}
	for (uint8_t a = 0;a < writed_size;++a)
		tableText[position][a] = 0x00;
	lineShift = lineNo;
}
void text::setTextCoord(bool coordEn, int16_t X, int16_t Y)
{
	coordText = coordEn;
	textCoord.X = X;
	textCoord.Y = Y;
}
void text::setClearLine(bool input)
{
	clearLine = input;
}
//---------------------------------OTHER STUFF--------------------------------
COORD get_position()
{
	CONSOLE_SCREEN_BUFFER_INFO sB;
	COORD coordNew = { 0,0 };
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sB))
	{
		coordNew.X = sB.dwCursorPosition.X;
		coordNew.Y = sB.dwCursorPosition.Y;
		return coordNew;
	}
	return coordNew;
}
COORD get_mouseEventCoord()
{
	DWORD cmdEvents;
	COORD mCoord;
	INPUT_RECORD iRec;
	while (1)
	{
		ReadConsoleInput(hInput, &iRec, 1, &cmdEvents);
		switch (iRec.EventType)
		{
			case MOUSE_EVENT:
			{
				if (iRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					mCoord.X = iRec.Event.MouseEvent.dwMousePosition.X;
					mCoord.Y = iRec.Event.MouseEvent.dwMousePosition.Y;
					return mCoord;
				}
				break;
			}
		}
	}
	//return { 0,0 };
}
void set_pos(COORD coord)
{
	COORD newPos;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	newPos.X = coord.X;
	newPos.Y = coord.Y;
	SetConsoleCursorPosition(hConsole, coord);
}
void setupWindow()
{
	DWORD prev_mode;
	PCONSOLE_FONT_INFOEX lpCCF = new CONSOLE_FONT_INFOEX();
	string cmdHeader = cpTranslate("[В РАЗРАБОТКЕ][CMD 1320x680] DB Manager by Anrej0705[github.com/anrej0705] v0.5[ОЖИДАНИЕ]", 1251);
	LPSTR proc = const_cast<char*>(cmdHeader.c_str());
	hwnd = GetConsoleWindow();
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	hInput = GetStdHandle(STD_INPUT_HANDLE);
	//setlocale(LC_ALL, "Russian");
	//cout << "CP=" << GetConsoleOutputCP() << endl;
	//cout << "CP=" << GetConsoleCP() << endl;
	lpCCF->cbSize = sizeof(_CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hCon, 0, lpCCF);
	GetConsoleMode(hInput, &prev_mode);
	lpCCF->dwFontSize.X = 8;
	lpCCF->dwFontSize.Y = 12;
	lpCCF->FontFamily = FF_DONTCARE;
	lpCCF->FontWeight = FW_NORMAL;
	wcscpy(lpCCF->FaceName, L"Terminal");
	SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS |
		(prev_mode & ~ENABLE_QUICK_EDIT_MODE));
	SetCurrentConsoleFontEx(hCon, 0, lpCCF);
	SetConsoleCP(866);
	SetConsoleOutputCP(866);
	//cout << "CP=" << GetConsoleOutputCP() << endl;
	//cout << "CP=" << GetConsoleCP() << endl;
	//SetConsoleTitle(proc);
	if (hwnd != NULL)
	{
		MoveWindow(hwnd, 140, 140, 1320, 680, TRUE);
	}
}
//----------------------------------------------------------------------------