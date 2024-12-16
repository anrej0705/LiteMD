#include <QtWidgets>
#include "mdScreen.h"
#include "boost/thread/thread.hpp"
#include "boost/chrono/chrono.hpp"
#include "syntax_preprocessor.h"
#include "syntax_postprocessor.h"
#include "symbolCleaner.h"
#include "urlBasicSimplifiedParser.h"
#include "urlBasicParser.h"
#include "urlAdvancedParser.h"
#include "headerLvlParser.h"
#include "shieldingParser.h"
#include "crlfProcessor.h"
#include "logger_backend.h"
#include "exceptionHandler.h"
#include "extended_strikethroughParser.h"
#include "hack_compat_parser.h"
#include "italicParser.h"
#include <string>
#include <regex>
extern "C"
{
	#include "global_definitions.h"
	#include "globalFlags.h"
}

extern struct parser_switchers parswitch;
extern struct depr_paerser_switchers dparswitch;

std::mutex balamut;

mdScreen::mdScreen(QWidget* scrWgt) : QLabel(scrWgt)
{
	push_log("[QT]Инициализация монитора");
	//Задаем формат отображения как html
	setTextFormat(Qt::RichText);
	//Задаем выравниваем на вверх и лево
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//Разрешаем интерактивный текст
	setTextInteractionFlags(Qt::TextBrowserInteraction);
	setOpenExternalLinks(1);
}

//Простой слот - принимает сигнал и изменяет виджет
void mdScreen::slotSetText(const QString& str)
{
	push_log("[РЕНДЕР]Начата обработка текста в HTML формат");
	//Преобразаем текст к 16 битному формату	//с 0.2.1 к 8 битному
	mdInput = str.toStdString();

	//Если чекбокс обработки устаревшего текста отключён
	if (enableDeprFeatures == 0)
	{
		push_log("[РЕНДЕР]Выбрана акутальная цепочка парсеров");
		balamut.lock();
		//Обрабатываем текст
		push_log("[РЕНДЕР]Предварительная конвертация экранированных символов");
		mdInput = shieldingParser(mdInput);						//0 -> 1|Предварительная конвертация экранированных символов
		push_log("[РЕНДЕР]Фильтрация служебных символов не являющихся частью тега");
		mdInput = symbolCleaner(mdInput);						//1 -> 2|Фильтрация служебных символов не являющихся частью тега
		if (parswitch.en_simple_url)
		{
			push_log("[РЕНДЕР]Обработка <www.url.ru>");
			mdInput = basicUrlParser(mdInput);					//2 -> 3|Обработка <www.url.ru>
		}
		if (parswitch.en_adv_url)
		{
			push_log("[РЕНДЕР]Обработка [name](url)");
			mdInput = advancedUrlParser(mdInput);				//3 -> 4|Обработка [name](url)
		}
		if (parswitch.en_header_lvl)
		{
			push_log("[РЕНДЕР]Обработка уровня заголовков");
			mdInput = headerLvlParser(mdInput);					//4 -> 5|Обработка уровня заголовков
		}
		if (parswitch.en_ex_strkthg)
		{
			push_log("[РЕНДЕР]Обработка зачёркнутых строк");
			mdInput = extended_strikethroughParser(mdInput);	//5 -> 6|Обработка зачёркнутых строк
		}
		if (parswitch.en_compat_undr)
		{
			push_log("[РЕНДЕР]Обработка аттрибута подчёркивания");//6 -> 7|Обработка в обратной совместимости
			mdInput = compatParser(mdInput);
		}
		if (parswitch.en_italic)
		{
			push_log("[РЕНДЕР]Обработка тегов курсивного тега");//7 -> 8|Обработка обозначений для курсива
			mdInput = italicParser(mdInput);
		}
		push_log("[РЕНДЕР]Обработка переноса строки");
		mdInput = crlfProcessor(mdInput);						//8 -> 9|Обработка переноса строки
		push_log("[РЕНДЕР]Конвертация в QString");
		//Преобразуем в QString
		mdFormatted = QString::fromStdString(mdInput);
		balamut.unlock();
	}
	else
	{
		std::wstring mdInputDepr(str.toStdWString());
		push_log("[РЕНДЕР]Выбрана устаревшая цепочка парсеров, возможны ошибки");
		push_log("[РЕНДЕР]Пожалуйста сохраните лог");
		initializeDeprecatedVars();
		balamut.lock();
		if (dparswitch.en_t_prep)
		{
			push_log("[РЕНДЕР][УСТАРЕВШИЙ]Фильтрация служебных символов, возможны ошибки");
			preprocessTrianlgeBrackets(mdInputDepr);
		}
		if (dparswitch.en_url_bas_simple)
		{
			push_log("[РЕНДЕР][УСТАРЕВШИЙ]Обработка <www.url.com>, возможны ошибки");
			mdInputDepr = basicSimplifiedUrlParser(mdInputDepr);
		}
		if (dparswitch.en_url_bas)
		{
			push_log("[РЕНДЕР][УСТАРЕВШИЙ]Обработка <http://www.url.com>, возможны ошибки");
			mdInputDepr = hyperlinkParser(mdInputDepr);
	}
		if (dparswitch.en_url_adv)
		{
			push_log("[РЕНДЕР][УСТАРЕВШИЙ]Обработка [name](url), возможны ошибки");
			textProcessorDeprecated();
		}
		push_log("[РЕНДЕР][УСТАРЕВШИЙ]Конвертация в QString");
		mdFormatted = QString::fromStdWString(mdInputDepr);
		if (dparswitch.en_t_post)
		{
			push_log("[РЕНДЕР][УСТАРЕВШИЙ]Постобработка, возможны ошибки");
			processCRLF(mdFormatted);
		}
		balamut.unlock();
	}

	push_log("[РЕНДЕР]Обработка завершена, передаю в монитор");

	//Отправляем на показ
	this->setText(mdFormatted);
}
