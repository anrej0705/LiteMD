#include "syntax_preprocessor.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
//Чистим от скобок которые могут вызывать ложное срабатывание парсеров
void preprocessTrianlgeBrackets(std::wstring &wstr)
{
	try
	{
		//Если находим открывающую скобку - заменяем и вызываем себя в рекурсию
		for (int i = 0; i < wstr.size(); ++i)
		{
			if (wstr.at(i) == '<')
			{
				wstr.replace(i, 1, L"&#60;");
				preprocessTrianlgeBrackets(wstr);
			}
		}
		//Если находим закрывающую скобку то заменяем и в рекурсию
		for (int i = 0; i < wstr.size(); ++i)
		{
			if (wstr.at(i) == '>')
			{
				wstr.replace(i, 1, L"&#62;");
				preprocessTrianlgeBrackets(wstr);
			}
		}
	}
	catch (exceptionHandler)
	{
		push_log("[FATAL]Сбой в модуле: [УСТАРЕВШИЙ]preprocessTrianlgeBrackets");
		push_log("[exceptionHandler]Запрашиваю сохранение лога");
		throw(exceptionHandler(exceptionHandler::FATAL, QString("O kurwa!!!\nСбой в модуле [УСТАРЕВШИЙ]preprocessTrianlgeBrackets")));
	}
}
