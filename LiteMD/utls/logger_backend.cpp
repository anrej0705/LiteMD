#include "logger_backend.h"
#include "exceptionHandler.h"

logger_backend* logger_backend::log_bcknd = 0;
singlet_remover logger_backend::remv;

//int testpoint1;
//std::string testpoint2;

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
	log_str = new boost::container::string;
}

logger_backend::~logger_backend()
{
	//Подчищаем за собой
	for (uint32_t _index = 0; _index < log_str_counter; ++_index)
		free(log_container[_index]);
	free(log_container);
	delete(log_str);
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

	//Выделяем память для указателя на строчку логов
	++log_str_counter;
	new_lc_ptr = (char**)realloc(log_container, sizeof(char*)*log_str_counter);
	if (new_lc_ptr != NULL)
	{
		log_container = new_lc_ptr;
		//Выделяем память для занесения строчки логов
		new_l_ptr = (char*)calloc(16 + log_size + 2, sizeof(char));
		if (new_l_ptr != NULL)
		{
			log_container[log_str_counter - 1] = new_l_ptr;
			log_container[log_str_counter - 1][0] = '[';
			//Достаём значение системного таймера
			strncpy(&log_container[log_str_counter - 1][1], &boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::universal_time()).c_str()[11], 14);
			log_container[log_str_counter - 1][15] = ']';
			strncpy(&log_container[log_str_counter - 1][16], log, log_size);
			log_container[log_str_counter - 1][log_size + 16] = '\0';
		}
		else
			throw(exceptionHandler(exceptionHandler::WARNING, "Не удалось выделить память для строчки лога(указатель зашкварился)"));
	}
	else
		throw(exceptionHandler(exceptionHandler::WARNING, "Не удалось выделить память в контейнере логов(указатель зашкварился)"));
}

void push_log(const char* log)	//По идее это должно без проблем вызываться из сей
{
	t_mut.lock();	//Эта тема будет вызываться из разных потоков поэтому надо выстроить очередь
	logger_backend::getInstance().insert_log(log, strlen(log));
	t_mut.unlock();
}

boost::container::vector<QString> logger_backend::get_logs()
{
	boost::container::vector<QString> container;
	for (uint32_t _index = 0; _index < log_str_counter; ++_index)
		container.push_back(QString::fromUtf8(log_container[_index]));
	return container;
}
