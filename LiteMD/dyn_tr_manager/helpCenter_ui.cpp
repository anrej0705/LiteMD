#include "helpCenter.h"
#include "ui_update_event.h"
#include "event_id_constructor.h"
#include "logger_backend.h"

extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

bool helpCenter::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_helpCenter_UPDATE_EVENT))
	{
		update_ui();
		return 1;
	}
	return QWidget::eventFilter(pobj, p_event);
}

void helpCenter::update_ui()
{
	push_log("[helpCenter_ui]Обновление интерфейса");

	//Создаём список потомков
	QList<QWidget*> wList = this->findChildren<QWidget*>();

	//Дрюкаем новым стилем каждого по очереди
	foreach(QWidget * sWgt, wList)
	{
		sWgt->setStyle(QStyleFactory::create(chosenTheme));
	}

	//Сбрасываем флаг
	//uiChanged = 0;

	this->setWindowTitle(tr("Program guide"));
}