#include "syntax_postprocessor.h"
#include "qstring.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
//������ �� ������� \n
void processCRLF(QString &str)
{
	try
	{
		for (int i = 0; i < str.size(); ++i)
		{
			//���� ������� \n �� �������� html �����
			if (str[i] == '\n')
			{
				str.replace(i, 1, "<BR>");
				processCRLF(str);
			}
		}
	}
	catch (exceptionHandler)
	{
		push_log("[FATAL]���� � ������: [����������]processCRLF");
		push_log("[exceptionHandler]���������� ���������� ����");
		throw(exceptionHandler(exceptionHandler::FATAL, QString("O kurwa!!!\n���� � ������ [����������]processCRLF")));
	}
}
