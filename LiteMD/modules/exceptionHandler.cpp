#include "exceptionHandler.h"
#include "logger_backend.h"

exceptionHandler::exceptionHandler(exceptType type)
{
	int returnCode = 0;
	switch (type)
	{
		case WARNING:
		{
			returnCode = QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("Exception throwed!"), QMessageBox::Ok);
			push_log("[WARNING]Обнаружен сбой в работе, в возможно нестабильная работа программы");
			break;
		}
		case FATAL:
		{
			returnCode = QMessageBox::critical(0, QObject::tr("FATAL"), QObject::tr("Exception throwed!\nProgramm will close"), QMessageBox::Ok);
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
			returnCode = QMessageBox::critical(0, QObject::tr("FATAL"), reason, QMessageBox::Ok);
			push_log("[FATAL]Остановка работы");
			dump_crash_log();
			if (returnCode == QMessageBox::Ok)
				exit(1);
			break;
		}
	}
}
