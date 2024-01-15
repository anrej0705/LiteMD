#pragma once
#include <QtWidgets>
#include <regex>

class mdScreen : public QLabel
{
	Q_OBJECT
	private:
		int lengShift;	//Контейнер для хранения смещения текста
		std::wregex* regexHyperlink;	//Регулярки для обработки гиперссылок
		std::wregex* advRegexHyperlink;	//Регулярка для альтернативных гиперссылок
		std::wstring mdInput;	//Контейнер для предварительной обработки текста
		QString mdFormatted;	//Контейнер для обработанного текста
		std::wstring hyperlinkParser(std::wstring&);	//Парсер гипертекста(скоро будет заменен)
	public:
		//Базовый конструктор
		mdScreen(QWidget* scrWgt = 0);
	public slots:
		//Слот для принятия текста от редактора
		void slotSetText(const QString&);	//Слот для принятия сигнала textChanged() и высылки сигнала с текстом
};
