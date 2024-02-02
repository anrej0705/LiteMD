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

void mdScreen::initializeDeprecatedVars()
{
	lengShift = 0;
	//Инициализируем регексы
	regexHyperlink = new std::wregex(L"[<]{1,1}(http|https|ftp://){0,1}\\S{1,}[>]{1,1}", std::wregex::collate);
	advRegexHyperlink = new std::wregex(L"(\\[(.*?)\\])(\\(\\S{1,})\\)", std::wregex::collate);
}
