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
#include "globalFlags.h"
#include "logger_backend.h"
#include <string>
#include <regex>

extern struct parser_switchers parswitch;

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
	//Преобразаем текст к 16 битному формату
	mdInput = str.toStdString();

	balamut.lock();
	//Обрабатываем текст
	push_log("[РЕНДЕР]Предварительная конвертация экранированных символов");
	mdInput = shieldingParser(mdInput);					//0 -> 1|Предварительная конвертация экранированных символов
	push_log("[РЕНДЕР]Фильтрация служебных символов не являющихся частью тега");
	mdInput = symbolCleaner(mdInput);					//1 -> 2|Фильтрация служебных символов не являющихся частью тега
	if (parswitch.en_simple_url)
	{
		push_log("[РЕНДЕР]Обработка <www.url.ru>");
		mdInput = basicUrlParser(mdInput);				//2 -> 3|Обработка <www.url.ru>

	}
	if (parswitch.en_adv_url)
	{
		push_log("[РЕНДЕР]Обработка [name](url)");
		mdInput = advancedUrlParser(mdInput);			//3 -> 4|Обработка [name](url)

	}
	if (parswitch.en_header_lvl)
	{
		push_log("[РЕНДЕР]Обработка уровня заголовков");
		mdInput = headerLvlParser(mdInput);				//4 -> 5|Обработка уровня заголовков

	}
	push_log("[РЕНДЕР]Обработка переноса строки");
	mdInput = crlfProcessor(mdInput);					//5 -> 6|Обработка переноса строки
	balamut.unlock();
	
	//Преобразуем в QString
	mdFormatted = QString::fromStdString(mdInput);

	push_log("[РЕНДЕР]Обработка завершена, передаю на показ");

	//Отправляем на показ
	this->setText(mdFormatted);
}
