#include "LiteMD.h"
#include <QtWidgets/QApplication>
#include <regex>
#include <string>

int main(int argc, char *argv[])
{
    QApplication mdApp(argc, argv);
	QApplication::setQuitOnLastWindowClosed(1);
	LiteMD sdiEditor;
	sdiEditor.resize(1280, 800);
	sdiEditor.show();
    return mdApp.exec();
}
