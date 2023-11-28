#include "syntax_postprocessor.h"
#include "qstring.h"
//Чистим от симолов \n
void processCRLF(QString &str)
{
	for (int i = 0;i < str.size();++i)
	{
		//Если находим \n то заменяем html кодом
		if (str[i] == '\n')
		{
			str.replace(i, 1, "<BR>");
			processCRLF(str);
		}
	}
}
