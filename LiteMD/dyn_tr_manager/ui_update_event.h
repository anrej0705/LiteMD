#pragma once
#include <QtWidgets>

class ui_update_event : public QEvent 
{
	public:
		ui_update_event() : QEvent(static_cast<QEvent::Type>(QEvent::User+33))
		{}
		QString info()
		{
			return "Custom event";
		}
};

class ui_event_filter : public QObject
{
	protected:
		virtual bool eventFilter(QObject*, QEvent*);
	public:
		ui_event_filter(QObject* podj = 0);
};
