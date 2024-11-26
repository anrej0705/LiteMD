#include "LiteMD.h"
#include "logger_backend.h"
#include <QtWidgets/QApplication>
#include <regex>
#include <string>

int main(int argc, char *argv[])
{
	push_log("[ЛОГ]Инициализация системы логов");
    QApplication mdApp(argc, argv);
	QApplication::setQuitOnLastWindowClosed(1);
	LiteMD sdiEditor;
	sdiEditor.resize(1280, 800);
	sdiEditor.show();
    return mdApp.exec();
}
