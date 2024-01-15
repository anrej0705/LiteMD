#include "xmlReader.h"
#include "global_definitions.h"
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

}
