#include "GuiDownloader.h"
#include "global_definitions.h"

bool DownloaderGui::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_GuiDownloader_UPDATE_EVENT))
	{
		update_ui();
		return 1;
	}
	return QWidget::eventFilter(pobj, p_event);
}

void DownloaderGui::update_ui()
{
	setWindowTitle(tr("HTTP Download module GUI(Deprecated)"));
	dwPt->setText(tr("&Go"));
	plbl->setWindowTitle(tr("View"));

}
