#include "frameChangelog.h"
#include "global_definitions.h"
#include "ui_update_event.h"
#include "logger_backend.h"
extern "C"
{
	#include "globalFlags.h"
}

bool currentChangelog::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_frame_changelog_UPDATE_EVENT))
	{
		update_ui();
		return 1;
	}
	return QWidget::eventFilter(pobj, p_event);
}

void currentChangelog::update_ui()
{
	push_log("[frameChangelog_ui]Обновление интерфейса");

	//Создаём список потомков
	QList<QWidget*> wList = this->findChildren<QWidget*>();

	//Дрюкаем новым стилем каждого по очереди
	foreach(QWidget * sWgt, wList)
	{
		sWgt->setStyle(QStyleFactory::create(chosenTheme));
	}

	//Сбрасываем флаг
	//uiChanged = 0;

	setWindowTitle(tr("New version changelog"));
}
