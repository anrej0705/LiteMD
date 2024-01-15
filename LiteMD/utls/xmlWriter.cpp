#include "xmlWriter.h"
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
	QDomElement patchNoteRead = attrib(*lmdFileSet, "patchNoteRead", logReadState);
	lmdSet->appendChild(patchNoteRead);
}

xmlWriter::xmlWriter(QString fName)
{
	fileName = fName;
	lmdFileSet = new QDomDocument("LMD");
	lmdSet = new QDomElement(lmdFileSet->createElement(appSign));
	lmdFileSet->appendChild(*lmdSet);
	QDomElement patchNoteRead = attrib(*lmdFileSet, "patchNoteRead", logReadState);
	lmdSet->appendChild(patchNoteRead);
}

void xmlWriter::writeConfig()
{
	delete lmdFileSet;
	delete lmdSet;
	lmdFileSet = new QDomDocument("LMD");
	lmdSet = new QDomElement(lmdFileSet->createElement(appSign));
	lmdFileSet->appendChild(*lmdSet);
	QDomElement patchNoteRead = attrib(*lmdFileSet, "patchNoteRead", logReadState);
	lmdSet->appendChild(patchNoteRead);
	QFile settingsFile(fileName);
	if (settingsFile.open(QIODevice::WriteOnly))
	{
		QTextStream(&settingsFile) << lmdFileSet->toString();
		settingsFile.close();
	}
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
