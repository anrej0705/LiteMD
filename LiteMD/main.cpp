#include "LiteMD.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication mdApp(argc, argv);
    LiteMD sdiEditor;
	sdiEditor.show();
    return mdApp.exec();
}
