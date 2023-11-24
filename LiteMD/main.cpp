#include "LiteMD.h"
#include <QtWidgets/QApplication>
#include <regex>
#include <string>

int main(int argc, char *argv[])
{
    QApplication mdApp(argc, argv);
	QTranslator lmd_lgn;
	QString localeName = QLocale::system().name();
	if (localeName == "en_US")
	{
		if (!lmd_lgn.load("loc\\LiteMD_en.qm", "."))
			QErrorMessage::qtHandler();
	}
	else if (localeName == "ru_RU")
	{
		if (!lmd_lgn.load("loc\\LiteMD_ru.qm", "."))
			QErrorMessage::qtHandler();
	}
	else if (localeName == "ru_UA")
	{
		if (!lmd_lgn.load("loc\\LiteMD_ru_UA.qm", "."))
			QErrorMessage::qtHandler();
	}
	if (!mdApp.installTranslator(&lmd_lgn))
		QErrorMessage::qtHandler();
	QApplication::setQuitOnLastWindowClosed(1);
	LiteMD sdiEditor;
	sdiEditor.resize(1280, 800);
	sdiEditor.show();
    return mdApp.exec();
}
