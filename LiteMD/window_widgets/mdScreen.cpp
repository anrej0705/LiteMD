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
#include "shieldingParser.h"
#include "crlfProcessor.h"
#include "globalFlags.h"
#include <string>
#include <regex>

extern struct parser_switchers parswitch;

std::mutex balamut;

mdScreen::mdScreen(QWidget* scrWgt) : QLabel(scrWgt)
{
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
	//Преобразаем текст к 16 битному формату
	mdInput = str.toStdString();

	balamut.lock();
	//Обрабатываем текст
	mdInput = shieldingParser(mdInput);					//0 -> 1|Предварительная конвертация экранированных символов
	mdInput = symbolCleaner(mdInput);					//1 -> 2|Фильтрация служебных символов не являющихся частью тега
	if (parswitch.en_simple_url)
		mdInput = basicUrlParser(mdInput);				//2 -> 3|Обработка <www.url.ru>
	if (parswitch.en_adv_url)
		mdInput = advancedUrlParser(mdInput);			//3 -> 4|Обработка [name](url)
	mdInput = crlfProcessor(mdInput);					//4 -> 5|Обработка переноса строки
	balamut.unlock();
	
	//Преобразуем в QString
	mdFormatted = QString::fromStdString(mdInput);

	//Отправляем на показ
	this->setText(mdFormatted);
}
