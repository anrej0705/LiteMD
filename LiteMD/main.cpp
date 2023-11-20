#include "LiteMD.h"
#include <QtWidgets/QApplication>
#include <regex>
#include <string>

int main(int argc, char *argv[])
{
    QApplication mdApp(argc, argv);

	std::string mdInput{ "To be, or not to be, that is the question..." }; // исходная строка
	std::regex rex{ "(\\S+)" }; // регулярное выражение
	std::sregex_iterator beg{ mdInput.cbegin(), mdInput.cend(), rex }; // итератор
	// на начало последовательности найденных совпадений
	std::sregex_iterator end{};  // 'нулевой' итератор

    LiteMD sdiEditor;
	sdiEditor.show();
    return mdApp.exec();
}
