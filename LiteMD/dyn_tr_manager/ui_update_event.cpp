#include "ui_update_event.h"
#include "global_definitions.h"
#include "event_id_constructor.h"

ui_event_filter::ui_event_filter(QObject* pobj) : QObject(pobj)
{}
bool ui_event_filter::eventFilter(QObject* podj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_UI_UPDATE_EVENT))	//1000+33=1033
	{
		ui_event_filter(p_event);
		if (!QCoreApplication::sendEvent(qApp, new event_id_constructor(APP_EVENT_appSettings_UPDATE_EVENT)))
			QErrorMessage::qtHandler();
		return 1;
	}
	//ui_event_filter(p_event);
	return 0;
}
