#ifndef LAST_FILE_MANAGER_H
#define LAST_FILE_MANAGER_H

#include <string>
#include <deque>

class LastFileManager
{
private:
	std::deque<std::string> lastFilePaths_;
	std::string path_;
public:
	LastFileManager(std::string path);
	void save() const;
	const std::deque<std::string>& getFiles() const { return lastFilePaths_; }
	void addFile(std::string path);
};

#endif // !LAST_FILE_MANAGER_H
