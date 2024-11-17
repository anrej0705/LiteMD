#pragma once

#include <string>
#include <regex>
#include <deque>
#include <boost/container/string.hpp>
#include <boost/container/vector.hpp>

//Регексы для парсинга
extern std::wregex regexHyperlink;
extern std::wregex simplifiedRegexHyperlink;
extern std::wregex advRegexHyperlink;

//Наборы символов для обработки и фильтрации
extern std::wstring symbolCollection;
extern std::wstring symbolClearanceBack;
extern std::wstring symbolClearanceFront;
extern std::vector<std::wstring> replaceSymbols;

//Наборы HTML-конструкций разметки
extern std::vector<std::wstring> basicUrlWrap;
extern std::vector<std::wstring> advUrlWrap;

//Наборы символов для экранирования
extern boost::container::string shieldingSymbolsSrc;
extern boost::container::vector<boost::container::string> shieldingSymbols;
