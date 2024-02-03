#include "xmlWriter.h"
#include "exceptionHandler.h"
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}

xmlWriter::xmlWriter()
{
	fileName = "config.xml";
	lmdFileSet = new QDomDocument("LMD");
	lmdSet = new QDomElement(lmdFileSet->createElement(appSign));
	lmdFileSet->appendChild(*lmdSet);
	QDomElement build = attrib(*lmdFileSet, "build", BUILD_NUMBER);
	QDomElement patchNoteRead = attrib(*lmdFileSet, "patchNoteRead", logReadState);
	QDomElement indevFeatures = attrib(*lmdFileSet, "enableIndevFeatures", enableIndevFeatures);
	QDomElement deprFeatures = attrib(*lmdFileSet, "enableDeprFeatures", enableDeprFeatures);
	lmdSet->appendChild(build);
	lmdSet->appendChild(patchNoteRead);
	lmdSet->appendChild(indevFeatures);
	lmdSet->appendChild(deprFeatures);
}

xmlWriter::xmlWriter(QString fName)
{
	fileName = fName;
	lmdFileSet = new QDomDocument("LMD");
	lmdSet = new QDomElement(lmdFileSet->createElement(appSign));
	lmdFileSet->appendChild(*lmdSet);
	QDomElement build = attrib(*lmdFileSet, "build", BUILD_NUMBER);
	QDomElement patchNoteRead = attrib(*lmdFileSet, "patchNoteRead", logReadState);
	QDomElement indevFeatures = attrib(*lmdFileSet, "enableIndevFeatures", enableIndevFeatures);
	QDomElement deprFeatures = attrib(*lmdFileSet, "enableDeprFeatures", enableDeprFeatures);
	lmdSet->appendChild(build);
	lmdSet->appendChild(patchNoteRead);
	lmdSet->appendChild(indevFeatures);
	lmdSet->appendChild(deprFeatures);
}

void xmlWriter::writeConfig()
{
	delete lmdFileSet;
	delete lmdSet;
	lmdFileSet = new QDomDocument("LMD");
	lmdSet = new QDomElement(lmdFileSet->createElement(appSign));
	lmdFileSet->appendChild(*lmdSet);
	QDomElement build = attrib(*lmdFileSet, "build", BUILD_NUMBER);
	QDomElement patchNoteRead = attrib(*lmdFileSet, "patchNoteRead", logReadState);
	QDomElement indevFeatures = attrib(*lmdFileSet, "enableIndevFeatures", enableIndevFeatures);
	QDomElement deprFeatures = attrib(*lmdFileSet, "enableDeprFeatures", enableDeprFeatures);
	lmdSet->appendChild(build);
	lmdSet->appendChild(patchNoteRead);
	lmdSet->appendChild(indevFeatures);
	lmdSet->appendChild(deprFeatures);
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
