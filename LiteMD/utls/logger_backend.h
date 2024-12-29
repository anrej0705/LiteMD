#pragma once
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/container/string.hpp>
#include <boost/container/vector.hpp>
#include <string>
#include <QString>

//Опережающие объявления
//Пизжено из проекта semaphor_multithread https://github.com/anrej0705/semaphor_multithread
class logger_backend;

//Класс, гарантирующий удаление синглета после закрытия проги
//Пизжено из проекта semaphor_multithread https://github.com/anrej0705/semaphor_multithread
class singlet_remover
{
	private:
		logger_backend* l_inst;
	public:
		~singlet_remover();
		void initialize(logger_backend*);
};

class logger_backend
{
	private:
		boost::container::string* log_str;
		static logger_backend* log_bcknd;
		static singlet_remover remv;
		char** log_container;
		uint32_t log_str_counter;
		uint32_t msg_limit;
		friend class singlet_remover;
	public:
		logger_backend();
		~logger_backend();
		static logger_backend& getInstance();
		void insert_log(const char* log, uint32_t log_size);
		boost::container::vector<QString> get_logs();
		void clear_logs();
		char* get_stroke(uint32_t);
		uint32_t get_size();
		uint32_t get_limit();
		void set_limit(uint32_t limit);
};

void dump_crash_log();
void push_log(const char* log);									//Лог формата char*
void push_log(const std::string& log);							//Лог формата std::string
void push_log(const QString& log);								//Лог формата QString
void set_log_limit(uint32_t limit);								//Установка лимита на количество сообщений
void save_log();												//Сохранение логов
void save_log(const std::string path, const char* log_name);	//Перегруженная функция
