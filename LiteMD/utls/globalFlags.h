#pragma once
#ifndef GLOBALFLAGS_H_
#define GLOBALFLAGS_H_
#include <stdbool.h>
//����� ���������
extern bool fileOpenedState;
extern bool fileChangedState;
extern bool appTitleUpdated;
extern bool appClose;
//Deprecated
extern bool allowHttpWarn;
extern bool httpDerpWarned;

//���������
extern struct appSettingsStruct
{
	bool logReadState
}settingStruct;
#endif
