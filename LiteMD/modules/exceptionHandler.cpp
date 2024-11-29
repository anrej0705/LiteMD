#include "exceptionHandler.h"
#include "logger_backend.h"
#include <boost/stacktrace.hpp>
#include <sstream>

exceptionHandler::exceptionHandler(exceptType type)
{
	int returnCode = 0;
	std::stringstream error;
	switch (type)
	{
		case WARNING:
		{
			error << boost::stacktrace::stacktrace();
			returnCode = QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("Exception throwed!"), QMessageBox::Ok);
			push_log("[WARNING]Обнаружен сбой в работе, в возможно нестабильная работа программы");
			break;
		}
		case FATAL:
		{
			error << boost::stacktrace::stacktrace();
			returnCode = QMessageBox::critical(0, QObject::tr("FATAL"), QString(QObject::tr("Exception throwed!\nProgramm will close") + QString::fromStdString(error.str())), QMessageBox::Ok);
			push_log("[FATAL]Остановка работы");
			dump_crash_log();
			if (returnCode == QMessageBox::Ok)
				exit(1);
			break;
		}
	}
}

exceptionHandler::exceptionHandler(exceptType type, QString reason)
{
	int returnCode = 0;
	std::stringstream error;
	switch (type)
	{
		case WARNING:
		{
			returnCode = QMessageBox::warning(0, QObject::tr("Warning"), reason, QMessageBox::Ok);
			push_log("[WARNING]Обнаружен сбой в работе, в возможно нестабильная работа программы");
			break;
		}
		case FATAL:
		{
			returnCode = QMessageBox::critical(0, QObject::tr("FATAL"), QString(reason + ("\n\nStacktrace:") + QString::fromStdString(error.str())), QMessageBox::Ok);
			push_log("[FATAL]Остановка работы");
			dump_crash_log();
			if (returnCode == QMessageBox::Ok)
				exit(1);
			break;
		}
	}
}
