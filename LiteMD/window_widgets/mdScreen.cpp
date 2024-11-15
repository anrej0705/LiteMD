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
#include <string>
#include <regex>

std::mutex balamut;

mdScreen::mdScreen(QWidget* scrWgt) : QLabel(scrWgt)
{
	//Создаём поток который будем в будущем запускать для обработки текста
	parsingThread = new boost::thread(&mdScreen::processThread, this);
	//Задаем формат отображения как html
	setTextFormat(Qt::RichText);
	//Задаем выравниваем на вверх и лево
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//Разрешаем интерактивный текст
	setTextInteractionFlags(Qt::TextBrowserInteraction);
	setOpenExternalLinks(1);
}

void mdScreen::processThread() //Обработчик в отдельном потоке, предотвраящет повисание проги на больших текстах
{
	std::wstring textToProcess = mdInput;	//Для предотвращения дедлоков
	//Обрабатываем текст
	textToProcess = shieldingParser(textToProcess);					//0 -> 1|Предварительная конвертация экранированных символов
	//textToProcess = symbolCleaner(textToProcess);					//1 -> 2|Фильтрация служебных символов не являющихся частью тега
	//textToProcess = basicSimplifiedUrlParser(textToProcess);		//2 -> 3|Обработка <www.url.ru>
	//textToProcess = basicUrlParser(textToProcess);				//3 -> 4|Обработка <http://www.url.ru>
	//textToProcess = advancedUrlParser(textToProcess);				//4 -> 5|Обработка [name](url)
	textToProcess = crlfProcessor(textToProcess);					//5 -> 6|Обработка переноса строки

	//Преобразуем в QString под лочкой
	balamut.lock();
	mdFormatted = QString::fromStdWString(textToProcess);
	this->setText(mdFormatted);
	balamut.unlock();
}

//Простой слот - принимает сигнал и изменяет виджет
void mdScreen::slotSetText(const QString& str)
{
	bool temp = NULL;
	//Преобразаем текст к 16 битному формату
	mdInput = str.toStdWString();
	//Копираем в поле вывода
	mdFormatted = QString::fromStdWString(mdInput);

	//processThread();
	//Если поток ужа запущен, то прибить и по новой
	/*if (!parsingThread->joinable())
	{
		parsingThread->interrupt();
		delete(parsingThread);
		parsingThread = new boost::thread(&mdScreen::processThread, this);
	}

	parsingThread->detach();*/

	//Обрабатываем текст
	//mdInput = shieldingParser(mdInput);					//0 -> 1|Предварительная конвертация экранированных символов
	//mdInput = symbolCleaner(mdInput);					//1 -> 2|Фильтрация служебных символов не являющихся частью тега
	//mdInput = basicSimplifiedUrlParser(mdInput);		//2 -> 3|Обработка <www.url.ru>
	//mdInput = basicUrlParser(mdInput);					//3 -> 4|Обработка <http://www.url.ru>
	//mdInput = advancedUrlParser(mdInput);				//4 -> 5|Обработка [name](url)
	mdInput = crlfProcessor(mdInput);					//5 -> 6|Обработка переноса строки

	//Преобразуем в QString
	mdFormatted = QString::fromStdWString(mdInput);

	//Отправляем на показ
	this->setText(mdFormatted);
}
