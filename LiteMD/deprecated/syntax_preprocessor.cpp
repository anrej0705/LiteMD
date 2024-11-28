#include "syntax_preprocessor.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
//������ �� ������ ������� ����� �������� ������ ������������ ��������
void preprocessTrianlgeBrackets(std::wstring &wstr)
{
	try
	{
		//���� ������� ����������� ������ - �������� � �������� ���� � ��������
		for (int i = 0; i < wstr.size(); ++i)
		{
			if (wstr.at(i) == '<')
			{
				wstr.replace(i, 1, L"&#60;");
				preprocessTrianlgeBrackets(wstr);
			}
		}
		//���� ������� ����������� ������ �� �������� � � ��������
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
		push_log("[FATAL]���� � ������: [����������]preprocessTrianlgeBrackets");
		push_log("[exceptionHandler]���������� ���������� ����");
		throw(exceptionHandler(exceptionHandler::FATAL, QString("O kurwa!!!\n���� � ������ [����������]preprocessTrianlgeBrackets")));
	}
}
