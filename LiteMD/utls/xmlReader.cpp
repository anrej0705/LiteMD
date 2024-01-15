#include "xmlReader.h"
#include "global_definitions.h"
extern "C"
{
	#include "globalFlags.h"
}
xmlReader::xmlReader()
{
	fileName = "config.xml";
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

void xmlReader::readConfig()
{
	QString value;
	QFile settings(fileName);
	if (settings.open(QIODevice::ReadOnly))
	{
		QXmlStreamReader settingsReader(&settings);
		settingsReader.readNext();
		settingsReader.readNext();
		qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
		if (settingsReader.text() == QString("<!DOCTYPE LMD>"))
		{
			settingsReader.readNext();
			if (settingsReader.name() == appSign)	//Ищем сигнатуру __Shani_basic
			{
				settingsReader.readNext();
				do {
					settingsReader.readNext();
					if (settingsReader.tokenString() == "StartElement" && settingsReader.name() == "patchNoteRead")
					{
						settingsReader.readNext();
						settingsReader.readNext();
						settingsReader.readNext();
						qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
						value = settingsReader.text().toString();
						if (value == QString("true"))
							logReadState = 1;
						else if (value == QString("false"))
							logReadState = 0;
						else
							break;
					}
				} while (!settingsReader.atEnd());
			}
		}
	}
	settings.close();	//Закрываем файл чтобы освободить дескриптор
}
