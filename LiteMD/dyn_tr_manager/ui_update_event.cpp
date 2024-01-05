#include "ui_update_event.h"


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
