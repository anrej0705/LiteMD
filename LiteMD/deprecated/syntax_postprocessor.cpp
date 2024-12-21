#include "syntax_postprocessor.h"
#include "qstring.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
//Чистим от симолов \n
void processCRLF(QString &str)
{
	try
	{
		for (int i = 0; i < str.size(); ++i)
		{
			//Если находим \n то заменяем html кодом
			if (str[i] == '\n')
			{
				str.replace(i, 1, "<BR>");
				processCRLF(str);
			}
		}
	}
	catch (exceptionHandler)
	{
		push_log("[FATAL]Сбой в модуле: [УСТАРЕВШИЙ]processCRLF");
		push_log("[exceptionHandler]Запрашиваю сохранение лога");
		throw(exceptionHandler(exceptionHandler::FATAL, QString("O kurwa!!!\nСбой в модуле [УСТАРЕВШИЙ]processCRLF")));
	}
}
