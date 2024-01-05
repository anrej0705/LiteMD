#pragma once
#include <qevent.h>

//Конструктор событий, создает событие с пользовательским ИД и выгоняет
class event_id_constructor : public QEvent
{
	private:
		uint16_t e_id;
	public:
		event_id_constructor(const uint16_t event_id) : QEvent(static_cast<QEvent::Type>(QEvent::User + event_id))	//1000 + ИД пользователя
		{
			e_id = event_id;
		}
		QString info()
		{
			return "Event ID="+QString::number(QEvent::User+ e_id);
		}
};
