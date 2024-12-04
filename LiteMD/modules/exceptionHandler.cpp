#include "exceptionHandler.h"
#include "logger_backend.h"
#include <boost/stacktrace/stacktrace.hpp>
#include <boost/container/string.hpp>
#include <sstream>

exceptionHandler::exceptionHandler(exceptType type)
{
	boost::container::string* log_out = new boost::container::string;
	std::string err_str;	//Собсно лог
	int returnCode = 0;
	std::stringstream error;
	switch (type)
	{
		case WARNING:
		{
			error << boost::stacktrace::stacktrace();
			returnCode = QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("Exception throwed!"), QMessageBox::Ok);
			push_log("[WARNING]Обнаружен сбой в работе, в возможно нестабильная работа программы");
			//Формируем stacktrace в лог
			while (std::getline(error, err_str, '\n'))
			{
				log_out->append("[STACKTRACE]");
				log_out->append(err_str.c_str());
				push_log(log_out->c_str());
				log_out->clear();
			}

			break;
		}
		case FATAL:
		{
			error << boost::stacktrace::stacktrace();
			returnCode = QMessageBox::critical(0, QObject::tr("FATAL"), QString(QObject::tr("Exception throwed!\nProgramm will close") + ("\n\nStacktrace:\n\n") + QString::fromStdString(error.str())), QMessageBox::Ok);
			push_log("[FATAL]Вызвано неизвестное исключение, см stacktrace");
			//Формируем stacktrace в лог
			while (std::getline(error, err_str, '\n'))
			{
				log_out->append("[STACKTRACE]");
				log_out->append(err_str.c_str());
				push_log(log_out->c_str());
				log_out->clear();
			}

			//Сбрасываем в файл
			dump_crash_log();
			if (returnCode == QMessageBox::Ok)
				exit(1);
			break;
		}
	}
}

exceptionHandler::exceptionHandler(exceptType type, QString reason)
{
	boost::container::string* log_out = new boost::container::string;
	std::string err_str;
	int returnCode = 0;
	std::stringstream error;
	switch (type)
	{
		case WARNING:
		{
			error << boost::stacktrace::stacktrace();
			returnCode = QMessageBox::warning(0, QObject::tr("Warning"), reason, QMessageBox::Ok);
			push_log("[WARNING]Обнаружен сбой в работе, в возможно нестабильная работа программы");
			//Формируем stacktrace в лог
			while (std::getline(error, err_str, '\n'))
			{
				log_out->append("[STACKTRACE]");
				log_out->append(err_str.c_str());
				push_log(log_out->c_str());
				log_out->clear();
			}
			push_log("[exceptionHandler]Лог падения сформирован");

			break;
		}
		case FATAL:
		{
			error << boost::stacktrace::stacktrace();
			returnCode = QMessageBox::critical(0, QObject::tr("FATAL"), QString(reason + ("\n\nStacktrace:\n\n") + QString::fromStdString(error.str())), QMessageBox::Ok);
			log_out->append("[FATAL]Вызвано исключение ");
			log_out->append(reason.toLocal8Bit());
			push_log(log_out->c_str());
			log_out->clear();
			push_log("[FATAL]Остановка работы");
			//Формируем stacktrace в лог
			while (std::getline(error, err_str, '\n'))
			{
				log_out->append("[STACKTRACE]");
				log_out->append(err_str.c_str());
				push_log(log_out->c_str());
				log_out->clear();
			}
			push_log("[exceptionHandler]Лог падения сформирован");

			//Сбрасываем в файл
			dump_crash_log();
			if (returnCode == QMessageBox::Ok)
				exit(1);
			break;
		}
	}
}
