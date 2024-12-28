#include "LiteMD.h"
#include "logger_backend.h"
#include <QtWidgets/QApplication>
#include <regex>
#include <string>
#include "qmicroz.h"	//DLL!!!

QStringList patches;

int main(int argc, char *argv[])
{	//Впадлу описывать
	QApplication mdApp(argc, argv);		//Собсно объект приложухи
	std::string init_log;				//Для хранения количества полученых аргументов

	//Разговорчики в чЯтике
	push_log("[ЛОГ]Инициализация системы логов");
	push_log("[ОБНОВЛЕНИЕ]Проверка обновлений");

	//Будущая супердуперфича
	patches = QDir(QCoreApplication::applicationDirPath() + "/").entryList(QStringList("patch_*.zip"));
	if (!patches.isEmpty())
	{
		push_log(std::string("[ОБНОВЛЕНИЕ]Найден архив с патчем " + patches.at(0).toStdString()));

		//Здесь патчер начинает свою работу
	}
	else
		push_log("[ОБНОВЛЕНИЕ]Патчи не найдены, пропуск");

	//Отписываеся о что получили аргументы и перечисляем их
	init_log.append("[MAIN]Получено аргументов: ");
	init_log.append(std::to_string(argc) + " ");
	for (int8_t _arg = 0; _arg < argc; ++_arg)
	{
		init_log.append("[");
		init_log.append(argv[_arg]);
		init_log.append("]");
	}
	//Кидаем лог
	push_log(init_log);

	//Прога закрывается только если закрыто последнее окно
	QApplication::setQuitOnLastWindowClosed(1);
	LiteMD sdiEditor(argc, argv);	//В будущем добавить возможность задать размер из аргументов
	sdiEditor.resize(1280, 800);

	//Начинаем движ
	sdiEditor.show();
    return mdApp.exec();
}
