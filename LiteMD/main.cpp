#include "LiteMD.h"
#include "logger_backend.h"
#include <QtWidgets/QApplication>
#include <regex>
#include <string>

int main(int argc, char *argv[])
{
	std::string init_log;
	push_log("[ЛОГ]Инициализация системы логов");
	init_log.append("[MAIN]Получено аргументов: ");
	init_log.append(std::to_string(argc) + " ");
	for (int8_t _arg = 0; _arg < argc; ++_arg)
	{
		init_log.append("[");
		init_log.append(argv[_arg]);
		init_log.append("]");
	}
	push_log(init_log);
    QApplication mdApp(argc, argv);
	QApplication::setQuitOnLastWindowClosed(1);
	LiteMD sdiEditor(argc, argv);
	sdiEditor.resize(1280, 800);
	sdiEditor.show();
    return mdApp.exec();
}
