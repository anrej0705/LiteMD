#include "logger_backend.h"
#include "exceptionHandler.h"
#include <sstream>

logger_backend* logger_backend::log_bcknd = 0;
singlet_remover logger_backend::remv;

int testpoint1;
std::string testpoint2;

std::mutex t_mut;

singlet_remover::~singlet_remover()
{
	if (l_inst != NULL)
		delete l_inst;
}
void singlet_remover::initialize(logger_backend* ptr)
{
	l_inst = ptr;
}

logger_backend::logger_backend()
{
	log_str_counter = 0;	//Выделяем память на один указатель больше, на всякий
	log_container = (char**)calloc(log_str_counter + 1, sizeof(char*));
}

logger_backend::~logger_backend()
{
	//Подчищаем за собой
	for (uint32_t _index = 0; _index < log_str_counter + 1; ++_index)
		free(log_container[_index]);
	free(log_container);
}

logger_backend& logger_backend::getInstance()
{
	if (!log_bcknd)
	{
		log_bcknd = new logger_backend();
		remv.initialize(log_bcknd);
	}
	return *log_bcknd;
}

void logger_backend::insert_log(const char* log, uint32_t log_size)
{
	static char** new_lc_ptr;	//Указатель на новый блок, для проверки на зашкварность
	static char* new_l_ptr;		//То же самое но для строчки

	static uint8_t sssize = 0;

	//Достаём значение системного таймера
	boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%H:%M:%S.%f");
	std::stringstream date_stream;
	date_stream.imbue(std::locale(date_stream.getloc(), facet));
	date_stream << boost::posix_time::microsec_clock::universal_time();
	sssize = date_stream.str().size();

	//Выделяем память для указателя на строчку логов
	++log_str_counter;
	new_lc_ptr = (char**)realloc(log_container, log_str_counter + 1);
	if (new_lc_ptr != NULL)
		log_container = new_lc_ptr;
	else
		throw(exceptionHandler(exceptionHandler::WARNING, "Не удалось выделить память в контейнере логов(указатель зашкварился)"));

	//Выделяем память для занесения строчки логов
	new_l_ptr = (char*)calloc(log_size + sssize + 3, sizeof(char));
	if (new_l_ptr != NULL)
		log_container[log_str_counter - 1] = new_l_ptr;
	else
		throw(exceptionHandler(exceptionHandler::WARNING, "Не удалось выделить память для строчки лога(указатель зашкварился)"));

	//Слепляем смску в лог
	testpoint1 = log_size;
	testpoint2 = log;
	log_container[log_str_counter - 1][0] = '[';
	strncpy(&log_container[log_str_counter - 1][1], date_stream.str().c_str(), sssize);
	log_container[log_str_counter - 1][sssize + 1] = ']';
	strncpy(&log_container[log_str_counter - 1][sssize + 2], log, log_size);
}

void push_log(const char* log)	//По идее это должно без проблем вызываться из сей
{
	t_mut.lock();	//Эта тема будет вызываться из разных потоков поэтому надо выстроить очередь
	logger_backend::getInstance().insert_log(log, strlen(log));
	t_mut.unlock();
}
