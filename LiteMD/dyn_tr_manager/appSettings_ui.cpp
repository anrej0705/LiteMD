#include "appSettings.h"
#include "global_definitions.h"
#include "ui_update_event.h"
extern "C"
{
#include "globalFlags.h"
}
extern struct parser_switchers parswitch;
bool appSettings::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_appSettings_UPDATE_EVENT))
	{
		update_ui();
		return 1;
	}
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_UI_UPDATE_USER_SETTINGS))
	{
		update_interactive();
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
	themeList->setItemText(0, tr("Will be added in future"));
	saveSettings->setItemText(0, tr("Will be added in future"));
	saveFreq->setItemText(0, tr("Will be added in future"));
	themeHint->setText(tr("UI Theme"));
	saveSettingsHint->setText(tr("Settings save type"));
	autoSaveHint->setText(tr("Autosave"));
	saveFreqHint->setText(tr("Autosave frequency"));
	depFuncHint->setText(tr("Enable deprecated features"));
	inDevFuncHint->setText(tr("Enable in-dev features"));
	reloadHint->setText(tr("reloadHint"));
	parseSimplLinksHint->setText(tr("Parse URL links"));
	parseAdvLinksHint->setText(tr("parseAdvLinksHint"));
	parseHeaderLvlHint->setText(tr("parseHeaderLvlHint"));
	allowWarningsHint->setText(tr("Show one-time deprecated warning"));
	allowCacheHint->setText(tr("Enable content caching"));
}

void appSettings::update_interactive()
{
	enableIndevFeatures == 0 ? devFunc->setChecked(0) : devFunc->setChecked(1);
	enableDeprFeatures == 0 ? depFunc->setChecked(0) : depFunc->setChecked(1);
	parswitch.en_simple_url == 0 ? parseSimplLinks->setChecked(0) : parseSimplLinks->setChecked(1);
	parswitch.en_adv_url == 0 ? parseAdvLinksl->setChecked(0) : parseAdvLinksl->setChecked(1);
	parswitch.en_header_lvl == 0 ? parseHeaderLvl->setChecked(0) : parseHeaderLvl->setChecked(1);
}
