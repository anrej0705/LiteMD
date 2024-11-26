#pragma once
#include <boost/container/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>

//ќпережающие объ€влени€
//ѕизжено из проекта semaphor_multithread https://github.com/anrej0705/semaphor_multithread
class logger_backend;

// ласс, гарантирующий удаление синглета после закрыти€ проги
//ѕизжено из проекта semaphor_multithread https://github.com/anrej0705/semaphor_multithread
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
};
