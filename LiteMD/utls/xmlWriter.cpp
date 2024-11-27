#include "xmlWriter.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}
extern struct parser_switchers parswitch;
xmlWriter::xmlWriter()
{
	fileName = "config.xml";
	push_log(std::string("[XML]Задано имя файла конфига для чтения " + fileName.toStdString()).c_str());
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
	lmdSet->appendChild(build);
	lmdSet->appendChild(patchNoteRead);
	lmdSet->appendChild(indevFeatures);
	lmdSet->appendChild(deprFeatures);
	lmdSet->appendChild(langCodeParam);
	lmdSet->appendChild(enBasicUrlParse);
	lmdSet->appendChild(enAdvUrlParse);
	lmdSet->appendChild(enHeaderLvlParse);
	QFile settingsFile(fileName);
	if (settingsFile.open(QIODevice::WriteOnly))
	{
		QTextStream(&settingsFile) << lmdFileSet->toString();
		settingsFile.close();
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
