#include "LiteMD.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
    LiteMD w;
    w.show();
    return a.exec();
}
