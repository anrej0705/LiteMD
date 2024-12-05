#include "appSettings.h"
#include "ui_update_event.h"
#include "event_id_constructor.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}
extern struct parser_switchers parswitch;
extern struct depr_paerser_switchers dparswitch;
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
	settingsLister->setTabText(3, tr("Logs"));
	settingsLister->setTabText(4, tr("Extended"));
	//settingsLister->setTabText(5, tr("Logs")); 0.2.5
	settingsLister->setTabText(5, tr("Cap"));
	themeList->setItemText(0, tr("Default"));
	colorTheme->setItemText(0, tr("Default"));
	saveSettings->setItemText(0, tr("XML"));
	saveFreq->setItemText(0, tr("NaN"));
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
	colorThemeHint->setText(tr("colorThemeHint"));
	clearLog->setText(tr("clearLog"));
	saveLog->setText(tr("saveLog"));
	logHint->setText(tr("logHint"));
	deprSyntaxPrepHint->setText(tr("deprSyntaxPrepHint"));
	deprSyntaxPostHint->setText(tr("deprSyntaxPostHint"));
	deprUrlSimplParserHint->setText(tr("deprUrlSimplParserHint"));
	deprUrlAdvParserHint->setText(tr("deprUrlAdvParserHint"));
	deprUrlBasParserHint->setText(tr("deprUrlBasParserHint"));
	parseStrikethroughHint->setText(tr("parseStrikethroughHint"));
}

void appSettings::update_interactive()
{
	//������ ������� � ����������� �� ���������� ��������
	enableIndevFeatures == 0 ? devFunc->setChecked(0) : devFunc->setChecked(1);
	enableDeprFeatures == 0 ? depFunc->setChecked(0) : depFunc->setChecked(1);
	parswitch.en_simple_url == 0 ? parseSimplLinks->setChecked(0) : parseSimplLinks->setChecked(1);
	parswitch.en_adv_url == 0 ? parseAdvLinksl->setChecked(0) : parseAdvLinksl->setChecked(1);
	parswitch.en_header_lvl == 0 ? parseHeaderLvl->setChecked(0) : parseHeaderLvl->setChecked(1);
	dparswitch.en_t_post == 0 ? deprSyntaxPrep->setChecked(0) : deprSyntaxPrep->setChecked(1);
	dparswitch.en_t_prep == 0 ? deprSyntaxPost->setChecked(0) : deprSyntaxPost->setChecked(1);
	dparswitch.en_url_adv == 0 ? deprUrlSimplParser->setChecked(0) : deprUrlSimplParser->setChecked(1);
	dparswitch.en_url_bas == 0 ? deprUrlAdvParser->setChecked(0) : deprUrlAdvParser->setChecked(1);
	dparswitch.en_url_bas_simple == 0 ? deprUrlBasParser->setChecked(0) : deprUrlBasParser->setChecked(1);

	//������ ����������� �������
	enableDeprFeatures == 0 ? parseSimplLinks->setEnabled(1) : parseSimplLinks->setDisabled(1);
	enableDeprFeatures == 0 ? parseAdvLinksl->setEnabled(1) : parseAdvLinksl->setDisabled(1);
	enableDeprFeatures == 0 ? parseHeaderLvl->setEnabled(1) : parseHeaderLvl->setDisabled(1);
	enableDeprFeatures == 0 ? parseStrikethrough->setEnabled(1) : parseStrikethrough->setDisabled(1);
	//enableDeprFeatures == 0 ? parseUnderlined->setEnabled(1) : parseUnderlined->setDisabled(1); 0.2.5
	enableDeprFeatures == 0 ? deprSyntaxPrep->setDisabled(1) : deprSyntaxPrep->setEnabled(1);
	enableDeprFeatures == 0 ? deprSyntaxPost->setDisabled(1) : deprSyntaxPost->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlSimplParser->setDisabled(1) : deprUrlSimplParser->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlAdvParser->setDisabled(1) : deprUrlAdvParser->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlBasParser->setDisabled(1) : deprUrlBasParser->setEnabled(1);

	enableDeprFeatures == 0 ? parseSimplLinksHint->setEnabled(1) : parseSimplLinksHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseAdvLinksHint->setEnabled(1) : parseAdvLinksHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseHeaderLvlHint->setEnabled(1) : parseHeaderLvlHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseStrikethroughHint->setEnabled(1) : parseStrikethroughHint->setDisabled(1);
	//enableDeprFeatures == 0 ? parseUnderlinedHint->setEnabled(1) : parseUnderlinedHint->setDisabled(1); 0.2.5
	enableDeprFeatures == 0 ? deprSyntaxPrepHint->setDisabled(1) : deprSyntaxPrepHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprSyntaxPostHint->setDisabled(1) : deprSyntaxPostHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlSimplParserHint->setDisabled(1) : deprUrlSimplParserHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlAdvParserHint->setDisabled(1) : deprUrlAdvParserHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlBasParserHint->setDisabled(1) : deprUrlBasParserHint->setEnabled(1);

	enableIndevFeatures == 1 ? themeHint->setEnabled(1) : themeHint->setDisabled(1);
	enableIndevFeatures == 1 ? saveSettingsHint->setEnabled(1) : saveSettingsHint->setDisabled(1);
	enableIndevFeatures == 1 ? autoSaveHint->setEnabled(1) : autoSaveHint->setDisabled(1);
	enableIndevFeatures == 1 ? saveFreqHint->setEnabled(1) : saveFreqHint->setDisabled(1);
	enableIndevFeatures == 1 ? colorThemeHint->setEnabled(1) : colorThemeHint->setDisabled(1);
	enableIndevFeatures == 1 ? allowCacheHint->setEnabled(1) : allowCacheHint->setDisabled(1);

	enableIndevFeatures == 1 ? themeList->setEnabled(1) : themeList->setDisabled(1);
	enableIndevFeatures == 1 ? saveSettings->setEnabled(1) : saveSettings->setDisabled(1);
	enableIndevFeatures == 1 ? autoSave->setEnabled(1) : autoSave->setDisabled(1);
	enableIndevFeatures == 1 ? saveFreq->setEnabled(1) : saveFreq->setDisabled(1);
	enableIndevFeatures == 1 ? colorTheme->setEnabled(1) : colorTheme->setDisabled(1);
	enableIndevFeatures == 1 ? allowCache->setEnabled(1) : allowCache->setDisabled(1);
}
