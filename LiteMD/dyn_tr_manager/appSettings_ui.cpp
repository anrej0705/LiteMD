#include "appSettings.h"
#include "ui_update_event.h"
#include "event_id_constructor.h"
#include "logger_backend.h"
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
	push_log("[appSettings_ui]Обновление интерфейса");

	//Создаём список потомков
	QList<QWidget*> wList = this->findChildren<QWidget*>();

	//Дрюкаем новым стилем каждого по очереди
	foreach(QWidget * sWgt, wList)
	{
		sWgt->setStyle(QStyleFactory::create(chosenTheme));
	}

	//Сбрасываем флаг
	//uiChanged = 0;

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
	settingsLister->setTabText(5, tr("Hacks"));
	settingsLister->setTabText(6, tr("Cap"));
	//themeList->setItemText(0, tr("Default"));
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
	msgLimitHint->setText(tr("msgLimitHint"));
	limitSpinBox->setValue(log_limit);
	setDefault->setText(tr("setDefault"));
	parseUnderlinedHint->setText(tr("parseUnderlinedHint"));
	compatilibtyUndrHint->setText(tr("compatilibtyUndrHint"));
	parseItalicHint->setText(tr("parseItalicHint"));
	parseBoldHint->setText(tr("parseBoldHint"));
	saveLocationHint->setText(tr("saveLocationHint"));
	userPathHint->setText(tr("userPathHint"));

	//Задаем язык переводов на выбранный в настройках
	parseSimplLinks->setWhatsThis(tr("parseSimplLinksHelp"));
	parseAdvLinksl->setWhatsThis(tr("parseAdvLinkslHelp"));
	parseHeaderLvl->setWhatsThis(tr("parseHeaderLvlHelp"));
	parseItalic->setWhatsThis(tr("parseItalicHelp"));
	deprSyntaxPrep->setWhatsThis(tr("deprSyntaxPrepHelp"));
	deprSyntaxPost->setWhatsThis(tr("deprSyntaxPostHelp"));
	deprUrlSimplParser->setWhatsThis(tr("deprUrlSimplParserHelp"));
	deprUrlAdvParser->setWhatsThis(tr("deprUrlAdvParserHelp"));
	deprUrlBasParser->setWhatsThis(tr("deprUrlBasParserHelp"));
	combatilibtyUndr->setWhatsThis(tr("combatilibtyUndrHelp"));
	parseBold->setWhatsThis(tr("parseBoldHelp"));
	clearLog->setWhatsThis(tr("clearLogHelp"));
	saveLog->setWhatsThis(tr("saveLogHelp"));
	logBox->setWhatsThis(tr("logBoxHelp"));
	parseUnderlined->setWhatsThis(tr("parseUnderlinedHelp"));
	parseStrikethrough->setWhatsThis(tr("parseStrikethroughHelp"));
	allowWarnings->setWhatsThis(tr("allowWarningsHelp"));
	allowCache->setWhatsThis(tr("allowCacheHelp"));
	langList->setWhatsThis(tr("langListHelp"));
	themeList->setWhatsThis(tr("themeListHelp"));
	colorTheme->setWhatsThis(tr("colorThemeHelp"));
	saveSettings->setWhatsThis(tr("saveSettingsHelp"));
	saveLocation->setWhatsThis(tr("saveLocationHelp"));
	userPath->setWhatsThis(tr("userPathHelp"));
	autoSave->setWhatsThis(tr("autoSaveHelp"));
	saveFreq->setWhatsThis(tr("saveFreqHelp"));
	depFunc->setWhatsThis(tr("depFuncHelp"));
	devFunc->setWhatsThis(tr("devFuncHelp"));
	limitSpinBox->setWhatsThis(tr("limitSpinBoxHelp"));
}

void appSettings::update_interactive()
{
	//Меняем галочки в зависимости от полученных настроек
	enableIndevFeatures == 0 ? devFunc->setChecked(0) : devFunc->setChecked(1);
	enableDeprFeatures == 0 ? depFunc->setChecked(0) : depFunc->setChecked(1);
	parswitch.en_simple_url == 0 ? parseSimplLinks->setChecked(0) : parseSimplLinks->setChecked(1);
	parswitch.en_adv_url == 0 ? parseAdvLinksl->setChecked(0) : parseAdvLinksl->setChecked(1);
	parswitch.en_header_lvl == 0 ? parseHeaderLvl->setChecked(0) : parseHeaderLvl->setChecked(1);
	parswitch.en_ex_strkthg == 0 ? parseStrikethrough->setChecked(0) : parseStrikethrough->setChecked(1);
	parswitch.en_underlined == 0 ? parseUnderlined->setChecked(0) : parseUnderlined->setChecked(1);
	parswitch.en_compat_undr == 0 ? combatilibtyUndr->setChecked(0) : combatilibtyUndr->setChecked(1);
	parswitch.en_italic == 0 ? parseItalic->setChecked(0) : parseItalic->setChecked(1);
	parswitch.en_bold == 0 ? parseBold->setChecked(0) : parseBold->setChecked(1);
	dparswitch.en_t_post == 0 ? deprSyntaxPrep->setChecked(0) : deprSyntaxPrep->setChecked(1);
	dparswitch.en_t_prep == 0 ? deprSyntaxPost->setChecked(0) : deprSyntaxPost->setChecked(1);
	dparswitch.en_url_adv == 0 ? deprUrlSimplParser->setChecked(0) : deprUrlSimplParser->setChecked(1);
	dparswitch.en_url_bas == 0 ? deprUrlAdvParser->setChecked(0) : deprUrlAdvParser->setChecked(1);
	dparswitch.en_url_bas_simple == 0 ? deprUrlBasParser->setChecked(0) : deprUrlBasParser->setChecked(1);

	//Меняем доступность галочек
	enableDeprFeatures == 0 ? parseSimplLinks->setEnabled(1) : parseSimplLinks->setDisabled(1);
	enableDeprFeatures == 0 ? parseAdvLinksl->setEnabled(1) : parseAdvLinksl->setDisabled(1);
	enableDeprFeatures == 0 ? parseHeaderLvl->setEnabled(1) : parseHeaderLvl->setDisabled(1);
	enableDeprFeatures == 0 ? parseStrikethrough->setEnabled(1) : parseStrikethrough->setDisabled(1);
	enableDeprFeatures == 0 ? parseUnderlined->setEnabled(1) : parseUnderlined->setDisabled(1);
	enableDeprFeatures == 0 ? parseItalic->setEnabled(1) : parseItalic->setDisabled(1);
	enableDeprFeatures == 0 ? parseBold->setEnabled(1) : parseBold->setDisabled(1);
	enableDeprFeatures == 0 ? deprSyntaxPrep->setDisabled(1) : deprSyntaxPrep->setEnabled(1);
	enableDeprFeatures == 0 ? deprSyntaxPost->setDisabled(1) : deprSyntaxPost->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlSimplParser->setDisabled(1) : deprUrlSimplParser->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlAdvParser->setDisabled(1) : deprUrlAdvParser->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlBasParser->setDisabled(1) : deprUrlBasParser->setEnabled(1);

	enableDeprFeatures == 0 ? parseSimplLinksHint->setEnabled(1) : parseSimplLinksHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseAdvLinksHint->setEnabled(1) : parseAdvLinksHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseHeaderLvlHint->setEnabled(1) : parseHeaderLvlHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseStrikethroughHint->setEnabled(1) : parseStrikethroughHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseUnderlinedHint->setEnabled(1) : parseUnderlinedHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseItalicHint->setEnabled(1) : parseItalicHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseBoldHint->setEnabled(1) : parseBoldHint->setDisabled(1);
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
	enableIndevFeatures == 1 ? saveLocationHint->setEnabled(1) : saveLocationHint->setDisabled(1);
	enableIndevFeatures == 1 ? userPathHint->setEnabled(1) : userPathHint->setDisabled(1);

	enableIndevFeatures == 1 ? themeList->setEnabled(1) : themeList->setDisabled(1);
	enableIndevFeatures == 1 ? saveSettings->setEnabled(1) : saveSettings->setDisabled(1);
	enableIndevFeatures == 1 ? autoSave->setEnabled(1) : autoSave->setDisabled(1);
	enableIndevFeatures == 1 ? saveFreq->setEnabled(1) : saveFreq->setDisabled(1);
	enableIndevFeatures == 1 ? colorTheme->setEnabled(1) : colorTheme->setDisabled(1);
	enableIndevFeatures == 1 ? allowCache->setEnabled(1) : allowCache->setDisabled(1);
	enableIndevFeatures == 1 ? saveLocation->setEnabled(1) : saveLocation->setDisabled(1);
	enableIndevFeatures == 1 ? userPath->setEnabled(1) : userPath->setDisabled(1);
}
