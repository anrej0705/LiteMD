#include "regex.h"
#include "urlAdvancedParser.h"

std::wstring advancedUrlParser(std::wstring& rawInput)
{
	//Буффер для работы с текстом
	std::wstring buffer = rawInput;

	//Хранилище мусора
	std::vector<std::wstring> garbage;
	std::vector<std::wstring> xpression;

	//0 - первым мусор
	//1 - первым полезный фрагмент
	bool firstOrder = 0;

	//Указатель на название ссылки
	uint32_t linkIndex = 0;

	//Указатель на начало заголовка ссылки
	uint32_t nameIndex = 0;

	//Временный буфер для обработки
	std::wstring _xpression;

	//индекс вхождения и длина выражения
	uint32_t prevIndex = 0;
	uint32_t index = 0;
	uint32_t range = 0;

	//Обрабатыванием разделяя на мусор и отсеянные регуляркой выражения
	//71 секунда выполнение!!
	for (std::wsregex_iterator it = std::wsregex_iterator(buffer.cbegin(), buffer.cend(), advRegexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		prevIndex = index + range;										//Запоминаем предыдущее вхождение
		index = it->position();											//Получаем индекс вхождения
		range = it->length();											//Получаем длину отсеянного фрагмента
		index == 0 ? firstOrder = 1 : firstOrder = 0;					//Определяем порядок следования содержимого
		garbage.push_back(buffer.substr(prevIndex, index - prevIndex));	//Запоминаем мусор(его мы не трогаем)
		xpression.push_back(buffer.substr(index, range));				//Запоминаем фрагменты которые будем обрабатывать
	}

	//Если после последнего встреченного фрагмента остался мусор то добавляем его тоже
	if (index + range < buffer.size())
		garbage.push_back(buffer.substr(index + range, buffer.size() - (index + range)));

	//Обрабатываем отсеянные фрагменты приводя их в HTML формат
	for (uint16_t iter = 0; iter < xpression.size(); ++iter)
	{
		linkIndex = xpression.at(iter).find(L"(");																//Находим позицию начала ссылки
		nameIndex = xpression.at(iter).find(L"[");
		_xpression = advUrlWrap.at(0);																			//Формируем открывающий тег
		_xpression.append(xpression.at(iter).substr(linkIndex + 1, xpression.at(iter).size() - linkIndex - 2));	//Добавляем в тег ссылку освобожденную от служ. символов
		_xpression.append(advUrlWrap.at(1));																	//Завершаем формирование открывающего тега
		_xpression.append(xpression.at(iter).substr(nameIndex + 1, linkIndex - nameIndex - 2));					//Добавляем кликабельный текст ссылки
		_xpression.append(advUrlWrap.at(2));																	//Добавляем закрывающий тег
		xpression.at(iter) = _xpression;																		//Заменяем исходник обработанным текстом
	}

	//Подготавливаем буфер к сборке
	buffer.clear();

	//Финальный этап - сборка строки
	if (firstOrder)	//Если сначала у нас полезный контент то пихаем его
	{
		for (uint32_t index = 0; index < xpression.size(); ++index)
		{
			//Добавляем сначала полезный контент(он идёт первым) а потом мусор
			buffer += xpression.at(index);
			buffer += garbage.at(index);
		}
		//Если остался дополнительный мусор, то тоже добавляем
		if (xpression.size() < garbage.size())
			buffer += garbage.at(garbage.size());
	}
	else //Иначе считаем что мусор
	{
		for (uint32_t index = 0; index < garbage.size(); ++index)
		{
			//Тут наоборот - сначала мусор а потом полезное
			buffer += garbage.at(index);
			if (index < xpression.size())
				buffer += xpression.at(index);
		}
	}

	return buffer;
}
