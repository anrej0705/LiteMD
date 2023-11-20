#include "LiteMD.h"
#include <QtWidgets/QApplication>
#include <regex>
#include <string>

int main(int argc, char *argv[])
{
    QApplication mdApp(argc, argv);
    LiteMD sdiEditor;
	sdiEditor.resize(960, 576);
	sdiEditor.show();
    return mdApp.exec();
}
