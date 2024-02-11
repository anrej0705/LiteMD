#include <QtWidgets>
#include "mdScreen.h"
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
	mdInput = str.toStdWString();
	//Копираем в поле вывода
	mdFormatted = QString::fromStdWString(mdInput);

	//Обрабатываем текст
	mdInput = shieldingParser(mdInput);					//0 -> 1|Предварительная конвертация экранированных символов
	mdInput = symbolCleaner(mdInput);					//1 -> 2|Фильтрация служебных символов не являющихся частью тега
	mdInput = basicSimplifiedUrlParser(mdInput);		//2 -> 3|Обработка <www.url.ru>
	mdInput = basicUrlParser(mdInput);					//3 -> 4|Обработка <http://www.url.ru>
	mdInput = advancedUrlParser(mdInput);				//4 -> 5|Обработка [name](url)
	mdInput = crlfProcessor(mdInput);					//5 -> 6|Обработка переноса строки

	//Преобразуем в QString
	mdFormatted = QString::fromStdWString(mdInput);

	//Отправляем на показ
	this->setText(mdFormatted);
}
