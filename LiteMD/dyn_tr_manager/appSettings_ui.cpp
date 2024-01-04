#include "appSettings.h"
void appSettings::update_ui()
{
	setWindowTitle(tr("LiteMD Settings"));
	btnOk->setText(tr("&Ok"));
	btnCancel->setText(tr("&Cancel"));
	btnApply->setText(tr("&Apply"));
	workprogress->setText("<H1>" + tr("Work in progress, come later))") + "</H1>");
	langlistHint->setText(tr("Language:"));
	settingsLister->setTabText(0, tr("Basic"));
	settingsLister->setTabText(1, tr("Cap"));
}
