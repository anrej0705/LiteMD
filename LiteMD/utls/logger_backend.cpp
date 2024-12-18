#include "logger_backend.h"
#include "exceptionHandler.h"
#include "appSettings.h"
#include <fstream>
#include <iostream>

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
	msg_limit = 0;	//По умолчанию параметр не задан
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

uint32_t logger_backend::get_limit()
{
	return msg_limit;
}

void logger_backend::set_limit(uint32_t limit)
{
	msg_limit = limit;	//Из аргумента вхуяриваем ес чо
}

void push_log(const char* log)	//По идее это должно без проблем вызываться из сей
{
	t_mut.lock();	//Эта тема будет вызываться из разных потоков поэтому надо выстроить очередь
	if (logger_backend::getInstance().get_limit() != 0)	//Предел задан то тогда проверяем достигнут ли
	{													//если достигнут то чистим говнище вилкой
		if (logger_backend::getInstance().get_size() == logger_backend::getInstance().get_limit())
			logger_backend::getInstance().clear_logs();
	}
	logger_backend::getInstance().insert_log(log, strlen(log));
	t_mut.unlock();
}

void push_log(const std::string& log)	//Лог формата std::string
{
	t_mut.lock();	//Эта тема будет вызываться из разных потоков поэтому надо выстроить очередь
	if (logger_backend::getInstance().get_limit() != 0)	//Предел задан то тогда проверяем достигнут ли
	{													//если достигнут то чистим говнище вилкой
		if (logger_backend::getInstance().get_size() == logger_backend::getInstance().get_limit())
			logger_backend::getInstance().clear_logs();
	}
	logger_backend::getInstance().insert_log(log.c_str(), log.size());
	t_mut.unlock();
}

void push_log(const QString& log)		//Лог формата QString
{
	t_mut.lock();	//Эта тема будет вызываться из разных потоков поэтому надо выстроить очередь
	if (logger_backend::getInstance().get_limit() != 0)	//Предел задан то тогда проверяем достигнут ли
	{													//если достигнут то чистим говнище вилкой
		if (logger_backend::getInstance().get_size() == logger_backend::getInstance().get_limit())
		{
			//С 0.2.8 теперь дампится по пути заданному в конфиге каждый раз по достижению лимита
			::push_log("[LOG]Достигнут лимит сообщений логов, попытка сохранить лог в файл...");
			std::string filename("stable.");
			filename.append(&boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::universal_time()).c_str()[0], 19);
			std::replace(filename.begin(), filename.end(), ':', '-');
			std::replace(filename.begin(), filename.end(), 'T', '_');
			filename.append(".log");
			std::ofstream stable_log(QString(getConfigPath()).toStdString() + filename);
			for (uint32_t _index = 0; _index < logger_backend::getInstance().get_size(); ++_index)
			{
				filename = QString::fromUtf8(logger_backend::getInstance().get_stroke(_index)).toStdString() + '\n';
				stable_log.write(filename.c_str(), filename.size());
			}
			stable_log.close();
			logger_backend::getInstance().clear_logs();
		}
	}
	logger_backend::getInstance().insert_log(log.toLocal8Bit(), log.size());
	t_mut.unlock();
}

boost::container::vector<QString> logger_backend::get_logs()
{
	boost::container::vector<QString> container;	//Сюда будут копироваться логи из хранилища класса
	for (uint32_t _index = 0; _index < log_str_counter; ++_index)	//Копируем логи все до последнего
		container.push_back(QString::fromUtf8(log_container[_index]));
	return container;
}

void logger_backend::clear_logs()
{
	//Чистим строки
	for (uint32_t _index = 0; _index < log_str_counter; ++_index)
		free(log_container[_index]);
	free(log_container);	//Чистим массив и пересоздаём указатель
	log_container = (char**)calloc(log_str_counter + 1, sizeof(char*));
	log_str_counter = 0;
}

char* logger_backend::get_stroke(uint32_t _index)
{	//Здесь всё просто - получаем специфическую строчку по индексу если он не больше log_str_counter - 1
	if (_index < log_str_counter)
		return log_container[_index];
	else
		return log_container[log_str_counter - 1];
}

uint32_t logger_backend::get_size() {return log_str_counter;}

void dump_crash_log()
{
	//Если вдруг случился крит который роняет прогу то сохраняем лог в файл
	::push_log("[LOG]Попытка сохранить лог в файл...");
	std::string filename("crash.");
	filename.append(&boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::universal_time()).c_str()[0], 19);
	std::replace(filename.begin(), filename.end(), ':', '-');
	std::replace(filename.begin(), filename.end(), 'T', '_');
	filename.append(".log");
	std::ofstream crash_log(QString(getConfigPath()).toStdString() + filename);
	for (uint32_t _index = 0; _index < logger_backend::getInstance().get_size(); ++_index)
	{
		filename = QString::fromUtf8(logger_backend::getInstance().get_stroke(_index)).toStdString() + '\n';
		crash_log.write(filename.c_str(), filename.size());
	}
	crash_log.close();
}

void set_log_limit(uint32_t limit)
{
	logger_backend::getInstance().set_limit(limit);
}
