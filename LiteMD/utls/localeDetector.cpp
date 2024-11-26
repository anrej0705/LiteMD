#include "LiteMD.h"
#include "logger_backend.h"
#include <boost/container/string.hpp>
#include <map>

//Карта соответствия системного языка с его кодом в приложении
std::map<QString, int> localeMap{
	{"en_US", 0},
	{"ru_RU", 1},
	{"ru_UA", 2}
};

//Функция проверяет язык системы и задаёт язык приложения в соответствии
void LiteMD::localeDetector(void)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	QTranslator lmd_lgn;
	QString localeName = QLocale::system().name();
	
	log_stroke->append("[localeDetector]Обнаружен язык системы ");
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
