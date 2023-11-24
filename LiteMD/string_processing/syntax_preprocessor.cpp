#include "syntax_preprocessor.h"
void preprocessTrianlgeBrackets(std::wstring &wstr)
{
	for (int i = 0;i < wstr.size();++i)
	{
		if (wstr.at(i) == '<')
		{
			wstr.replace(i, 1, L"&#60;");
			preprocessTrianlgeBrackets(wstr);
		}
	}
	for (int i = 0;i < wstr.size();++i)
	{
		if (wstr.at(i) == '>')
		{
			wstr.replace(i, 1, L"&#62;");
			preprocessTrianlgeBrackets(wstr);
		}
	}
}
