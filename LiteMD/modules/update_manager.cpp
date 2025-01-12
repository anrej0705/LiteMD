#include <fstream>
#include <filesystem>
#include "update_manager.h"
#include "logger_backend.h"
#include "WinAPI_utils.h"
extern "C"
{
	#include "global_definitions.h"
}
#pragma comment(lib,"qmicroz.lib")	//При работе с dll обязательно(!!) прописывать

uint16_t exec_commands = 0;

//Флаги, по умолчанию
bool start_lmd = 1;
bool delete_patch_bit = 1;

//Для автономности класса все нужные методы и функции отдельно скопированы сюда

//Карта соответствия системного языка с его кодом в приложении
std::map<QString, int> llocaleMap{
	{"en_US", 0},
	{"ru_RU", 1},
	{"ru_UA", 2}
};

std::map<std::string, uint8_t> comand_set{
	{"ren", 0},
	{"del", 1},
	{"move", 2},
	{"copy", 3},
	{"md", 4},
	{"dd", 5},
	{"log", 6},
	{"lmd_begin", 254},
	{"lmd_end", 255}
};

QString llocaleNameConverter(QString lang_string_name, QString file_loc_name)
{
	//Создаем буфер в который прилепляем последовательно название языка в системе Qt, скобки и код языка
	//И возвращаем результат
	QString output = QObject::tr(lang_string_name.toLocal8Bit()) + "(" + file_loc_name + ")";
	return output;
}

inline QIcon setIcon()
{
	//Подгружаем иконку
	QPixmap input(QCoreApplication::applicationDirPath() + "/ress/icon_about.png");
	QPixmap appIcon(input.size());	//Создаём объект в памяти по размеру картинки
	appIcon.fill(Qt::transparent);	//Заливаем прозрачным(делаем альфа канал)
	QPainter p(&appIcon);			//Линкуем нашу загруженную иконку
	p.setOpacity(1);				//Врубаем альфа-канал
	p.drawPixmap(0, 0, input);		//Рисуем картинку с альфа-каналом
	p.end();
	return appIcon;
}

update_manager::update_manager(QString p_name, QWidget* uWgt) : QDialog(uWgt)
{
	push_log("[МЕНЕДЖЕР ОБНОВЛЕНИЙ]Инициализация");
	setModal(1);

	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	//Получаем номер текущей сборки
	curBuild = BUILD_NUMBER;

	//Получаем название архива с патчем
	patch_name = p_name;

	//Выдёргиваем номер билда из названия архива
	QString buildNum = patch_name.split("_")[1];
	buildNum = buildNum.split(".")[0];
	fndBuild = QString(buildNum).toInt();

	//Хардкодим папку и каталог
	appMainPath = QApplication::applicationDirPath().toStdString();
	working_folder = appMainPath + "/patch";

	//Так надёжнее
	log_stroke->append("[МЕНЕДЖЕР ОБНОВЛЕНИЙ]Текущая сборка ");
	log_stroke->append(std::to_string(curBuild).c_str());
	log_stroke->append(". Найденый архив патча ");
	log_stroke->append(patch_name.toLocal8Bit());
	log_stroke->append("\n");

	//Устанавливаем фильтр события для отработки события закрытия приложения
	qApp->installEventFilter(this);

	//----------------------------------------------------------------------------
	//
	//Здесь и далее работа с локализацией взята из appSettings.cpp и tab_basic.cpp
	//
	//----------------------------------------------------------------------------

	QString localeName = QLocale::system().name();

	//Создаем контейнер пути и прописываем туда путь до файлов локализации
	QString lang_path = QApplication::applicationDirPath();
	lang_path.append("/loc");

	//Указатель на текущую локаль
	QString current_lang = QLocale::system().name();

	//Объект для представления директории loc
	QDir lang_dir(lang_path);

	//Список файлов локалей
	QStringList available_langs = lang_dir.entryList(QStringList("LiteMD_*.qm"));

	if (!available_langs.isEmpty())
	{
		lloc_map = new std::map<uint8_t, QString>;

		push_log(std::string("[LOCALE]Найдено " + std::to_string(static_cast<uint8_t>(available_langs.size())) + " файлов локализаций"));

		for (uint8_t locales = 0; locales < static_cast<uint8_t>(available_langs.size()); ++locales)
		{
			QString locale_name = available_langs[locales];
			lloc_map->insert(std::pair<uint8_t, QString>(locales, locale_name));
			locale_name.truncate(locale_name.lastIndexOf("."));
			locale_name.remove(0, locale_name.indexOf("_", 0) + 1);
			QString locale = llocaleNameConverter(QLocale::languageToString(QLocale(locale_name).language()), locale_name);
			push_log(std::string("[LOCALE]Загружена локаль " + locale.toStdString()));
			/*if (current_lang == locale_name)
				langList->setCurrentIndex(locales); */
		}
	}
	else
		push_log("[LOCALE]Файлы языковых переводов не найдены! Не удалось настроить интерфейс");

	log_stroke->append("[localeDetector]Обнаружен язык системы ");
	log_stroke->append(localeName.toLocal8Bit());
	push_log(log_stroke->c_str());

	if (localeName == "en_US")
		load_land(llocaleMap[localeName]);
	else if (localeName == "ru_RU")
		load_land(llocaleMap[localeName]);
	else if (localeName == "ru_UA")
		load_land(llocaleMap[localeName]);

	//Загружаем версию из названия патча

	//Настройка интерфейса
	buttons = new QHBoxLayout;
	layers = new QVBoxLayout;
	main = new QGroupBox;

	//Устанавливаем локаль - если в системе не предусмотрена динамическая смена языка то
	//сначала нужно применить загруженную локаль, иначе эффекта не будет
	if (!qApp->installTranslator(&llmd_lng))
		push_log("[QTranslator]Не удалось установить заданный язык приложения");

	//---------------------
	//
	//Окончание куска файла
	//
	//---------------------

	//Инициализация
	QVBoxLayout* fix = new QVBoxLayout;
	hints = new QVBoxLayout;
	cboxes = new QVBoxLayout;
	checkbox_composer = new QHBoxLayout;
	setWindowTitle(tr("updateWindow") + "[ТЕСТИРУЕТСЯ]");
	btn_confirm = new QPushButton(tr("btn_confirm"));
	btn_decline = new QPushButton(tr("btn_decline"));
	btn_done = new QPushButton(tr("btn_done"));
	update_progress = new QProgressBar(0);
	question = new QLabel(tr("fndVer") + " " + QString::number(fndBuild) + " ." + tr("curVer") + " " + QString::number(curBuild) + "\n" + tr("question?"));
	restart_after_hint = new QLabel(tr("restart_after_hint?"));
	delete_patch_hint = new QLabel(tr("delete_patch_hint?"));
	restart_after = new QCheckBox;
	delete_patch = new QCheckBox;
	question->setWordWrap(1);

	//Настройки визуала
	update_progress->setFixedHeight(24);
	btn_done->setFixedHeight(25);
	btn_confirm->setFixedHeight(25);
	btn_decline->setFixedHeight(25);
	//update_progress->hide();
	restart_after->setChecked(1);
	delete_patch->setChecked(1);
	delete_patch->setEnabled(0);
	delete_patch_hint->setEnabled(0);
	restart_after->setEnabled(0);
	restart_after_hint->setEnabled(0);

	//Настройка подсказок
	restart_after_hint->setWhatsThis(tr("restart_after_help"));
	delete_patch_hint->setWhatsThis(tr("delete_patch_help"));
	restart_after->setWhatsThis(tr("restart_after_help"));
	delete_patch->setWhatsThis(tr("delete_patch_help"));

	//Настройки таблица
	QStringList horizontalLbl;
	horizontalLbl << tr("command") << tr("status");
	table = new QTableWidget(0, 2);
	table->verticalHeader()->setFixedWidth(24);
	table->verticalHeader()->setDefaultSectionSize(20);
	table->setHorizontalHeaderLabels(horizontalLbl);
	table->setColumnWidth(0, 240);	//Столбец команд
	table->setColumnWidth(1, 50);	//Столбец статуса - [ОК], [СБОЙ]
	table->setFocusPolicy(Qt::NoFocus);
	table->setSelectionMode(QAbstractItemView::NoSelection);
	table->hide();

	//Настройка высоты кнопок и подсказок
	restart_after_hint->setFixedHeight(18);
	delete_patch_hint->setFixedHeight(18);
	restart_after->setFixedHeight(18);
	delete_patch->setFixedHeight(18);

	//Настройка логера
	logger = new QTextEdit;
	logger->setReadOnly(1);
	logger->setFixedHeight(80);
	logger->hide();

	//Настройка
	question->setFixedHeight(40);
	question->setAlignment(Qt::AlignCenter);
	update_progress->setAlignment(Qt::AlignCenter);
	btn_done->hide();

	//Настройка галок и подсказок
	hints->addWidget(restart_after_hint);
	hints->addWidget(delete_patch_hint);
	cboxes->setAlignment(Qt::AlignRight);
	cboxes->addWidget(restart_after);
	cboxes->addWidget(delete_patch);
	hints->setSpacing(1);
	cboxes->setSpacing(1);
	checkbox_composer->addLayout(hints);
	checkbox_composer->addLayout(cboxes);

	//Сборка
	buttons->addWidget(btn_confirm);
	buttons->addWidget(btn_decline);
	buttons->addWidget(btn_done);
	layers->addWidget(question);
	layers->addWidget(update_progress);
	layers->addLayout(checkbox_composer);
	layers->addLayout(buttons);
	layers->addWidget(table);
	layers->addWidget(logger);
	main->setLayout(layers);
	fix->addWidget(main);
	setLayout(fix);

	uint8_t connected_signals = 0;

	push_log("[QT->LiteMD]Установка связей сигнал-слот");
	//Блок сигнально-слотовых связей
	if (!connect(btn_confirm, SIGNAL(clicked()), this, SLOT(slot_confirm())))
		QErrorMessage::qtHandler(); ++connected_signals;//Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(btn_decline, SIGNAL(clicked()), this, SLOT(slot_decline())))
		QErrorMessage::qtHandler(); ++connected_signals;//Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(btn_done, SIGNAL(clicked()), this, SLOT(slot_done())))
		QErrorMessage::qtHandler(); ++connected_signals;//Соединяем сигнал от редактора к слоту изменения текста
	if (!connect(restart_after, SIGNAL(stateChanged(int)), this, SLOT(slot_switch_reset(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель совместимости рендера
	if (!connect(delete_patch, SIGNAL(stateChanged(int)), this, SLOT(slot_delete_patch(int))))
		QErrorMessage::qtHandler();	++connected_signals;//Переключатель совместимости рендера
	push_log(std::string("[QT->LiteMD]Образовано " + std::to_string(connected_signals) + " связей"));

	setFixedSize(384, 186);	//448 - с таблицей и окном логера
	
	//Вешаем красивую иконку
	setWindowIcon(setIcon());

	delete(log_stroke);
}

update_manager::~update_manager()
{
	//Если пользователь выбрал автоматичекий запуск после обновления то запускаем процесс не дожидаясь его выходим
	std::string myName = QCoreApplication::applicationFilePath().toStdString();
	if (start_lmd)
	{
		std::string new_exe_name = QCoreApplication::applicationDirPath().toStdString() + "/" + QFileInfo(QCoreApplication::applicationFilePath()).fileName().toStdString();
		if (new_exe_name.find("_old") != -1)	//Здесь проверка если юзер что-то нахуевертил в каталоге то запускаться не будем
		{
			new_exe_name.erase(new_exe_name.find("_old"), 4);
			insert_log("Запускаю LiteMD[" + new_exe_name + "]");
			save_log(std::string(appMainPath + "/"), "update");
			newInstance(new_exe_name.c_str());
			/*LPCSTR lPath(new_exe_name.c_str());
			LPSTR null = const_cast<char*>("");
			STARTUPINFO update = { sizeof(update) };
			PROCESS_INFORMATION proc_info;
			if (CreateProcess(lPath, null, NULL, NULL, TRUE, 0, NULL, NULL, &update, &proc_info))
			{
				CloseHandle(proc_info.hProcess);
				CloseHandle(proc_info.hThread);
			}
			else
				qDebug() << GetLastError();*/
			//exit(0);
			//system(new_exe_name.c_str());
			//QProcess* update_prc = new QProcess();	//Создаём процесс и запускаем его не дожидаясь
			//update_prc->startDetached(QString::fromStdString(new_exe_name));
		}
	}
	//Если пользователь решил удалить архив то после завершения он удаляется
	if (delete_patch_bit)
	{
		insert_log("Удаляю архив с пачтем");
		if (std::remove(QCoreApplication::applicationDirPath().toLocal8Bit() + "/" + patch_name.toLocal8Bit()) != 0)
			insert_log("Не удалось либо файла нет");
	}
	save_log(std::string(appMainPath + "/"), "update");
	freeProcess(std::wstring(myName.begin(), myName.end()).c_str());
}

void update_manager::slot_confirm()
{
	//Длинный линейный метод
	//Вырубаем кнопки чтобю юзер не сломал чего
	btn_confirm->setEnabled(0);
	btn_decline->setEnabled(0);

	float percentage = 0.0f;
	setFixedSize(384, 448);

	//Плюсуем процентик
	update_progress->setValue(1);

	//Показываем таблицу команд и консоль
	table->show();
	logger->show();
	//update_progress->show();

	//Начинаем ИБД в консоли
	insert_log("Начинаю обновление");
	insert_log("Распаковываю zip в папку temp");

	//Задаём папку в которую будут вытаскиваться файлы патча
	updateDir = new QDir(QApplication::applicationDirPath() + "/" + QString::fromStdString(working_folder));
	_qmz = new QMicroz(QApplication::applicationDirPath() + "/" + patch_name);
	if (_qmz)
	{
		update_progress->setValue(4);
		//При успешной ассоциации с архивом, задаём выходной путь, считаем 
		//прибавку процентов прогресс бара за файл(50% - распаковка завершена);
		_qmz->setOutputFolder(QApplication::applicationDirPath() + "/" + QString::fromStdString(working_folder) + "/");
		_zip = new ZipContentsList(_qmz->contents());
		insert_log("В патче найдено файлов: " + std::to_string(_zip->count()));
		percentage = 94.0f / static_cast<float>(_zip->count());
	}

	//Извлекаем в папку с патчем
	for (uint16_t _idx = 0; _idx < _zip->count(); ++_idx)
	{
		_qmz->extractIndex(_idx);
		update_progress->setValue(4 + static_cast<int>(percentage * (_idx + 1)));
		insert_log(std::string("Извлекаю ") + _zip->value(_idx).toStdString());
	}

	//Ищём файл commands.txt в папке с патчем
	std::ifstream cmd(std::string(working_folder + "/commands.txt"));

	if (cmd.is_open() == false)
	{
		insert_log("Не найден commands.txt");
	}
	else
	{
		insert_log("Выполняю скрипт commands.txt");
		std::string str;
		uint8_t commands = 0;
		update_progress->setValue(50);
		//Буферизуем команды из файла
		while (std::getline(cmd, str))
		{
			commands_set.push_back(str);

			//Создаём строчку с названием команды
			tabItm = new QTableWidgetItem(QString::fromStdString(commands_set.at(commands)));
			tabItm->setFlags(tabItm->flags() ^ Qt::ItemIsEditable);	//Отключаем возможность редактирования(потому что это отчёт и его менять нельзя)

			//Вставляем строчку в таблицу
			table->insertRow(table->rowCount());
			table->setItem(table->rowCount() - 1, 0, tabItm);
			++commands;

			//Обновляем интерфейс, чтобы табличка вовремя подгонялась
			qApp->processEvents();
		}
		percentage = 49.0f / commands;

		//Выполняем команды
		for (uint16_t _cmd = 0; _cmd < commands; ++_cmd)
		{
			if (_cmd == 0)
			{
				if (execute_command(commands_set.at(_cmd).c_str(), _cmd) != -1)
					insert_log("Не найдено начало программы!");
			}
			else if (_cmd == commands - 1)
			{
				if (execute_command(commands_set.at(_cmd).c_str(), _cmd) != 1)
					insert_log("Не найден конец программы!");
			}
			else
			{
				execute_command(commands_set.at(_cmd).c_str(), _cmd);
			}

			//Обновляем полоску прогресса
			update_progress->setValue(50 + static_cast<int>(percentage * (_cmd + 1)));
		}

		update_progress->setValue(100);
		insert_log(std::string("Выполнено " + std::to_string(exec_commands) + " команд"));
		insert_log("Обновление завершено");
	}
	
	btn_confirm->hide();
	btn_decline->hide();
	btn_done->show();

	delete(_zip);
	delete(_qmz);
}

void update_manager::slot_decline()
{
	insert_log("Работа завершена, сохраняю лог");
	qApp->closeAllWindows();
}

int update_manager::execute_command(QString commands, uint16_t no)
{
	//Пока не доделано

	static bool started = 0;													//Признак начала программы (возврат -1)
	static bool ended = 0;														//Признак конца программы  (возврат 1)

	uint8_t command_code = 127;													//По умолчанию - без кода

	command = commands.split(" ")[0].toStdString();								//Выцепляем из потока команду

	command_code = static_cast<uint8_t>(comand_set[command]);					//Получаем uint8_t код команды

	//Смотри код команды и обрабатываем по коду
	switch (command_code)
	{
		case 0:		//	ren "file1" "file2"		: Переименовать file1 в file2
		{
			//Выцепляем аргументы
			arg1 = appMainPath + commands.split("\"")[1].toStdString();
			arg2 = appMainPath + commands.split("\"")[3].toStdString();
			//Пытаемся переименовать файл из аргумента 1 в аргумент 2
			if(!std::rename(arg1.c_str(), arg2.c_str()))
				insert_status_code("OK", Qt::green, no);
			else
				insert_status_code("FAIL", Qt::red, no);
			break;
		}
		case 1:		//	del "file"				: Удалить file
		{
			//Выцепляем аргументы
			arg1 = appMainPath + commands.split("\"")[1].toStdString();
			if (!std::remove(arg1.c_str()))
				insert_status_code("OK", Qt::green, no);
			else
				insert_status_code("FAIL", Qt::red, no);
			break;
		}
		case 2:		//	move "file" "dir2"		: Переместить файл file в dir2
		{
			bool step_status = 0;
			//Выцепляем аргументы
			arg1 = working_folder + commands.split("\"")[1].toStdString();
			arg2 = appMainPath + commands.split("\"")[3].toStdString();
			//rename здесь работает коряво поэтому костылим
			try
			{
				std::filesystem::copy(arg1, arg2);
				step_status = 1;
			}
			catch (const std::filesystem::filesystem_error& e)
			{
				step_status = 0;
			}

			if (step_status && !std::remove(arg1.c_str()))
				insert_status_code("OK", Qt::green, no);
			else
				insert_status_code("FAIL", Qt::red, no);
			break;
		}
		case 3:		//	copy "file" "dir2"		: Скопировать файл file в dir2
		{
			//Выцепляем аргументы
			arg1 = working_folder + commands.split("\"")[1].toStdString();
			arg2 = appMainPath + commands.split("\"")[3].toStdString();
			try
			{
				std::filesystem::copy(arg1, arg2);
				insert_status_code("OK", Qt::green, no);
			}
			catch(const std::filesystem::filesystem_error& e)
			{
				insert_status_code("FAIL", Qt::red, no);
			}
			break;
		}
		case 4:		//	md "dir"				: Создать каталог dir
		{
			//Выцепляем аргументы
			arg1 = appMainPath + commands.split("\"")[1].toStdString();
			if (!std::filesystem::exists(arg1))
			{
				if(std::filesystem::create_directory(arg1))
					insert_status_code("OK", Qt::green, no);
				else
					insert_status_code("FAIL", Qt::red, no);
				break;
			}
			insert_status_code("OK", Qt::green, no);
			break;
		}
		case 5:		//	dd "dir"				: Удалить каталог dir
		{
			//Выцепляем аргументы
			arg1 = appMainPath + commands.split("\"")[1].toStdString();
			if (std::filesystem::exists(arg1))
			{
				if (std::filesystem::remove_all(arg1))
					insert_status_code("OK", Qt::green, no);
				else
					insert_status_code("FAIL", Qt::red, no);
				break;
			}
			insert_status_code("OK", Qt::green, no);
			break;
		}
		case 6:		//	log "string"			: Записать в лог строку "string"
		{
			//Выцепляем аргументы
			arg1 = commands.split("\"")[1].toStdString();
			insert_log(arg1);
			insert_status_code("OK", Qt::green, no);
			break;
		}
		case 254:	//	lmd_begin				: Начало программы
		{
			if (no == 0)
			{
				insert_status_code("OK", Qt::green, no);
				return -1;
			}
			insert_status_code("FAIL", static_cast<Qt::GlobalColor>((Qt::red | Qt::white)), no);
			return 0;
			//break;
		}
		case 255:	//	lmd_end					: Конец программы
		{
			insert_status_code("OK", Qt::green, no);
			return 1;
			break;
		}
	}
	return 0;
}

void update_manager::load_land(int lIndx)
{
	int langIndx = 0;
	langIndx = lIndx;
	auto it = lloc_map->cbegin();
	std::advance(it, lIndx);
	QString lang_file = it->second;
	push_log(std::string("[ЛОКАЛИЗАЦИЯ]Открываю файл локали " + QCoreApplication::applicationDirPath().toStdString() + "/loc/" + lang_file.toStdString()));
	if (!llmd_lng.load(QCoreApplication::applicationDirPath() + "/loc/" + lang_file, "."))
		push_log("[ЛОКАЛИЗАЦИЯ]Возника проблема с открытием файла");
}

void update_manager::insert_log(std::string input)
{
	logger->append(QString::fromStdString(input));
	push_log("[МЕНЕДЖЕР ОБНОВЛЕНИЙ]" + input);
}

void update_manager::slot_done()
{
	insert_log("Работа завершена, сохраняю лог");
	QApplication::exit();
	//update_manager::~update_manager();
	//exit(0);
}

void update_manager::insert_status_code(QString status, Qt::GlobalColor color, uint16_t no) noexcept
{
	QFont status_font("Monospace");												//Настройки шрифта для колонки статуса - моноширинный
	status_font.setBold(1);														//Задаём свойства жирного текста
	//Вставляем "ОК"
	//Создаём строчку с названием команды
	tabItm = new QTableWidgetItem("[" + status + "]");
	tabItm->setFlags(tabItm->flags() ^ Qt::ItemIsEditable);	//Отключаем возможность редактирования(потому что это отчёт и его менять нельзя)
	tabItm->setBackgroundColor(color);						//Красим в зелёный
	tabItm->setTextAlignment(Qt::AlignCenter);				//Размещаем по центру
	tabItm->setFont(status_font);							//Задаём свойства шрифта

	//Увеличиваем счётчик выполненных команд
	++exec_commands;

	//Вставляем строчку в таблицу
	table->setItem(no, 1, tabItm);
}

void update_manager::slot_switch_reset(int bit)
{
	start_lmd = static_cast<bool>(bit);
	//Если 0 - то разблокируется галка удаления архива
	//сделано для защиты от срабатывания на свой же архив патча
	start_lmd == 0 ? delete_patch_hint->setEnabled(1) : delete_patch_hint->setEnabled(0);
	start_lmd == 0 ? delete_patch->setEnabled(1) : delete_patch->setEnabled(0);
	if (start_lmd)
	{
		delete_patch->setChecked(1);
		delete_patch_bit = 1;
	}
}

void update_manager::slot_delete_patch(int bit)
{
	delete_patch_bit = static_cast<bool>(bit);
}
