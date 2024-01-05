#pragma once
#ifndef GLOBALFLAGS_H_
#define GLOBALFLAGS_H_
#include <stdbool.h>
//Флаги состояний
extern bool fileOpenedState;
extern bool fileChangedState;
extern bool appTitleUpdated;
extern bool appClose;
//Deprecated
extern bool httpDerpWarned;
#endif
