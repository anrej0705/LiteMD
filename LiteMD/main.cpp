#include "LiteMD.h"
#include "logger_backend.h"
#include "update_manager.h"
#include <fstream>
#include <QtWidgets/QApplication>
#include <regex>
#include <string>

QStringList patches;

int main(int argc, char *argv[])
{	//Впадлу описывать
	QApplication mdApp(argc, argv);		//Собсно объект приложухи
	std::string init_log;				//Для хранения количества полученых аргументов

	//Разговорчики в чЯтике
	push_log("[ЛОГ]Инициализация системы логов");
	push_log("[ПРОВЕРКА ОБНОВЛЕНИЙ]Проверка обновлений");

	//Прикрепляемся в старому файлу
	std::ifstream old_file(std::string(QCoreApplication::applicationDirPath().toStdString() + "/" + "LiteMD_old.exe"));

	//Будущая супердуперфича
	patches = QDir(QCoreApplication::applicationDirPath() + "/").entryList(QStringList("patch_*.zip"));
	if (!patches.isEmpty() && old_file.good())
	{
		push_log(std::string("[ПРОВЕРКА ОБНОВЛЕНИЙ]Найден архив с патчем " + patches.at(0).toStdString()));
		
		//Здесь патчер начинает свою работу и получает в качестве аргумента найденый архив
		update_manager mngr(patches.at(0));
		mngr.show();
		return mdApp.exec();
	}
	else
		push_log("[ПРОВЕРКА ОБНОВЛЕНИЙ]Патчи не найдены, пропуск");

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
