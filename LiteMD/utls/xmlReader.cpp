#include "xmlReader.h"
#include "exceptionHandler.h"
#include "event_id_constructor.h"
#include "logger_backend.h"
#include <boost/container/vector.hpp>
#include "LiteMD.h"
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}
extern QString appPath;	//Каталог, в котором лежит исполняемый фаил приложения
extern struct parser_switchers parswitch;
extern struct depr_paerser_switchers dparswitch;
xmlReader::xmlReader()
{
	fileName = getConfigPath() + "/config.xml";
	boost::container::string* log_out = new boost::container::string("[XML]Задано имя файла конфига для записи ");
	log_out->append(fileName.toLocal8Bit());
	push_log(log_out->c_str());	//xmlWriter.cpp 17:20
}

xmlReader::xmlReader(QString fName)
{
	fileName = fName;
}

bool xmlReader::checkFileExisting()
{
	QFile check(fileName);
	if (check.open(QIODevice::ReadOnly))
		return 1;
	return 0;
}
//Счётчик количества успешно прочитанных параметров
//если всё гуд то должен ровняться макросу PARAM_CNT из global_definitions.h
uint8_t paramReadedCnt = 0;

//Списочек на выдрачивание из конфига, будет пополняться
boost::container::vector<QString> xml_tags{
	QString("patchNoteRead"),
	QString("enableIndevFeatures"),
	QString("enableDeprFeatures"),
	QString("langCode"),
	QString("enBasicUrlParse"),
	QString("enAdvUrlParse"),
	QString("enHeaderLvlParse"),
	QString("depr_en_t_post"),
	QString("depr_en_t_prep"),
	QString("depr_en_url_adv"),
	QString("depr_en_url_bas"),
	QString("depr_en_url_bas_simple"),
	QString("enStrikethroughHintParse"),
	QString("logs_limit"),
	QString("parseUnderlined"),
	QString("combatilibtyUndr"),
	QString("en_italic"),
	QString("en_bold"),
	QString("dataLocation"),
};

bool xmlReader::readConfig()
{
	static bool read_retry;

	read_retry == 0 ? push_log("[XML]Чтение конфига") : push_log("[XML]Чтение конфига, повторная попытка");

	//"Просто поверь мне" - говорил Ферми, запуская свой первый ядерный реактор
	static boost::container::vector<void*> _xml_ptr;	//Ебанёт? Не должно по идее
	_xml_ptr.push_back(reinterpret_cast<void*>(&logReadState));
	_xml_ptr.push_back(reinterpret_cast<void*>(&enableIndevFeatures));
	_xml_ptr.push_back(reinterpret_cast<void*>(&enableDeprFeatures));
	_xml_ptr.push_back(reinterpret_cast<void*>(&langCode));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_simple_url));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_adv_url));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_header_lvl));
	_xml_ptr.push_back(reinterpret_cast<void*>(&dparswitch.en_t_post));
	_xml_ptr.push_back(reinterpret_cast<void*>(&dparswitch.en_t_prep));
	_xml_ptr.push_back(reinterpret_cast<void*>(&dparswitch.en_url_adv));
	_xml_ptr.push_back(reinterpret_cast<void*>(&dparswitch.en_url_bas));
	_xml_ptr.push_back(reinterpret_cast<void*>(&dparswitch.en_url_bas_simple));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_ex_strkthg));
	_xml_ptr.push_back(reinterpret_cast<void*>(&log_limit));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_underlined));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_compat_undr));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_italic));
	_xml_ptr.push_back(reinterpret_cast<void*>(&parswitch.en_bold));
	_xml_ptr.push_back(reinterpret_cast<void*>(&dataLocation));

	int* _int_ptr;
	bool* _bool_ptr;

	bool old_ver = 0;

	QString value;
	QFile settings(fileName);
	bool readSuccess = 0;
	if (settings.open(QIODevice::ReadOnly))
	{
		QXmlStreamReader settingsReader(&settings);
		QXmlStreamAttributes attr = settingsReader.attributes();
		settingsReader.readNext();
		settingsReader.readNext();
		//qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
		if (settingsReader.text() == QString("<!DOCTYPE LMD>"))
		{
			settingsReader.readNext();
			if (settingsReader.name() == appSign)	//Ищем сигнатуру __Shani_basic
			{
				settingsReader.readNext();
				do {
					settingsReader.readNext();
					if (settingsReader.tokenString() == "StartElement" && settingsReader.name() == "build")
					{
						readSuccess = 1;
						settingsReader.readNext();
						settingsReader.readNext();
						settingsReader.readNext();
						//qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
						value = settingsReader.text().toString();
						if (value.toInt() < BUILD_NUMBER)
						{
							push_log(std::string("[XML]Обнаружена устаревшая версия конфига " + std::to_string(value.toInt()) + "(текущая версия " + std::to_string(BUILD_NUMBER)).c_str());
							push_log("[XML]Будет проведена попытка импорта некоторых настроек в новый конфиг");
							old_ver = 1;
						}
						for (uint8_t _xml_index = 0; _xml_index < PARAM_CNT; ++_xml_index)
						{
							settingsReader.readNext();
							settingsReader.readNext();
							settingsReader.readNext();
							settingsReader.readNext();
							settingsReader.readNext();
							if (settingsReader.tokenString() == "StartElement")
							{
								if (settingsReader.name().toString() == xml_tags[_xml_index])
								{
									settingsReader.readNext();
									settingsReader.readNext();
									attr = settingsReader.attributes();
									//qDebug() << attr[0].value();;
									settingsReader.readNext();
									value = settingsReader.text().toString();

									//Типа свич но не свич потому string видимо
									//евреи писали, я доложил гитлеру что кейс тут не работает
									if (attr[0].value() == "bool")
									{
										_bool_ptr = reinterpret_cast<bool*>(_xml_ptr[_xml_index]);
										if (value == QString("true"))
											*_bool_ptr = 1;
										else
											*_bool_ptr = 0;
									}
									else if (attr[0].value() == "int")
									{
										_int_ptr = reinterpret_cast<int*>(_xml_ptr[_xml_index]);
										*_int_ptr = value.toInt();
										if (*_int_ptr != (unsigned int)value.toInt())
											throw(exceptionHandler::WARNING, "Возможное повреждение данных при чтении");
									}
									++paramReadedCnt;
									readSuccess = 1;
								}
								else
								{	//Если прочитанный параметр не равен параметру из ветора
									//считаем что структура повреждена и тiкаем отсюда
									readSuccess = 0;
									break;
								}
							}
							//Если внезапно закончились теги то считаем что часть прочитана
							else
							{
								readSuccess = 0;
								break;
							}
						}
					}
					else
						readSuccess = 0;
					break;

				} while (!settingsReader.atEnd());
			}
		}
	}
	else
		throw(exceptionHandler(exceptionHandler::WARNING, QObject::tr("Cannot open config file!")));
	settings.close();	//Закрываем файл чтобы освободить дескриптор
	push_log(std::string("[XML]Прочитано " + std::to_string(paramReadedCnt) + " параметров из " + std::to_string(PARAM_CNT)).c_str());
	if (paramReadedCnt != PARAM_CNT)
	{
		push_log("[XML]Недостающие параметры будут записаны со значением по умолчанию");
		read_retry = 1;
		readSuccess = 0;
	}
	if (old_ver)
		readSuccess = 0;
	if (!QCoreApplication::sendEvent(qApp, new event_id_constructor(APP_EVENT_UI_UPDATE_USER_SETTINGS)))
		QErrorMessage::qtHandler();//Отправка события на обновление визуала - галочек, радиокнопок и прочего
	paramReadedCnt = 0;
	return readSuccess;
}
