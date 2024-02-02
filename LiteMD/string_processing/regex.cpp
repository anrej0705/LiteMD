#include "regex.h"

std::wregex regexHyperlink(L"[<]{1,1}(http|https|ftp:){0,1}://([^/?#>]*)[>]{1,1}", std::wregex::collate);	//<http://www.url.ru>
std::wregex simplifiedRegexHyperlink(L"[<]{1,1}([^</?#>]*)[>]{1,1}", std::wregex::collate);	//<www.url.ru>
std::wregex advRegexHyperlink(L"(\\[(.*?)\\])(\\(\\S{1,})\\)", std::wregex::collate);	//[url](любойтекст)

std::wstring symbolCollection(L"<>[]()");		//Служебные символы

//Символы обязательной очистки
std::wstring symbolClearanceBack(L">])");		//Очистка с начала
std::wstring symbolClearanceFront(L"<[(");		//Очистка с конца

//Библиотека заменителей
std::vector<std::wstring> replaceSymbols = { L"&#60;", L"&#62;" };

//Обёртка для HTML
std::vector<std::wstring> basicUrlWrap = { L"<A HREF=\"", L"\">", L"</A>"};
std::vector<std::wstring> advUrlWrap = { L"<A HREF=\"", L"\">", L"</A>" };
