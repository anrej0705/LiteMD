#include "appSettings.h"
#include "global_definitions.h"
#include "ui_update_event.h"

bool appSettings::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_appSettings_UPDATE_EVENT))
	{
		update_ui();
		return 1;
	}
	return QWidget::eventFilter(pobj, p_event);
}

void appSettings::update_ui()
{
	setWindowTitle(tr("LiteMD Settings"));
	btnOk->setText(tr("&Ok"));
	btnCancel->setText(tr("&Cancel"));
	btnApply->setText(tr("&Apply"));
	workprogress->setText("<H1>" + tr("Work in progress, come later))") + "</H1>");
	langListHint->setText(tr("Language:"));
	settingsLister->setTabText(0, tr("Basic"));
	settingsLister->setTabText(1, tr("Render"));
	settingsLister->setTabText(2, tr("Downloader"));
	settingsLister->setTabText(3, tr("Cap"));
	parseLinksHint->setText(tr("Parse URL links"));
	themeList->setItemText(0, tr("Will be added in future"));
	saveSettings->setItemText(0, tr("Will be added in future"));
	saveFreq->setItemText(0, tr("Will be added in future"));
	themeHint->setText(tr("UI Theme"));
	saveSettingsHint->setText(tr("Settings save type"));
	autoSaveHint->setText(tr("Autosave"));
	saveFreqHint->setText(tr("Autosave frequency"));
	depFuncHint->setText(tr("Enable deprecated features"));
	inDevFuncHint->setText(tr("Enable in-dev features"));
}
