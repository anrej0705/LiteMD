#ifndef LAST_FILE_MANAGER_H
#define LAST_FILE_MANAGER_H

#include <string>
#include <deque>

// Менеджер списка последних открытых файлов.
class LastFileManager
{
private:
	std::deque<std::string> lastFilePaths_; // Список последних открытых файлов.
	std::string path_; // Путь к файлу настроек сохранения.
	const int numberOfRecords_{ 3 };
public:
	LastFileManager(); // Конструктор без параметров.
	LastFileManager(std::string path); // Конструктор с указанием пути к списку последних открытых файлов.
	void save() const; // Сохраняет список последних открытых файлов.
	const std::deque<std::string>& getFiles() const { return lastFilePaths_; } // Возвращает список последних открытых файлов.
	void addFile(std::string path); // Добавляет файл в список последних открытых файлов.
};

#endif // !LAST_FILE_MANAGER_H
