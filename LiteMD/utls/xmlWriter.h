#pragma once
#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>

class xmlWriter
{
	private:
		QString fileName;
		QDomDocument* lmdFileSet;
		QDomElement* lmdSet;
		QDomElement attrib(QDomDocument& setDoc, const QString& attribName, const QVariant& attrType);
		QDomElement makeElement(QDomDocument& setDoc, const QString& attribName, const QString& attribType = QString(), const QString& value = QString());
	public:
		xmlWriter();
		xmlWriter(QString fName);
		void writeConfig();
};
