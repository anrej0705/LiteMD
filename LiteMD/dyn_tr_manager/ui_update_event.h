#pragma once
#include <QtWidgets>

class ui_event_filter : public QObject
{
	protected:
		virtual bool eventFilter(QObject*, QEvent*);
	public:
		ui_event_filter(QObject* podj = 0);
};
