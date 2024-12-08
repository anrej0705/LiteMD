#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0501
#include <SDKDDKVer.h>
///////////////////////////////////////////////
//2023 Anrej0705
//For absolutely free use!
//See me at github.com/anrej0705
//ГЕТСУГА ТЕНШОУ
///////////////////////////////////////////////
#include "windows.h"
#include "bridge.h"
#include "gui.h"
#include <string>
#include <iostream>
#include <list>
#include <ios>
#include <conio.h>
#include <iomanip>

using namespace std;
HANDLE bCon;
uint8_t frameBuilder::itemId;

uint8_t charSet1251_to_866[256] = {
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07, 0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,	// 0x00...0x0F
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,	// 0x10...0x1F
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27, 0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,	// 0x20...0x2F
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37, 0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,	// 0x30...0x3F
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47, 0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,	// 0x40...0x4F
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57, 0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,	// 0x50...0x5F
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67, 0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,	// 0x60...0x6F
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77, 0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,	// 0x70...0x7F
	0x89,0x81,0x82,0x83,0x84,0x85,0x86,0x87, 0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,	// 0x80...0x8F
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, NULL,NULL,0xDB,NULL,NULL,NULL,NULL,NULL,	// 0x90...0x9F 0x9A(Ль)->0xDB
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, 0xF0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,	// 0xA0...0xAF
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, 0xF1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,	// 0xB0...0xBF
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87, 0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,	// 0xC0...0xCF
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97, 0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,	// 0xD0...0xDF
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7, 0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,	// 0xE0...0xEF
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7, 0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,	// 0xF0...0xFF
};

shadowCache::shadowCache()
{
	modeByte = 0x0F;
}
shadowCache::shadowCache(COORD startFrame, COORD sizeFrame)
{
	stringstream logCout;
	modeByte = 0;
	cacheFrameStart = startFrame;
	cacheFrameSize = sizeFrame;
	screenshot = 0;
	logCout << "Задана область кеширования X=" << cacheFrameSize.X << " Y=" << cacheFrameSize.Y << endl;
	//putLog(logCout.str(), INFO);
	memset(cacheBuffer, 0x00, sizeof(cacheBuffer));
	memset(atrCacheBuf, 0x00, sizeof(atrCacheBuf));
	memset(frameCache, 0x00, sizeof(frameCache));
	memset(atrCacheFr, 0x00, sizeof(atrCacheFr));
}
shadowCache::shadowCache(CMDcolor bColor, CMDcolor tColor, COORD startFrame, COORD sizeFrame)
{
	modeByte = 0;
	cacheFrameStart = startFrame;
	cacheFrameSize = sizeFrame;
	modeByte |= tColor;
	modeByte |= bColor << 4;
	screenshot = 0;
	memset(cacheBuffer, 0x00, sizeof(cacheBuffer));
	memset(atrCacheBuf, 0x00, sizeof(atrCacheBuf));
	memset(frameCache, 0x00, sizeof(frameCache));
	memset(atrCacheFr, 0x00, sizeof(atrCacheFr));
}
shadowCache::~shadowCache()
{
	//delete[] &cacheBuffer;	 Я не ебу как тут правильно удалить. Пока так оставлю
	//delete cacheBuffer;
	//delete[] atrCacheBuf;
	//delete atrCacheBuf;
	//delete[] frameCache;
	//delete frameCache;
	//delete[] atrCacheFr;
	//delete atrCacheFr;
	//cout << "delete" << endl;
}
void shadowCache::scan()	//Сканирование подоконного пространства
{
	uint8_t _cX = 0;
	uint8_t _cY = 0;
	bCon = GetStdHandle(STD_OUTPUT_HANDLE);
	TCHAR symbolBuffer;
	WORD attBuffer;
	DWORD dwRead;
	uint8_t chByte;
	uint8_t atByte;
	string logOut("Область кеширована");
	stringstream cOutLog;
	SetConsoleTextAttribute(bCon, modeByte);
	for (SHORT a = 0;a < cacheFrameSize.Y+1;++a)
	{
		for (SHORT b = 0;b < cacheFrameSize.X+1;++b)
		{
			//Устанавливаем курсор на координаты окна
			SetConsoleCursorPosition(bCon, { cacheFrameStart.X + b,cacheFrameStart.Y + a });
			//Читаем символ и аттрибуты его
			ReadConsoleOutputCharacter(bCon, &symbolBuffer, 1, { cacheFrameStart.X + b,cacheFrameStart.Y + a }, &dwRead);
			ReadConsoleOutputAttribute(bCon, &attBuffer, 1, { cacheFrameStart.X + b,cacheFrameStart.Y + a }, &dwRead);
			//Преобразуем в 8 битный формат
			chByte = (uint8_t)symbolBuffer;
			atByte = (uint8_t)attBuffer;
			//Записываем в кеш и приращимваем курсор
			frameCache[_cY][_cX] = chByte;
			atrCacheFr[_cY][_cX] = atByte;
			++_cX;
		}
		_cX = 0;
		++_cY;
	}
	_cY = 0;
	cOutLog << " [X=" << cacheFrameStart.X << " Y=" << cacheFrameStart.Y << "] -> [X=" << cacheFrameSize.X << " Y=" << cacheFrameSize.Y << "]" << endl;
	logOut = logOut + cOutLog.str();
	//putLog(logOut, INFO);
}
void shadowCache::show()	//Вывод окна из кеша
{
	uint8_t _cX = 0;
	uint8_t _cY = 0;
	bCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(bCon, 0x00);
	for (SHORT a = 0;a < cacheFrameSize.Y+1;++a)
	{
		for (SHORT b = 0;b < cacheFrameSize.X+1;++b)
		{
			//Устанавливаем курсор консоли на координаты окна
			SetConsoleCursorPosition(bCon, { cacheFrameStart.X + b,cacheFrameStart.Y + a });
			//Читаем аттрибут символа
			modeByte = atrCacheBuf[_cY][_cX];
			//Применяем аттрибут символа
			SetConsoleTextAttribute(bCon, modeByte);
			//Читаем символ и записываем его
			printf("%c", cacheBuffer[_cY][_cX]);
			++_cX;
		}
		_cX = 0;
		++_cY;
	}
}
void shadowCache::hide()	//Сокрытие окна
{
	uint8_t _cX = 0;
	uint8_t _cY = 0;
	bCon = GetStdHandle(STD_OUTPUT_HANDLE);
	TCHAR symbolBuffer;
	WORD attBuffer;
	DWORD dwRead;
	uint8_t chByte;
	uint8_t atByte;
	SetConsoleTextAttribute(bCon, modeByte);
	for (SHORT a = 0;a < cacheFrameSize.Y+1;++a)
	{
		for (SHORT b = 0;b < cacheFrameSize.X+1;++b)
		{
			SetConsoleCursorPosition(bCon, { cacheFrameStart.X + b,cacheFrameStart.Y + a });
			if (!screenshot)
			{
				//Читаем символ и его атрибут
				ReadConsoleOutputCharacter(bCon, &symbolBuffer, 1, { cacheFrameStart.X + b,cacheFrameStart.Y + a }, &dwRead);
				ReadConsoleOutputAttribute(bCon, &attBuffer, 1, { cacheFrameStart.X + b,cacheFrameStart.Y + a }, &dwRead);
				//Преобразуем к формату 8 битного числа
				chByte = (uint8_t)symbolBuffer;
				atByte = (uint8_t)attBuffer;
				//Помещаем в кеш и приращиваем курсор его
				cacheBuffer[_cY][_cX] = chByte;
				atrCacheBuf[_cY][_cX] = atByte;
			}
			modeByte = atrCacheFr[_cY][_cX];
			SetConsoleTextAttribute(bCon, modeByte);
			printf("%c", frameCache[_cY][_cX]);
			++_cX;
		}
		_cX = 0;
		++_cY;
	}
	_cY = 0;
	screenshot = 1;
}
void shadowCache::setCacheCoord(COORD start, COORD size)
{
	stringstream logCout;
	cacheFrameStart = start;
	cacheFrameSize = size;
	logCout << "Задана область кеширования X=" << cacheFrameSize.X << " Y=" << cacheFrameSize.Y << endl;
	//putLog(logCout.str(), INFO);
}

frameBuilder::frameBuilder()
{
	rTable = new int16_t[4096];
	++itemId;
	colCur = 0;
	logCout << "Создана коллекция с ИД=" << setw(4) << setfill('0') << to_string(itemId) << endl;;
	//putLog(logCout.str(), INFO);
}
frameBuilder::~frameBuilder()
{
	--itemId;
}
void frameBuilder::addItem(itemNames newItem, CMDcolor backColor, CMDcolor textColor, COORD startCoord, COORD sizeCoord, ...)
{
	int16_t xCo;
	int16_t yCo;
	uint8_t targetItemId;	//ИД целевой сущности
	int16_t sYco;	//Координаты разделителя по вертикали
	char *userInputText;	//Контейнер пользовательского ввода
	va_list listParams;
	itemSettings itSet = S_NULL;
	int16_t *rowConfig;
	va_start(listParams, newItem);
	for (uint8_t a = 0;a < 4;++a)	//Пропуск первых 5 аргументов
		va_arg(listParams, itemSettings);
	switch (newItem)
	{
		case ITEM_BASE:
		{
			//cout << "Set BASE" << endl;
			guiCollection[colCur] = new mainWindow(backColor);
			guiCollectionPtr.push_back(guiCollection[colCur]);
			//++colCur;
			break;
		}
		case ITEM_FRAME:
		{
			//cout << "Set FRAME" << endl;
			guiCollection[colCur] = new winFrame(backColor, textColor, startCoord, sizeCoord);
			guiCollectionPtr.push_back(guiCollection[colCur]);
			//++colCur;
			break;
		}
		case ITEM_TABLE:
		{
			//cout << "Set TABLE" << endl;
			guiCollection[colCur] = new table(backColor, textColor, startCoord, sizeCoord);
			guiCollectionPtr.push_back(guiCollection[colCur]);
			//++colCur;
			break;
		}
		case ITEM_TEXT:
		{
			//cout << "Set TEXT" << endl;
			guiCollection[colCur] = new text(backColor, textColor, startCoord, sizeCoord);
			guiCollectionPtr.push_back(guiCollection[colCur]);
			//++colCur;
			break;
		}
		case ITEM_LINE:
		{
			//cout << "Set LINE" << endl;
			guiCollection[colCur] = new line(backColor, textColor, startCoord, sizeCoord);
			guiCollectionPtr.push_back(guiCollection[colCur]);
			//++colCur;
			break;
		}
	}
	while(itSet<=0xFF&&itSet>=0x00)
	{
		switch (itSet)
		{
			case TEXT:
			{
				guiCollection[colCur]->setFillMode(TXT);
				break;
			}
			case BACK:
			{
				guiCollection[colCur]->setFillMode(BCK);
				break;
			}
			case BOTH:
			{
				guiCollection[colCur]->setFillMode(BTH);
				break;
			}
			case SINGLE_LINE:
			{
				guiCollection[colCur]->setStyle(SINGLE_LIN);
				break;
			}
			case DOUBLE_LINE:
			{
				guiCollection[colCur]->setStyle(DOUBLE_LIN);
				break;
			}
			case HIDDEN_LINE:
			{
				guiCollection[colCur]->setStyle(HIDDEN_LIN);
				break;
			}
			case FRAME_ENABLE:
			{
				guiCollection[colCur]->setFrame(FRAME_EN);
				break;
			}
			case FRAME_DISABLE:
			{
				guiCollection[colCur]->setFrame(FRAME_DIS);
				break;
			}
			case F_HEADER_ENABLE:
			{
				guiCollection[colCur]->setFrame(F_HEADER_EN);
				break;
			}
			case F_HEADER_DISABLE:
			{
				guiCollection[colCur]->setFrame(F_HEADER_DIS);
				break;
			}
			case F_CONTROLS_ENABLE:
			{
				guiCollection[colCur]->setFrame(F_CONTROLS_EN);
				break;
			}
			case F_CONTROLS_DISABLE:
			{
				guiCollection[colCur]->setFrame(F_CONTROLS_DIS);
				break;
			}
			case F_CONNECT_DISABLE:
			{
				guiCollection[colCur]->setFrame(F_CONNECT_DIS);
				break;
			}
			case F_CONNECT_SIMPLE:
			{
				guiCollection[colCur]->setFrame(F_CONNECT_SIMPL);
				break;
			}
			case F_CONNECT_ADVANCED:
			{
				guiCollection[colCur]->setFrame(F_CONNECT_ADV);
				break;
			}
			case T_ENABLE_SCROLL:
			{
				guiCollection[colCur]->setFrame(T_ENABLE_SCRL);
				break;
			}
			case T_DISABLE_SCROLL:
			{
				guiCollection[colCur]->setFrame(T_DISABLE_SCRL);
				break;
			}
			case T_BUFFER:
			{
				guiCollection[colCur]->setFrame(T_BUFR);
				break;
			}
			case T_DIRECT:
			{
				guiCollection[colCur]->setFrame(T_DIR);
				break;
			}
			case ALIGN_LEFT:
			{
				guiCollection[colCur]->setAlign(ALIGN_L);
				break;
			}
			case ALIGN_CENTER:
			{
				guiCollection[colCur]->setAlign(ALIGN_C);
				break;
			}
			case ALIGN_RIGHT:
			{
				guiCollection[colCur]->setAlign(ALIGN_R);
				break;
			}
			case L_INVERT_ENABLE:
			{
				guiCollection[colCur]->scanLine(L_INVERT_EN);
				break;
			}
			case L_INVERT_DISABLE:
			{
				guiCollection[colCur]->scanLine(L_INVERT_DIS);
				break;
			}
			case F_SCROLL_ENABLE:
			{
				guiCollection[colCur]->setFrame(F_SCROLL_EN);
				break;
			}
			case F_SCROLL_DISABLE:
			{
				guiCollection[colCur]->setFrame(F_SCROLL_DIS);
				break;
			}
			case F_SHADOW_ENABLE:
			{
				guiCollection[colCur]->setFrame(F_SHADOW_EN);
				break;
			}
			case F_SHADOW_DISABLE:
			{
				guiCollection[colCur]->setFrame(F_SHADOW_DIS);
				break;
			}
			case SINGLE_SEPARATOR:
			{
				sYco = va_arg(listParams, SHORT);
				guiCollection[colCur]->setHorizontalSeparator(sYco, SINGLE_LIN);
				break;
			}
			case DOUBLE_SEPARATOR:
			{
				sYco = va_arg(listParams, SHORT);
				guiCollection[colCur]->setHorizontalSeparator(sYco, DOUBLE_LIN);
				break;
			}
			case THICK_SEPARATOR:
			{
				sYco = va_arg(listParams, SHORT);
				//guiCollection[colCur]->setHorizontalSeparator(sYco, THICK_LIN);
				break;
			}
			case NO_SEPARATOR:
			{
				break;
			}
			case S_NULL:
			{
				break;
			}
			case CLEAR_LINE_DISABLE:
			{
				guiCollection[colCur]->setClearLine(0);
				break;
			}
			case CLEAR_LINE_ENABLE:
			{
				guiCollection[colCur]->setClearLine(1);
				break;
			}
			case COORD_DISABLE:
			{
				guiCollection[colCur]->setTextCoord(0,0,0);
				break;
			}
			case COORD_ENABLE:
			{
				xCo = va_arg(listParams, int16_t);
				yCo = va_arg(listParams, int16_t);
				guiCollection[colCur]->setTextCoord(1, xCo, yCo);
				break;
			}
			case GET_HSEP_INFO:
			{
				targetItemId = va_arg(listParams, uint8_t);
				guiCollection[colCur]->setLineShift(targetItemId);
				break;
			}
			case TABLE_LINE:
			{
				targetItemId = va_arg(listParams, uint8_t);
				userInputText = va_arg(listParams, char*);
				guiCollection[colCur]->placeLine(targetItemId, cpTranslate(userInputText,1251), guiCollection[colCur]->getRowCount());
				break;
			}
			case ITEM_NAME:
			{
				userInputText = va_arg(listParams, char*);
				guiCollection[colCur]->setName(userInputText);
				break;
			}
			case ROWS_CONFIG:
			{
				rowConfig = va_arg(listParams, int16_t*);
				guiCollection[colCur]->setTableRows(rowConfig);
				break;
			}
			case H_SEPARATOR:
			{
				itSet = va_arg(listParams, itemSettings);
				sYco = va_arg(listParams, int16_t);
				switch (itSet)
				{
					case SINGLE_LINE:
					{
						guiCollection[colCur]->setHorizontalSeparator(sYco, SINGLE_LIN);
						break;
					}
					case DOUBLE_LINE:
					{

						guiCollection[colCur]->setHorizontalSeparator(sYco, DOUBLE_LIN);
						break;
					}
				}
				break;
			}
			case TABLE_ROW:
			{
				itSet = va_arg(listParams, itemSettings);
				sYco = va_arg(listParams, int16_t);
				switch (itSet)
				{
					case SINGLE_LINE:
					{
						guiCollection[colCur]->setRows(SINGLE_SEP, sYco);
						break;
					}
					case DOUBLE_LINE:
					{
						guiCollection[colCur]->setRows(DOUBLE_SEP, sYco);
						break;
					}
				}
				break;
			}
			case HEADER_TEXT:
			{
				userInputText = va_arg(listParams, char*);
				guiCollection[colCur]->setHeader(cpTranslate(userInputText, 1251));
				break;
			}
			case TABLE_TEXT:
			{
				userInputText = va_arg(listParams, char*);
				guiCollection[colCur]->printTable(cpTranslate(userInputText, 1251), guiCollection[colCur]->getRowCount());
				break;
			}
			case BODY_TEXT:
			{
				userInputText = va_arg(listParams, char*);
				guiCollection[colCur]->printText(cpTranslate(userInputText, 1251));
				break;
			}
		}
		itSet = va_arg(listParams, itemSettings);
	}
	++colCur;
	va_end(listParams);
}
void frameBuilder::remove_item_back()
{
	guiCollectionPtr.pop_back();
	delete guiCollection[colCur-1];
	colCur--;
}
void frameBuilder::clearCollection()
{
	for (;colCur > 0;--colCur)
		delete guiCollection[colCur - 1];
	guiCollectionPtr.clear();
}
void frameBuilder::heapSort()
{
	guiCollectionPtr.sort([](CmdWinGUI *lhs, CmdWinGUI *rhs) {
		return lhs->getPriority() < rhs->getPriority();
	});
}
void frameBuilder::getHeapInfo()
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
		cout << "ID=" << to_string((*it)->getItemId()) << " RENDER PRIORITY=" << to_string((*it)->getPriority()) << endl;
}
void frameBuilder::render(itemSettings itSet, ...)
{
	va_list argList;
	va_start(argList, itSet);
	uint8_t usrInput = 0;
	switch (itSet)
	{
		case DRAW_GLOBAL:
		{
			for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
				if(!(*it)->getHideFlagStatus())
					(*it)->draw();
			break;
		}
		case DRAW_LOCAL:
		{
			usrInput = va_arg(argList, uint8_t);
			for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
			{
				if ((*it)->getPriority() >= usrInput && !(*it)->getHideFlagStatus())
					(*it)->draw();
			}
			break;
		}
		case DRAW_TARGET:
		{
			usrInput = va_arg(argList, uint8_t);
			for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
				if ((*it)->getItemId() == usrInput && !(*it)->getHideFlagStatus())
					(*it)->draw();
			break;
		}
		case DRAW_EDITED:
		{
			usrInput = va_arg(argList, uint8_t);
			for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
			{
				if ((*it)->getEditFlagStatus() && !(*it)->getHideFlagStatus())
					(*it)->draw();
			}
			break;
		}
	}
	va_end(argList);
}
void frameBuilder::lineCtrReset(uint16_t itemId)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
		if ((*it)->getItemId() == itemId)
		{
			(*it)->filledRowsReset(); 
			break;
		}
}
void frameBuilder::printTabLine(uint16_t itemId, uint8_t lineNo, char* str)
{
	for(auto it=guiCollectionPtr.begin();it!=guiCollectionPtr.end();++it)
		if ((*it)->getItemId() == itemId)
		{
			(*it)->placeLine(lineNo, cpTranslate(str, 1251), (*it)->getRowCount());
			break;
		}
}
void frameBuilder::lineErase(uint16_t itemId, uint16_t tableId, uint8_t lineNo)
{
	char _eptr[256];
	uint8_t linCnt = 0;
	memset(_eptr, 0x20, 256);
	uint8_t innouLineSize = 0;
	//cout << "Text line ID=" << to_string(itemId) << endl;
	//cout << "Table line ID=" << to_string(tableId) << endl;
	/*for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			cout << "Text found" << endl;
			(*it)->getItemId();
		}
		if ((*it)->getItemId() == tableId)
		{
			cout << "Table found" << endl;
			(*it)->getItemId();
		}
	}*/
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			linCnt = (*it)->getLinesCount();
			for (auto it2 = guiCollectionPtr.begin();it2 != guiCollectionPtr.end();++it2)
			{
				if ((*it2)->getItemId() == tableId)
				{
					linCnt = (*it2)->getLinesCount();
					for (uint8_t a = 0;a <= (*it2)->getLinesCount();++a)
					{
						//Хотфикс 09.10.2023 14:31
						//Искажение позиции курсора при выбранном режиме выравнивания(центр, справа)
						//(*it)->placeLine(lineNo, _eptr, (*it)->getRowCount());
						(*it)->eraseLine(lineNo, (*it)->getRowCount());
					}
					break;
				}
			}
		}
	}
}
void frameBuilder::printItemText(uint16_t itemId, char* input, itemSettings itSet)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			switch (itSet)
			{
				case ALIGN_LEFT:
				{
					(*it)->setAlign(ALIGN_L);
					(*it)->printText(cpTranslate(input,1251));
					break;
				}
				case ALIGN_CENTER:
				{
					(*it)->setAlign(ALIGN_C);
					(*it)->printText(cpTranslate(input, 1251));
					break;
				}
				case ALIGN_RIGHT:
				{
					(*it)->setAlign(ALIGN_R);
					(*it)->printText(cpTranslate(input, 1251));
					break;
				}
			}
			break;
		}
	}
}
void frameBuilder::itemTextErase(uint16_t itemId)
{
	char nullArr[256];
	memset(nullArr, 0x00, 256);
	memset(nullArr, 0x20, 255);
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			(*it)->printText(nullArr);
			break;
		}
	}
}
void frameBuilder::itemHeaderErase(uint16_t itemId)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			(*it)->setHeader("NULL");
		}
	}
}
void frameBuilder::printItemHeader(uint16_t itemId, char* input, itemSettings itSet)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			switch (itSet)
			{
				case ALIGN_LEFT:
				{
					(*it)->setAlign(ALIGN_L);
					(*it)->setHeader(cpTranslate(input, 1251));
					break;
				}
				case ALIGN_CENTER:
				{
					(*it)->setAlign(ALIGN_C);
					(*it)->setHeader(cpTranslate(input, 1251));
					break;
				}
				case ALIGN_RIGHT:
				{
					(*it)->setAlign(ALIGN_R);
					(*it)->setHeader(cpTranslate(input, 1251));
					break;
				}
			}
			break;
		}
	}
}
void frameBuilder::setFrame(uint16_t itemId, itemSettings itSet, ...)
{
	itemSettings *_is = &itSet;
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end(); ++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			while (*_is < 0xFF && *_is>0x00)
			{
				switch (*_is)
				{
					case SINGLE_LINE:
					{
						(*it)->setStyle(SINGLE_LIN);
						break;
					}
					case DOUBLE_LINE:
					{
						(*it)->setStyle(DOUBLE_LIN);
						break;
					}
					case HIDDEN_LINE:
					{
						(*it)->setStyle(HIDDEN_LIN);
						break;
					}
					case FRAME_ENABLE:
					{
						(*it)->setFrame(FRAME_EN);
						break;
					}
					case FRAME_DISABLE:
					{
						(*it)->setFrame(FRAME_DIS);
						break;
					}
					case F_HEADER_ENABLE:
					{
						(*it)->setFrame(F_HEADER_EN);
						break;
					}
					case F_HEADER_DISABLE:
					{
						(*it)->setFrame(F_HEADER_DIS);
						break;
					}
					case F_CONTROLS_ENABLE:
					{
						(*it)->setFrame(F_CONTROLS_EN);
						break;
					}
					case F_CONTROLS_DISABLE:
					{
						(*it)->setFrame(F_CONTROLS_DIS);
						break;
					}
					case F_CONNECT_DISABLE:
					{
						(*it)->setFrame(F_CONNECT_DIS);
						break;
					}
					case F_CONNECT_SIMPLE:
					{
						(*it)->setFrame(F_CONNECT_SIMPL);
						break;
					}
					case F_CONNECT_ADVANCED:
					{
						(*it)->setFrame(F_CONNECT_ADV);
						break;
					}
					case F_SCROLL_ENABLE:
					{
						(*it)->setFrame(F_SCROLL_EN);
						break;
					}
					case F_SCROLL_DISABLE:
					{
						(*it)->setFrame(F_SCROLL_DIS);
						break;
					}
					case F_SHADOW_ENABLE:
					{
						(*it)->setFrame(F_SHADOW_EN);
						break;
					}
					case F_SHADOW_DISABLE:
					{
						(*it)->setFrame(F_SHADOW_DIS);
						break;
					}
				}
				++_is;
			}
			break;
		}
	}
}
int16_t *frameBuilder::getRowConfig(uint16_t itemId)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			memcpy(rTable, (*it)->getRowSize(), 256);
			//return (*it)->getRowSize();
			break;
		}
	}
	return rTable;
}
uint8_t frameBuilder::getHsepShift(uint16_t itemId)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
		{
			return (uint8_t)(*it)->getHsepShift();
		}
	}
	return 0;
}
uint16_t frameBuilder::searchByName(string targetName)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
		if ((*it)->getItemName() == targetName)
			return (*it)->getItemId();
	return NULL;
}
COORD frameBuilder::getTextPos(uint16_t itemId)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
			return (*it)->getTextPos();
	}
	return { 0,0 };
}
COORD frameBuilder::getTextSize(uint16_t itemId)
{
	for (auto it = guiCollectionPtr.begin();it != guiCollectionPtr.end();++it)
	{
		if ((*it)->getItemId() == itemId)
			return (*it)->getTextSize();
	}
	return { 0,0 };
}

string cpTranslate(char *input, int srcCp)
{
	//char temp[256];
	char *temp;
	temp = new char[256];
	memset(temp, 0x00, 256 * sizeof(char));
	//cout << "input string: " << input << endl;
	switch (srcCp)
	{
		case 1251:
		{
			for (uint8_t a = 0;a < 0xFF;++a)
			{
				if (input[a] == NULL)
					break;
				temp[a] = charSet1251_to_866[(uint8_t)input[a]];
				//printf(">> CHAR: %c 0x%02X\n", input[a], (uint8_t)input[a]);
				//printf("<< CHAR: %c 0x%02X\n", temp[a], (uint8_t)temp[a]);
			}
		}
	}
	/*cout << "Output string: " << temp << endl;
	for (uint8_t a = 0;a < 0xFF;++a)
	{
		if (input[a] == NULL)
			break;
		printf("CHAR: %c 0x%02X\n", input[a], (uint8_t)input[a]);
	}*/
	string output(temp);
	delete[] temp;
	return output;
}
