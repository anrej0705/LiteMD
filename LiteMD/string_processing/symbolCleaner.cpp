#include "symbolCleaner.h"
#include "regex.h"
#include "exceptionHandler.h"
#include <boost/thread/thread.hpp>
#include <mutex>
#include <map>
std::condition_variable condition;
std::mutex m_mut;

//Хранит индексы заменяемых спецсимволов
//first - позиция
//second - длина
std::map<int, int> symbolIndexes;

//0 - первым мусор
//1 - первым полезный фрагмент
bool firstOrden = 0;

bool hLinkDetect = 0;
bool hSimpLinkDetect = 0;
bool hAdvLinkDetect = 0;

bool hLinkParsed = 0;
bool hSimpLinkParsed = 0;
bool hAdvLinkParsed = 0;

//Эту дичь пихать только в методы
#define THREAD_LOCK std::lock_guard<std::mutex> lg(m_mut);
#define NOTIFY_ALL_THREAD condition.notify_all();

/*//Запуск менеджера и перенос в отдельный поток
void semaphor_manager::run_manager()
{
	manager_thread = new boost::thread(&semaphor_manager::queueManager, this);
	manager_thread->detach();
}*/

void saveToMap(int position, int length)
{
	THREAD_LOCK
	if (!symbolIndexes.insert(std::map<int, int>::value_type(position, length)).second)
	{
		throw(exceptionHandler(exceptionHandler::WARNING));	//Если не удалось вставить то кидаем пред
	}
	NOTIFY_ALL_THREAD
}

void regexHyperlinkParse(std::wstring input)
{
	//std::wstring temp;
	for (auto it = std::wsregex_iterator(input.begin(), input.end(), regexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		it->position() == 0 ? firstOrden = 1 : firstOrden = 0;
		saveToMap(it->position(), it->length());
		//temp = input.substr(it->position(), it->length());
		//temp = temp;
		//qDebug() << "Detect regexHyperlink:" << QString::fromStdWString(temp);
		hLinkDetect = 1;
	}
	hLinkParsed = 1;
}

void regexSimplyHLinkParse(std::wstring input)
{
	//std::wstring temp;
	for (auto it = std::wsregex_iterator(input.begin(), input.end(), simplifiedRegexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		it->position() == 0 ? firstOrden = 1 : firstOrden = 0;
		saveToMap(it->position(), it->length());
		//temp = input.substr(it->position(), it->length());
		//temp = temp;
		//qDebug() << "Detect simplifiedRegexHyperlink:" << QString::fromStdWString(temp);
		hSimpLinkParsed = 1;
	}
	hSimpLinkParsed = 1;
}

void regexAdvHLinkParse(std::wstring input)
{
	//std::wstring temp;
	for (auto it = std::wsregex_iterator(input.begin(), input.end(), advRegexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		it->position() == 0 ? firstOrden = 1 : firstOrden = 0;
		saveToMap(it->position(), it->length());
		//temp = input.substr(it->position(), it->length());
		//temp = temp;
		//qDebug() << "Detect advRegexHyperlink:" << QString::fromStdWString(temp);
		hAdvLinkDetect = 1;
	}
	hAdvLinkParsed = 1;
}

std::wstring symbolCleaner(std::wstring& rawInput)
{
	//Создаем буффер, в котором будем чистить спецсимволы
	std::wstring buffer = rawInput;

	//Создаем строку в которой будут искаться регулярки
	std::wstring regBuffer(buffer);

	//Контейнер спецсимвола, сюда помещается первый попавшийся спецсимвол
	std::wstring symbol;

	//Хранилище параметров обработки строки
	std::wsmatch wstrMatch;

	//Указатель на индекс символа из библиотеки замены
	uint8_t replaceIndex = 0;

	//Указатель на предыдущее вхождение
	uint32_t prevIndex = 0;
	uint32_t prevSize = 0;

	//Маркер, по которому потом будет заменяться пустые символы
	std::wstring marker(L"\u009C");
	//std::wstring marker(L"Z");

	//Хранит значение предыдущего спецсимвола
	uint32_t symbolDuplicate = 0;

	//Хранилище мусора
	std::vector<std::wstring> garbage;
	std::vector<std::wstring> xpression;

	//Размечаем файл для нарезки на фрагменты, разделяем на потоки
	boost::thread* regexHyperlinkParser = new boost::thread(&regexHyperlinkParse, buffer);
	boost::thread* regexSimplyHLinkParser = new boost::thread(&regexSimplyHLinkParse, buffer);
	boost::thread* regexAdvHLinkParser = new boost::thread(&regexAdvHLinkParse, buffer);
	regexHyperlinkParser->detach();
	regexSimplyHLinkParser->detach();
	regexAdvHLinkParser->detach();
	while ((hLinkParsed && hSimpLinkParsed && hAdvLinkParsed) == 0)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
	}
	/*for (auto it = std::wsregex_iterator(buffer.begin(), buffer.end(), regexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		it->position() == 0 ? firstOrden = 1 : firstOrden = 0;
		saveToMap(it->position(), it->length());
	}*
	for (auto it = std::wsregex_iterator(buffer.begin(), buffer.end(), simplifiedRegexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		it->position() == 0 ? firstOrden = 1 : firstOrden = 0;
		saveToMap(it->position(), it->length());
	}
	for (auto it = std::wsregex_iterator(buffer.begin(), buffer.end(), advRegexHyperlink); it != std::wsregex_iterator(); ++it)
	{
		it->position() == 0 ? firstOrden = 1 : firstOrden = 0;
		saveToMap(it->position(), it->length());
	}*/

	//Нарезаем строку на "полезные" фрагменты и мусор
	//Полезные фрагменты - прошедшие проверку регексом - обработке не подлежат на данном этапе
	//Мусор - чистится от служебных символов
	//В зависимости от того что было в начале порядок разделения будет разным
	if (firstOrden)
	{
		for (auto it = symbolIndexes.begin(); it != symbolIndexes.end(); ++it)
		{
			xpression.push_back(buffer.substr(prevIndex + prevSize, it->first - prevIndex - prevSize));
			prevIndex = it->first;
			prevSize = it->second;
			garbage.push_back(buffer.substr(prevIndex, prevSize));
		}
	}
	else
	{
		for (auto it = symbolIndexes.begin(); it != symbolIndexes.end(); ++it)
		{
			garbage.push_back(buffer.substr(prevIndex + prevSize, it->first - prevIndex - prevSize));
			prevIndex = it->first;
			prevSize = it->second;
			xpression.push_back(buffer.substr(prevIndex, prevSize));
		}
	}

	//Если после последнего встреченного фрагмента остался мусор то добавляем его тоже
	if (prevIndex < buffer.size() && firstOrden)
		garbage.push_back(buffer.substr(prevIndex, buffer.size() - (prevIndex)));

	//Если никаких спецсимволов не обнаружено то считаем что это был обычный текст и пропускаем
	if (!hLinkDetect && !hSimpLinkDetect && !hAdvLinkDetect)
		garbage.push_back(buffer);

	for (uint32_t iters = 0; iters < garbage.size(); ++iters)
	{
		//Проходимся по фрагменту заменяя служебные символы маркерами
		for (uint32_t index = 0; index < garbage.at(iters).size(); ++index)
		{
			//Если встречаем повтор символа то удаляем с предыдущей позиции
			if (!symbol.empty())
				if (garbage.at(iters).at(index) == symbol.at(0))
				{
					symbolIndexes.insert(std::map<int, int>::value_type(symbolDuplicate, replaceIndex));
					garbage.at(iters).replace(symbolDuplicate, 1, marker);
					symbolDuplicate = index;
				}

			//Если новый встречается новый символ то считаем что очистка до него завершена
			//и поэтому чистим повторы нового символа
			if (symbolCollection.find(garbage.at(iters).at(index)) <= symbolCollection.size() && !symbol.empty())
				if (garbage.at(iters).at(index) != symbol.at(0))
					symbol.clear();

			//Если спецсимвол первый раз попался то запоминаем его
			if (symbolCollection.find(garbage.at(iters).at(index)) <= symbolCollection.size() && symbol.empty())
			{
				replaceIndex = symbolCollection.find(garbage.at(iters).at(index));
				symbol = symbolCollection.at(replaceIndex);
				symbolDuplicate = index;
			}
		}

		//Ищем остатки проходясь с начала в конец(поиск закрывающих символов
		for (uint32_t index = 0; index < garbage.at(iters).size(); ++index)
		{
			//Как только находим первый попавшийся - убираем и выходим из цикла
			if (symbolClearanceBack.find(garbage.at(iters).at(index)) <= symbolClearanceBack.size())
			{
				symbolIndexes.insert(std::map<int, int>::value_type(index, symbolClearanceBack.find(garbage.at(iters).at(index))));
				garbage.at(iters).replace(index, 1, marker);
				break;
			}
		}

		//Ищем остатки проходясь с конца в начало(поиск открывающих символов)
		for (uint32_t index = garbage.at(iters).size() - 1; index > 0; --index)
		{
			if (garbage.at(iters).empty())
				break;
			//Как только находим первый попавшийся - убираем и выходим из цикла
			if (symbolClearanceFront.find(garbage.at(iters).at(index)) <= symbolClearanceFront.size())
			{
				symbolIndexes.insert(std::map<int, int>::value_type(index, symbolClearanceFront.find(garbage.at(iters).at(index))));
				garbage.at(iters).replace(index, 1, marker);
				break;
			}
		}

		//Заменяем маркеры на html имитаторы спецсимволов
		for (std::map<int, int>::reverse_iterator it = symbolIndexes.rbegin(); it != symbolIndexes.rend(); ++it)
		{
			//Заменяем лишние символы руководствуясь содержимым карты
			switch (it->second)
			{
				case 0:
				{
					garbage.at(iters).replace(it->first, 1, replaceSymbols[it->second]);
					break;
				}
				case 1:
				{
					garbage.at(iters).replace(it->first, 1, replaceSymbols[it->second]);
					break;
				}
			}
		}

		//Чистим позицию символа, чистим карту символов перед следующей итерацией
		symbolDuplicate = 0;
		symbolIndexes.clear();
		symbol.clear();	//Очищаем символ чтобы не было ложного срабатывания в некст итерации
	}

	//Чистим буффер т.к старые данные в нём больше не нужны
	buffer.clear();

	if (firstOrden)	//Если сначала у нас полезный контент то пихаем его
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
			if(index < xpression.size())
				buffer += xpression.at(index);
		}
		//Если остался дополнительный контент, то добавляем, аналогично как с мусором
		if (garbage.size() < xpression.size())
			buffer += xpression.at(xpression.size());
	}

	//buffer = buffer;

	return buffer;
}
