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
	mdInput = symbolCleaner(mdInput);					//0 -> 1|Фильтрация служебных символов не являющихся частью тега
	mdInput = basicSimplifiedUrlParser(mdInput);		//1 -> 2|Обработка <www.url.ru>
	mdInput = basicUrlParser(mdInput);					//2 -> 3|Обработка <http://www.url.ru>
	mdInput = advancedUrlParser(mdInput);				//3 -> 4|Обработка [name](url)

	//Преобразуем в QString
	mdFormatted = QString::fromStdWString(mdInput);

	//Отправляем на показ
	this->setText(mdFormatted);
}
