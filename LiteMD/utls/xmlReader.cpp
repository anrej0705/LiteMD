#include "xmlReader.h"
#include "global_definitions.h"
#include "exceptionHandler.h"
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

bool xmlReader::readConfig()
{
	QString value;
	QFile settings(fileName);
	bool readSuccess = 0;
	if (settings.open(QIODevice::ReadOnly))
	{
		QXmlStreamReader settingsReader(&settings);
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
						if (value.toInt() != BUILD_NUMBER)
							return 0;	//Обновляем конфиг из-за разницы в версиях

					}
					else
						readSuccess = 0;
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();

					if (settingsReader.tokenString() == "StartElement" && settingsReader.name() == "patchNoteRead")
					{
						settingsReader.readNext();
						settingsReader.readNext();
						settingsReader.readNext();
						//qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
						value = settingsReader.text().toString();
						if (value == QString("true"))
							logReadState = 1;
						else if (value == QString("false"))
							logReadState = 0;
						else
							return 0;	//Здесь и далее - возвращаем 0 если есть ошибки в чтении
					}
					else
						readSuccess = 0;
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();

					if (settingsReader.tokenString() == "StartElement" && settingsReader.name() == "enableIndevFeatures")
					{
						settingsReader.readNext();
						settingsReader.readNext();
						settingsReader.readNext();
						//qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
						value = settingsReader.text().toString();
						if (value == QString("true"))
							enableDeprFeatures = 1;
						else if (value == QString("false"))
							enableDeprFeatures = 0;
						else
							return 0;
					}
					else
						readSuccess = 0;
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();

					if (settingsReader.tokenString() == "StartElement" && settingsReader.name() == "enableDeprFeatures")
					{
						settingsReader.readNext();
						settingsReader.readNext();
						settingsReader.readNext();
						//qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
						value = settingsReader.text().toString();
						if (value == QString("true"))
							enableDeprFeatures = 1;
						else if (value == QString("false"))
							enableDeprFeatures = 0;
						else
							return 0;
					}
					else
						readSuccess = 0;

					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();
					settingsReader.readNext();

					if (settingsReader.tokenString() == "StartElement" && settingsReader.name() == "langCode")
					{
						settingsReader.readNext();
						settingsReader.readNext();
						settingsReader.readNext();
						//qDebug() << settingsReader.tokenString() << settingsReader.name() << settingsReader.text();
						value = settingsReader.text().toString();
						langCode = value.toInt();
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
	return readSuccess;
}
