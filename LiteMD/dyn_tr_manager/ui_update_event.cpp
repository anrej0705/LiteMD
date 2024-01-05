#include "ui_update_event.h"

/*
* Таблица событий для обновления интерфейса
* Диапазон ID 34-101
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

ui_event_filter::ui_event_filter(QObject* pobj) : QObject(pobj)
{}
bool ui_event_filter::eventFilter(QObject* podj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + 33))
	{
		ui_event_filter(p_event);
		return 1;
	}
	//ui_event_filter(p_event);
	return 0;
}
