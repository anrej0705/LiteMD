#pragma once
#include <QtWidgets>
#include "globalFlags.h"

class ui_update_event : public QEvent
{
	public:
		ui_update_event() : QEvent((Type)(UI_UPDATE_EVENT))
		{}
};
