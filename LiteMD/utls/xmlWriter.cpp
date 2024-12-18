#include "xmlWriter.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
#include <boost/container/string.hpp>
#include "LiteMD.h"
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}
extern struct parser_switchers parswitch;
extern struct depr_paerser_switchers dparswitch;
xmlWriter::xmlWriter()
{
	fileName = getConfigPath() + "/config.xml";
	boost::container::string* log_out = new boost::container::string("[XML]Задано имя файла конфига для чтения ");
	log_out->append(fileName.toLocal8Bit());
	push_log(log_out->c_str());	//Кароч я реально нихуя не понимаю почему std string ебёт мозга в дебаге
								//каким-то критом, который я никогда в жизни не видел. Крит выбрасывается
								//ещё до передачи аргумента в саму функцию логера. Вижла ебанулась, опять
								//Бустовый string типа решил проблему
}

xmlWriter::xmlWriter(QString fName)
{
	fileName = fName;
}

void xmlWriter::writeConfig()
{
	//delete lmdFileSet;
	//delete lmdSet;
	push_log("[XML]Запись конфига");
	lmdFileSet = new QDomDocument("LMD");
	lmdSet = new QDomElement(lmdFileSet->createElement(appSign));
	lmdFileSet->appendChild(*lmdSet);
	QDomElement build = attrib(*lmdFileSet, "build", BUILD_NUMBER);
	QDomElement patchNoteRead = attrib(*lmdFileSet, "patchNoteRead", logReadState);
	QDomElement indevFeatures = attrib(*lmdFileSet, "enableIndevFeatures", enableIndevFeatures);
	QDomElement deprFeatures = attrib(*lmdFileSet, "enableDeprFeatures", enableDeprFeatures);
	QDomElement langCodeParam = attrib(*lmdFileSet, "langCode", langCode);
	QDomElement enBasicUrlParse = attrib(*lmdFileSet, "enBasicUrlParse", parswitch.en_simple_url);
	QDomElement enAdvUrlParse = attrib(*lmdFileSet, "enAdvUrlParse", parswitch.en_adv_url);
	QDomElement enHeaderLvlParse = attrib(*lmdFileSet, "enHeaderLvlParse", parswitch.en_header_lvl);
	QDomElement en_t_post = attrib(*lmdFileSet, "depr_en_t_post", dparswitch.en_t_post);
	QDomElement en_t_prep = attrib(*lmdFileSet, "depr_en_t_prep", dparswitch.en_t_prep);
	QDomElement en_url_adv = attrib(*lmdFileSet, "depr_en_url_adv", dparswitch.en_url_adv);
	QDomElement en_url_bas = attrib(*lmdFileSet, "depr_en_url_bas", dparswitch.en_url_bas);
	QDomElement en_url_bas_simple = attrib(*lmdFileSet, "depr_en_url_bas_simple", dparswitch.en_url_bas_simple);
	QDomElement enStrikethroughHintParse = attrib(*lmdFileSet, "enStrikethroughHintParse", parswitch.en_ex_strkthg);
	QDomElement logs_limit = attrib(*lmdFileSet, "logs_limit", log_limit);
	QDomElement parseUnderlined = attrib(*lmdFileSet, "parseUnderlined", parswitch.en_underlined);
	QDomElement combatilibtyUndr = attrib(*lmdFileSet, "combatilibtyUndr", parswitch.en_compat_undr);
	QDomElement en_italic = attrib(*lmdFileSet, "en_italic", parswitch.en_italic);
	QDomElement en_bold = attrib(*lmdFileSet, "en_bold", parswitch.en_bold);
	QDomElement data_location = attrib(*lmdFileSet, "dataLocation", dataLocation);
	lmdSet->appendChild(build);
	lmdSet->appendChild(patchNoteRead);
	lmdSet->appendChild(indevFeatures);
	lmdSet->appendChild(deprFeatures);
	lmdSet->appendChild(langCodeParam);
	lmdSet->appendChild(enBasicUrlParse);
	lmdSet->appendChild(enAdvUrlParse);
	lmdSet->appendChild(enHeaderLvlParse);
	lmdSet->appendChild(en_t_post);
	lmdSet->appendChild(en_t_prep);
	lmdSet->appendChild(en_url_adv);
	lmdSet->appendChild(en_url_bas);
	lmdSet->appendChild(en_url_bas_simple);
	lmdSet->appendChild(enStrikethroughHintParse);
	lmdSet->appendChild(logs_limit);
	lmdSet->appendChild(parseUnderlined);
	lmdSet->appendChild(combatilibtyUndr);
	lmdSet->appendChild(en_italic);
	lmdSet->appendChild(en_bold);
	lmdSet->appendChild(data_location);
	QFile settingsFile(fileName);
	if (settingsFile.open(QIODevice::WriteOnly))
	{
		QTextStream(&settingsFile) << lmdFileSet->toString();
		settingsFile.close();
		settingChanged = 0;
	}
	else
		throw(exceptionHandler(exceptionHandler::FATAL));
}

QDomElement xmlWriter::attrib(QDomDocument& setDoc, const QString& attribName, const QVariant& attrType)
{
	//switch-type
	if (attrType.typeName() == QString("bool"))
	{
		QDomElement settingsElement = makeElement(setDoc, attribName);
		settingsElement.appendChild(makeElement(setDoc, "parameter", "bool", attrType.toString()));
		return settingsElement;
	}
	else if (attrType.typeName() == QString("int"))
	{
		QDomElement settingsElement = makeElement(setDoc, attribName);
		settingsElement.appendChild(makeElement(setDoc, "parameter", "int", attrType.toString()));
		return settingsElement;
	}
	else if (attrType.typeName() == QString("QString"))
	{
		QDomElement settingsElement = makeElement(setDoc, attribName);
		settingsElement.appendChild(makeElement(setDoc, "parameter", "string", attrType.toString()));
		return settingsElement;
	}
	return QDomElement();
}

QDomElement xmlWriter::makeElement(QDomDocument& setDoc, const QString& attribName, const QString& attribType, const QString& value)
{
	QDomElement element = setDoc.createElement(attribName);
	if (!attribType.isEmpty())
	{
		QDomAttr elementAttr = setDoc.createAttribute("type");
		elementAttr.setValue(attribType);
		element.setAttributeNode(elementAttr);
	}
	if (!value.isEmpty())
	{
		QDomText attribString = setDoc.createTextNode(value);
		element.appendChild(attribString);
	}
	return element;
}
