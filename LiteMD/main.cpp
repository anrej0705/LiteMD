#include "LiteMD.h"
#include "logger_backend.h"
#include "update_manager.h"
#include <fstream>
#include <QtWidgets/QApplication>
#include <regex>
#include <string>
#include <boost/chrono/chrono.hpp>
#include <boost/thread/thread.hpp>

QStringList patches;

int main(int argc, char *argv[])
{	//Впадлу описывать
	QApplication mdApp(argc, argv);		//Собсно объект приложухи
	push_log("[ЛОГ]Инициализация системы логов");
	std::string init_log;				//Для хранения количества полученых аргументов

	std::string myName = QFileInfo(QCoreApplication::applicationFilePath()).fileName().toStdString();

	push_log(std::string("[MAIN]Запущен: " + myName));

	//push_log("[MAIN]Проверка наличия артефактов");

	if (myName.find("_old") == -1)	//Если имя запускаемого приложения не содержит суффикса то попытаемся удалить старый файл
	{
		/*myName.insert(myName.find("."), "_old");	//Вставляем суффикс
		myName.insert(0, QCoreApplication::applicationDirPath().toStdString() + "/");
		push_log(std::string("[MAIN]Пытаюсь удалить артефакт ") + myName);
		if (std::remove(myName.c_str()) != 0)
		{
			push_log("[MAIN]Не удалось, вторая попытка через 2 секунды");
			boost::this_thread::sleep_for(boost::chrono::nanoseconds(10));
			boost::this_thread::sleep_until(boost::chrono::system_clock::now() + boost::chrono::seconds(2));
			if (std::remove(myName.c_str()) != 0)
			{
				push_log("[MAIN]Не удалось, вторая попытка через 5 секунд");
				boost::this_thread::sleep_for(boost::chrono::nanoseconds(10));
				boost::this_thread::sleep_until(boost::chrono::system_clock::now() + boost::chrono::seconds(5));
				if (std::remove(myName.c_str()) != 0)
					push_log("[MAIN]Не удалось, удали вручную");
			}
			std::ifstream oldFile(myName);
			if (oldFile.bad())
			{
				//QProcess* update_prc = new QProcess();	//Создаём процесс и запускаем его не дожидаясь
				/*myName = QFileInfo(QCoreApplication::applicationFilePath()).fileName().toStdString();
				LPCSTR lPath(std::string(QCoreApplication::applicationDirPath().toLocal8Bit() + myName.c_str()).c_str());
				LPSTR null = const_cast<char*>("");
				STARTUPINFO update = { sizeof(update) };
				PROCESS_INFORMATION proc_info;
				if (CreateProcess(lPath, null, NULL, NULL, TRUE, 0, NULL, NULL, &update, &proc_info))
				{
					CloseHandle(proc_info.hProcess);
					CloseHandle(proc_info.hThread);
				}
				else
					qDebug() << GetLastError();
				//system(QCoreApplication::applicationDirPath().toLocal8Bit() + myName.c_str());
				//update_prc->startDetached(QString::fromStdString(myName));
				exit(0);
			}
		}*/
	}

	//Разговорчики в чЯтике
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

	old_file.close();

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
