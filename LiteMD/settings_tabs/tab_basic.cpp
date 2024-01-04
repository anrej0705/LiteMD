#include "appSettings.h"
#include "tab_basic.h"

//��������� ����� ����� - ���_�����(���_�����)
QString localeNameConverter(QString lang_string_name, QString file_loc_name);

//������������� ���� ������� ��������
void appSettings::configureBasicSettingsTab()
{
	//�������������� ����
	basicSettings = new QWidget;

	//��������� ����������
	QHBoxLayout* manager = new QHBoxLayout;
	QVBoxLayout* elemManager = new QVBoxLayout;

	//�������������� ������ ��� �������� ������
	langList = new QComboBox;

	QLabel langlistHint("Language:");
	//langlistHint.setBuddy(langList);

	//������� ��������� ���� � ����������� ���� ���� �� ������ �����������
	QString lang_path = QApplication::applicationDirPath();
	lang_path.append("/loc");

	//��������� �� ������� ������
	QString current_lang = QLocale::system().name();

	//������ ��� ������������� ���������� loc
	QDir lang_dir(lang_path);

	//������ ������ �������
	QStringList available_langs = lang_dir.entryList(QStringList("LiteMD_*.qm"));

	loc_map = new std::map<uint8_t, QString>;

	for (uint8_t locales = 0; locales < static_cast<uint8_t>(available_langs.size()); ++locales)
	{
		QString locale_name = available_langs[locales];
		loc_map->insert(std::pair<uint8_t, QString>(locales, locale_name));
		locale_name.truncate(locale_name.lastIndexOf("."));
		locale_name.remove(0, locale_name.indexOf("_",0) + 1);
		QString locale = localeNameConverter(QLocale::languageToString(QLocale(locale_name).language()),locale_name);
		langList->addItem(locale);
		if (current_lang == locale_name)
			langList->setCurrentIndex(locales);
	}

	//������������� ������ ������ � ���������
	QHBoxLayout* langListManager = new QHBoxLayout;
	QLabel* langListHint = new QLabel(tr("Language"));
	langListManager->addWidget(langListHint);
	langListManager->addWidget(langList);

	elemManager->setAlignment(Qt::AlignTop);
	elemManager->addLayout(langListManager);
	manager->addLayout(elemManager);

	basicSettings->setLayout(manager);
}
QString localeNameConverter(QString lang_string_name, QString file_loc_name)
{
	//������� ����� � ������� ���������� ��������������� �������� ����� � ������� Qt, ������ � ��� �����
	//� ���������� ���������
	QString output = QObject::tr(lang_string_name.toLocal8Bit()) + "(" + file_loc_name + ")";
	return output;
}
void appSettings::slot_lang_selected(int lIndx)
{
	int langIndx = 0;
	langIndx = lIndx;
	auto it = loc_map->cbegin();
	std::advance(it, lIndx);
	QString lang_file = it->second;

	if(!lmd_lng.load("loc/"+ lang_file, "."))
		QErrorMessage::qtHandler();
}
