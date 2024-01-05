#include "appSettings.h"
#include "ui_update_event.h"
void appSettings::update_ui(ui_update_event* p_event)
{
	bool a = 0;
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + 33))
	{
		a = 1;
	}
	setWindowTitle(tr("LiteMD Settings"));
	btnOk->setText(tr("&Ok"));
	btnCancel->setText(tr("&Cancel"));
	btnApply->setText(tr("&Apply"));
	workprogress->setText("<H1>" + tr("Work in progress, come later))") + "</H1>");
	langlistHint->setText(tr("Language:"));
	settingsLister->setTabText(0, tr("Basic"));
	settingsLister->setTabText(1, tr("Cap"));
}
