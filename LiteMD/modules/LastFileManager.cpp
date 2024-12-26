#include "LastFileManager.h"

#include <algorithm>
#include <fstream>
#include <iterator>

#include "appSettings.h" //getConfigPath() (прим. anrej0705)

// Конструктор по умолчанию.
LastFileManager::LastFileManager()
	: LastFileManager(3)
{
}

//appSettings 497:502 (прим. anrej0705)

// Конструктор с указанием размера списка.
LastFileManager::LastFileManager(int numberOfRecords) 
	: LastFileManager(std::string(getConfigPath().toStdString() + "/last_files"), numberOfRecords)
{
}

// Конструктор с указанием пути и размера списка последних открытых файлов.
LastFileManager::LastFileManager(std::string path, int numberOfRecords)
	: path_{path}, numberOfRecords_{ numberOfRecords }
{
	std::ifstream file(path);

	if (file.is_open() == false)
		return;

	for (
		std::string line;
		std::getline(file, line);
		lastFilePaths_.push_back(line));

	file.close();
}

// Добавляет файл в список последних открытых файлов.
void LastFileManager::addFile(std::string path)
{
	auto iterator = std::find(
		std::begin(lastFilePaths_),
		std::end(lastFilePaths_),
		path);

	// Если этот файл уже есть - удалить.
	if (iterator != lastFilePaths_.end())
		lastFilePaths_.erase(iterator);

	lastFilePaths_.push_front(path);

	// Если количество файлов больше 3х - удалить последний элемент.
	if (lastFilePaths_.size() > numberOfRecords_)
		lastFilePaths_.pop_back();
}

// Сохраняет список последних открытых файлов.
void LastFileManager::save() const
{
	std::ofstream file(path_, std::ios::trunc);

	std::copy(
		std::cbegin(lastFilePaths_),
		std::cend(lastFilePaths_),
		std::ostream_iterator<std::string>(file, "\n"));

	file.close();
}
