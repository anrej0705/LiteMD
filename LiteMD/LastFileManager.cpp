#include "LastFileManager.h"

#include <algorithm>
#include <fstream>
#include <iterator>

// Базовый конструктор.
LastFileManager::LastFileManager(std::string path)
{
	path_ = path;

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
		lastFilePaths_.erase(lastFilePaths_.end());
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
