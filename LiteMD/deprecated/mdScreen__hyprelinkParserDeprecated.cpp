#include <QtWidgets>
#include "mdScreen.h"
#include "syntax_preprocessor.h"
#include "syntax_postprocessor.h"
#include "symbolCleaner.h"
#include "urlBasicSimplifiedParser.h"
#include "urlBasicParser.h"
#include "urlAdvancedParser.h"
#include <string>
#include <regex>
#include "exceptionHandler.h"
#include "logger_backend.h"

//������ � ������������ <url> �� �������
std::wstring mdScreen::hyperlinkParser(std::wstring& str)
{
	try
	{
		//������� ���������� ��� �������� ����� ����� � ����� ���������
		int index = 0;
		int range = 0;
		std::wstring buffer = str;

		//������� ��������� ��� �������� ��������� � �������� ������� �� ������ �������� ��������
		std::list<std::wstring> garbage;
		std::list<std::wstring> xpressions;

		//���� ������ ������������ ���-�� ������� �� ������ ������ - ��������� �� ���� � ���
		for (std::wsregex_iterator it = std::wsregex_iterator(str.cbegin(), str.cend(), *regexHyperlink); it != std::wsregex_iterator(); ++it)
		{
			//������ ����� ������ � ������� �� ���������
			range = it->position();
			//�������� � �����
			buffer = str.substr(index, range - index);
			//���������� � ���������� ������� ������ ����� �� �������� ��� ���������� ������� ������������
			preprocessTrianlgeBrackets(buffer);
			//preprocessTrianlgeBrackets(buffer);
			//��������� ����� � ���������
			garbage.push_back(buffer);
			//���� ����������� ������
			for (int i = range; i < str.size(); ++i)
			{
				if (mdInput_depr.at(i) == '>')
				{
					//��� ������ ������� - ���������� ������� � ������� �� �����
					index = i + 1;
					break;
				}
			}
			//�������� � ����� ��������� ����������� � � ���������� � ���������
			buffer = str.substr(range, index - range);
			xpressions.push_back(buffer);
		}
		//���� � ����� ���-�� ���� �� ���������
		if (index < mdInput_depr.size())
		{
			//�������� � ����� �� ��������� �� �����, ������������ �� ������ � �������� ��������� � ���������
			buffer = mdInput_depr.substr(index, mdInput_depr.size() - index);
			preprocessTrianlgeBrackets(buffer);
			//preprocessTrianlgeBrackets(buffer);
			garbage.push_back(buffer);
		}
		//������ �����
		buffer = std::wstring(L"");
		//��������� ��� �����
		for (;;)
		{
			//���� ��������� � ������������ ��������� �� ���� �� ��������� � ������ �� 1 �������
			if (garbage.size() != 0)
			{
				buffer += garbage.front();
				garbage.pop_front();
			}
			//���� ��������� � url �������� �� ���� �� ��������� � ������ �� 1 �������
			if (xpressions.size() != 0)
			{
				buffer += xpressions.front();
				xpressions.pop_front();
			}
			//���� ��� ���������� ����� �� �������
			if (garbage.size() == 0 && xpressions.size() == 0)
				break;
		}
		//���������� ������������� ������ ��� ������ ������� �������� ������������� � HTML ����
		return buffer;
	}
	catch (exceptionHandler)
	{
		push_log("[FATAL]���� � ������: [����������]hyperlinkParser");
		push_log("[exceptionHandler]���������� ���������� ����");
		throw(exceptionHandler(exceptionHandler::FATAL, QString("O kurwa!!!\n���� � ������ [����������]hyperlinkParser")));
	}
}