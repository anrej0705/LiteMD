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
extern bool httpDerpWarned;

#define UI_UPDATE_EVENT 1033
#endif
