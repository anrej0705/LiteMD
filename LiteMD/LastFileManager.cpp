#include "LastFileManager.h"

#include <algorithm>
#include <fstream>
#include <iterator>

LastFileManager::LastFileManager(std::string path)
{
	path_ = path;

	std::ifstream file(path);

	if (file.is_open() == false)
		return;

	for (std::string line; std::getline(file, line); lastFilePaths_.push_back(line));

	file.close();
}

void LastFileManager::addFile(std::string path)
{
	auto iterator = std::find(std::begin(lastFilePaths_), std::end(lastFilePaths_), path);

	if (iterator != lastFilePaths_.end())
		lastFilePaths_.erase(iterator);

	lastFilePaths_.push_front(path);

	if (lastFilePaths_.size() > 3)
		lastFilePaths_.erase(lastFilePaths_.begin());
}

void LastFileManager::save() const
{
	std::ofstream file(path_, std::ios::trunc);

	std::copy(
		std::cbegin(lastFilePaths_),
		std::cend(lastFilePaths_),
		std::ostream_iterator<std::string>(file, "\n"));

	file.close();
}
