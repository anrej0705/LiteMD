#include "logger.h"
#include "global_definitions.h"
#include "ui_update_event.h"

bool logger::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_logger_UPDATE_EVENT))
	{
		update_ui();
		return 1;
	}
	return QWidget::eventFilter(pobj, p_event);
}
//Обновление интерфейса с помощью повторного чтения выбранной локализации
void logger::update_ui()
{
	this->setWindowTitle(tr("logTitle"));
	clearLog->setText(tr("clearLog"));
	saveLog->setText(tr("saveLog"));
	frame->setTitle(tr("logHint"));
}
