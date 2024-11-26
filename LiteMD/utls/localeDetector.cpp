#include "LiteMD.h"
#include "logger_backend.h"
#include <boost/container/string.hpp>
#include <map>

//����� ������������ ���������� ����� � ��� ����� � ����������
std::map<QString, int> localeMap{
	{"en_US", 0},
	{"ru_RU", 1},
	{"ru_UA", 2}
};

//������� ��������� ���� ������� � ����� ���� ���������� � ������������
void LiteMD::localeDetector(void)
{	//��������� ��� ������� ���� ����� ��������� � ����
	boost::container::string* log_stroke = new boost::container::string;

	QTranslator lmd_lgn;
	QString localeName = QLocale::system().name();
	
	log_stroke->append("[localeDetector]��������� ���� ������� ");
	log_stroke->append(localeName.toLocal8Bit());
	push_log(log_stroke->c_str());

	if (localeName == "en_US")
		mdlSet->slot_lang_selected(localeMap[localeName]);
	else if (localeName == "ru_RU")
		mdlSet->slot_lang_selected(localeMap[localeName]);
	else if (localeName == "ru_UA")
		mdlSet->slot_lang_selected(localeMap[localeName]);

	delete(log_stroke);
}
