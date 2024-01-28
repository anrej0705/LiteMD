#include "regex.h"

std::wregex regexHyperlink(L"[<]{1,1}(http|https|ftp://){0,1}\\S{1,}[>]{1,1}", std::wregex::collate);
std::wregex advRegexHyperlink(L"(\\[(.*?)\\])(\\(\\S{1,})\\)", std::wregex::collate);

std::wstring symbolCollection(L"<>");	//Служебные символы
std::wstring symbolClearance(L">");		//Символы обязательной очистки

//Библиотека заменителей
std::vector<std::wstring> replaceSymbols = { L"&#60;", L"&#62;" };
