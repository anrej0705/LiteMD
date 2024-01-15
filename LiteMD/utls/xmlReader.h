#pragma once
#include <qdom.h>
#include <qfile.h>
#include <qxmlstream.h>
#include <qdebug.h>

class xmlReader
{
	private:
		QString fileName;
	public:
		xmlReader();
		xmlReader(QString filename);
		bool checkFileExisting();
		void readConfig();
};
