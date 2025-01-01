#pragma once

#include <QtWidgets>
#include "qmicroz.h"	//DLL!!!

//Постараюсь всё держать в пределах одного хидера и листинга	
//Менеджер обновлений работает независимо, сначала LiteMD.exe 
//переименовыватся в LiteMD_old.exe
// 
//Если пользователь откажется от обновления то скачанный ахерив удаляется и
//название проги возвращается обратно
// 
//Если пользователь соглашается то распаковывается архив, 
//выполняются команды commands.txt и LiteMD_old.exe удаляется вместе с архивом
//а в папку пользователя помещается лог обновления со всей инфой
// 
//Синтаксис команд
//имя_команды1 аргумент1 аргумент2
//имя_команды2 аргумент1
//имя_команды3 аргумент1
//имя_команды4 аргумент1 аргумент2
// 
//Замечание 1: команда с 1 аргументом работает от каталога приложения
//del "/LiteMD.exe" будет воспринято как "C:/LiteMD/LiteMD.exe"
// 
//Замечение 2: команда с 2 аргументами работает от каталога патча и каталога приложения
//move "/LiteMD.exe" "/LiteMD.exe" работает как
//переместить "C:/LiteMD/patch/LiteMD.exe" в "C:/LiteMD/LiteMD.exe"
//
//Замечание 3: команда ren работает как исключение из правил замечания 2
// 
//Замечание 4: команды lmd_being и lmd_end нужны для идентификации файла команд
//всегда помещайте lmd_begin в начало и lmd_end в конец
// 
//Команды в файле commands.txt могут быть следующего вида:
//	ren "file1" "file2"		: Переименовать file1 в file2
//	del "file"				: Удалить file
//	move "file" "dir2"		: Переместить файл file в dir2
//	copy "file" "dir2"		: Скопировать файл file в dir2
//	md "dir"				: Создать каталог dir
//	dd "dir"				: Удалить каталог dir
//	log "string"			: Записать в лог строку "string"
//	lmd_begin				: Начало программы
//	lmd_end					: Конец программы
//Каждая команда разделяется байтом \n
//У каждой команды есть номер в map команд
//Аргументы разделяются пробелом
//В результате выполнения команды будет сброшен лог работы update.log

class update_manager : public QDialog
{
	Q_OBJECT
	private:
		//Внутрянка
		std::vector<std::string> commands_set;				//Список команд прочитанных из файла commands.txt
		std::string working_folder;							//Имя папки в которую будут сбрасываться временные файлы
		std::string command;								//Команда
		std::string arg1;									//Аргумент 1
		std::string arg2;									//Аргумент 2

		//Инфа о билдах
		int curBuild;										//Текущий билд из globalDefinition
		int fndBuild;										//Билд из названия архива
		QString patch_name;									//Название архива с патчем

		//Таблица со списком команд и их статусом выполнения
		QTableWidget* table;											//Таблица
		QTableWidgetItem* tabItm = 0;									//Элемент таблицы

		//Карта локалей
		std::map<uint8_t, QString>* lloc_map;							//Список локалей
		QTranslator llmd_lng;											//Менеджер локализации

		//Папка для хранения файлов обновлений
		QDir* updateDir;												//Папка для обновления
		ZipContentsList* _zip;											//Список файлов
		QMicroz* _qmz;													//Архив

		//Логер
		QTextEdit* logger;												//Логер для всего что не отправляется в таблицу

		//Настройка интерфейсов
		QHBoxLayout* buttons;
		QVBoxLayout* layers;											//Кнопки + прогресс бар

		//Интерфейсы
		QGroupBox* main;												//Рамка с подписью

		QPushButton* btn_confirm;										//Подтверждение
		QPushButton* btn_decline;										//Юзер отказывается
		QPushButton* btn_done;											//Кнопка выхода
		QProgressBar* update_progress;									//Для юзера, чтобы было не так скучно ждать
		QLabel* question;												//Вопрос "обновить?"
	protected:
		void insert_log(std::string input);								//Доработка логера для отправки сообщения в окно и логер
	public:
		update_manager(QString p_name, QWidget* uWgt = 0);
		update_manager::~update_manager();
		bool execute_command(QString command, uint16_t no);				//Парсер команд
	public slots:
		void slot_confirm();											//Подтверждает обновление
		void slot_decline();											//Юзер отказался от обновления
		void slot_done();												//Завершает работу менеджера
		void load_land(int);											//Скопировано из appSettings
};
