#pragma once
#include <QtWidgets>

//Конструктор событий
class event_id_constructor : public QEvent
{
	private:
		uint16_t e_id;
	public:
		event_id_constructor(const uint16_t event_id) : QEvent(static_cast<QEvent::Type>(QEvent::User + event_id))
		{
			e_id = event_id;
		}
		QString info()
		{
			return "Event ID="+QString::number(QEvent::User+ e_id);
		}
};
