#include "appSettings.h"
#include "global_definitions.h"
extern "C"
{
	#include "globalFlags.h"
}
void appSettings::configureRenderSettingsTab()
{
	renderSettings = new QWidget;

	//Инициализируем метки
	parseSimplLinksHint = new QLabel(tr("parseSimplLinksHint"));
	parseAdvLinksHint = new QLabel(tr("parseAdvLinksHint"));
	parseHeaderLvlHint = new QLabel(tr("parseHeaderLvlHint"));
	deprSyntaxPrepHint = new QLabel(tr("deprSyntaxPrepHint"));
	deprSyntaxPostHint = new QLabel(tr("deprSyntaxPostHint"));
	deprUrlSimplParserHint = new QLabel(tr("deprUrlSimplParserHint"));
	deprUrlAdvParserHint = new QLabel(tr("deprUrlAdvParserHint"));
	deprUrlBasParserHint = new QLabel(tr("deprUrlBasParserHint"));
	compatilibtyUndrHint = new QLabel(tr("compatilibtyUndrHint"));
	parseItalicHint = new QLabel(tr("parseItalicHint"));
	parseBoldHint = new QLabel(tr("parseBoldHint"));

	//Подсказка для перезагрузки документа
	reloadHint = new QLabel(tr("reloadHint"));
	reloadHint->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
	reloadHint->setFixedHeight(SETTINGS_HEIGH);
	 
	//Инициализируем элементы взаимодействия
	parseSimplLinks = new QCheckBox;
	parseAdvLinksl = new QCheckBox;
	parseHeaderLvl = new QCheckBox;
	parseItalic = new QCheckBox;
	parseBold = new QCheckBox;
	deprSyntaxPrep = new QCheckBox;
	deprSyntaxPost = new QCheckBox;
	deprUrlSimplParser = new QCheckBox;
	deprUrlAdvParser = new QCheckBox;
	deprUrlBasParser = new QCheckBox;
	combatilibtyUndr = new QCheckBox;

	//Устанавливаем высоту
	parseSimplLinksHint->setFixedHeight(SETTINGS_HEIGH);
	parseAdvLinksHint->setFixedHeight(SETTINGS_HEIGH);
	parseHeaderLvlHint->setFixedHeight(SETTINGS_HEIGH);
	deprSyntaxPrepHint->setFixedHeight(SETTINGS_HEIGH);
	deprSyntaxPostHint->setFixedHeight(SETTINGS_HEIGH);
	deprUrlSimplParserHint->setFixedHeight(SETTINGS_HEIGH);
	deprUrlAdvParserHint->setFixedHeight(SETTINGS_HEIGH);
	deprUrlBasParserHint->setFixedHeight(SETTINGS_HEIGH);
	compatilibtyUndrHint->setFixedHeight(SETTINGS_HEIGH);
	parseItalicHint->setFixedHeight(SETTINGS_HEIGH);
	parseBoldHint->setFixedHeight(SETTINGS_HEIGH);

	parseSimplLinks->setFixedHeight(SETTINGS_HEIGH);
	parseAdvLinksl->setFixedHeight(SETTINGS_HEIGH);
	parseHeaderLvl->setFixedHeight(SETTINGS_HEIGH);
	deprSyntaxPrep->setFixedHeight(SETTINGS_HEIGH);
	deprSyntaxPrep->setFixedHeight(SETTINGS_HEIGH);
	deprUrlSimplParser->setFixedHeight(SETTINGS_HEIGH);
	deprUrlAdvParser->setFixedHeight(SETTINGS_HEIGH);
	deprUrlBasParser->setFixedHeight(SETTINGS_HEIGH);
	combatilibtyUndr->setFixedHeight(SETTINGS_HEIGH);
	parseItalic->setFixedHeight(SETTINGS_HEIGH);
	parseBold->setFixedHeight(SETTINGS_HEIGH);

	//Настройки по умолчанию
	enableDeprFeatures == 0 ? deprSyntaxPrep->setChecked(0) : deprSyntaxPrep->setChecked(1);
	enableDeprFeatures == 0 ? deprSyntaxPost->setChecked(0) : deprSyntaxPost->setChecked(1);
	enableDeprFeatures == 0 ? deprUrlSimplParser->setChecked(0) : deprUrlSimplParser->setChecked(1);
	enableDeprFeatures == 0 ? deprUrlAdvParser->setChecked(0) : deprUrlAdvParser->setChecked(1);
	enableDeprFeatures == 0 ? deprUrlBasParser->setChecked(0) : deprUrlBasParser->setChecked(1);

	enableDeprFeatures == 0 ? parseSimplLinks->setChecked(1) : parseSimplLinks->setChecked(0);
	enableDeprFeatures == 0 ? parseAdvLinksl->setChecked(1) : parseAdvLinksl->setChecked(0);
	enableDeprFeatures == 0 ? parseHeaderLvl->setChecked(1) : parseHeaderLvl->setChecked(0);
	enableDeprFeatures == 0 ? parseItalic->setChecked(1) : parseItalic->setChecked(0);
	enableDeprFeatures == 0 ? parseBold->setChecked(1) : parseBold->setChecked(0);

	//В зависимости от прочитанного из конфига флага чекбосы либо активны либо нет
	enableDeprFeatures == 0 ? deprSyntaxPrep->setDisabled(1) : deprSyntaxPrep->setEnabled(1);
	enableDeprFeatures == 0 ? deprSyntaxPost->setDisabled(1) : deprSyntaxPost->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlSimplParser->setDisabled(1) : deprUrlSimplParser->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlAdvParser->setDisabled(1) : deprUrlAdvParser->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlBasParser->setDisabled(1) : deprUrlBasParser->setEnabled(1);

	enableDeprFeatures == 0 ? parseSimplLinks->setEnabled(1) : parseSimplLinks->setDisabled(1);
	enableDeprFeatures == 0 ? parseAdvLinksl->setEnabled(1) : parseAdvLinksl->setDisabled(1);
	enableDeprFeatures == 0 ? parseHeaderLvl->setEnabled(1) : parseHeaderLvl->setDisabled(1);
	enableDeprFeatures == 0 ? parseItalic->setEnabled(1) : parseItalic->setDisabled(1);
	enableDeprFeatures == 0 ? parseBold->setEnabled(1) : parseBold->setDisabled(1);

	//В зависимости от прочитанного из конфига флага подсказки либо активны либо нет
	enableDeprFeatures == 0 ? deprSyntaxPrepHint->setDisabled(1) : deprSyntaxPrepHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprSyntaxPostHint->setDisabled(1) : deprSyntaxPostHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlSimplParserHint->setDisabled(1) : deprUrlSimplParserHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlAdvParserHint->setDisabled(1) : deprUrlAdvParserHint->setEnabled(1);
	enableDeprFeatures == 0 ? deprUrlBasParserHint->setDisabled(1) : deprUrlBasParserHint->setEnabled(1);

	enableDeprFeatures == 0 ? parseSimplLinksHint->setEnabled(1) : parseSimplLinksHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseAdvLinksHint->setEnabled(1) : parseAdvLinksHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseHeaderLvlHint->setEnabled(1) : parseHeaderLvlHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseItalicHint->setEnabled(1) : parseItalicHint->setDisabled(1);
	enableDeprFeatures == 0 ? parseBoldHint->setEnabled(1) : parseBoldHint->setDisabled(1);

	//Инициализируем рамку
	QGroupBox* render_box = new QGroupBox;
	render_box->setAutoFillBackground(1);

	//Инициализируем рамки для подписей и элементов интерактивного взаимодействия
	QGroupBox* lbl_box = new QGroupBox;
	QGroupBox* interact_box = new QGroupBox;

	//Менеджеры размещения
	QVBoxLayout* lbl_lay = new QVBoxLayout;
	QVBoxLayout* interact_lay = new QVBoxLayout;

	//Вспомогательная компоновка
	QHBoxLayout* render_box_lay = new QHBoxLayout;
	QVBoxLayout* manager = new QVBoxLayout;

	//Добавляем в компоновку
	lbl_lay->addWidget(parseSimplLinksHint);
	lbl_lay->addWidget(parseAdvLinksHint);
	lbl_lay->addWidget(parseHeaderLvlHint);
	lbl_lay->addWidget(parseItalicHint);
	lbl_lay->addWidget(deprSyntaxPrepHint);
	lbl_lay->addWidget(deprSyntaxPostHint);
	lbl_lay->addWidget(deprUrlSimplParserHint);
	lbl_lay->addWidget(deprUrlAdvParserHint);
	lbl_lay->addWidget(deprUrlBasParserHint);
	lbl_lay->addWidget(compatilibtyUndrHint);
	lbl_lay->addWidget(parseBoldHint);

	//Подключение системы "Что это?"
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

	interact_lay->addWidget(parseSimplLinks);
	interact_lay->addWidget(parseAdvLinksl);
	interact_lay->addWidget(parseHeaderLvl);
	interact_lay->addWidget(parseItalic);
	interact_lay->addWidget(deprSyntaxPrep);
	interact_lay->addWidget(deprSyntaxPost);
	interact_lay->addWidget(deprUrlSimplParser);
	interact_lay->addWidget(deprUrlAdvParser);
	interact_lay->addWidget(deprUrlBasParser);
	interact_lay->addWidget(combatilibtyUndr);
	interact_lay->addWidget(parseBold);

	//Настраиваем порядок размещения элементов - сверху
	lbl_lay->setAlignment(Qt::AlignTop);
	interact_lay->setAlignment(Qt::AlignTop);

	//Привязываем менеджеры компоновки к виджетам
	lbl_box->setLayout(lbl_lay);
	interact_box->setLayout(interact_lay);

	//Задаем ширину блока
	lbl_box->setFixedWidth(HINTS_WIDTH);
	interact_box->setFixedWidth(INTERACT_WIDTH);

	//Компонуем виджеты с подсказками и элемантами взаимодействия
	//basic_box_lay->setAlignment(Qt::AlignLeft);
	render_box_lay->addWidget(lbl_box);
	render_box_lay->addWidget(interact_box);

	//Прикрепляем скомпонованные элементы
	render_box->setLayout(render_box_lay);
	manager->addWidget(render_box);
	manager->addWidget(reloadHint);

	renderSettings->setLayout(manager);
}
