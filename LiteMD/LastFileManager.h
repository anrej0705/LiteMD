#ifndef LAST_FILE_MANAGER_H
#define LAST_FILE_MANAGER_H

#include <string>
#include <deque>

// Менеджер списка последних открытых файлов.
class LastFileManager
{
private:
	std::deque<std::string> lastFilePaths_; // Список последних открытых файлов.
	const std::string path_; // Путь к файлу настроек сохранения.
	const int numberOfRecords_;

public:
	LastFileManager(); //  Конструктор по умолчанию.
	LastFileManager(int numberOfRecords); //  Конструктор с указанием размера списка последних открытых файлов.
	LastFileManager(std::string path, int numberOfRecords); //  Конструктор с указанием пути и размера списка последних открытых файлов.

	void save() const; // Сохраняет список последних открытых файлов.
	const std::deque<std::string>& getFiles() const { return lastFilePaths_; } // Возвращает список последних открытых файлов.
	void addFile(std::string path); // Добавляет файл в список последних открытых файлов.
};

#endif // !LAST_FILE_MANAGER_H
