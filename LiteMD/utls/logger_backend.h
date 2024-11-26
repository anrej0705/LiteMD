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
		friend class singlet_remover;
	public:
		logger_backend();
		~logger_backend();
		static logger_backend& getInstance();
		void insert_log(const char* log, uint32_t log_size);
		boost::container::vector<QString> get_logs();
};

void push_log(const char* log);

