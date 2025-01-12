#include "liParser.h"
//��� ������ --->
#include <boost/container/string.hpp>
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
	#include "global_definitions.h"
}
//---> � �� ���� ���� ����������� �� ������� � ������, ������ ����������� ������ �� �����

struct listStr
{
	int32_t li_str_start = -1;
	int32_t li_str_end = -1;
};

struct listStruct
{
	int32_t li_start = -1;
	int32_t li_end = -1;
	std::vector<listStr> strokes;
	bool found = 0;
};

//boost::container::string* li_output;
std::string* li_output;

std::string liParser(std::string& rawInput)
{
	//��� ������ --->
	boost::container::string* log_out = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//������ ���������� � ����������� ��������

	push_log(std::string("[������]������ ����� " + std::to_string(*buffer_size)));

	li_output = new std::string;
	//li_output = new boost::container::string;

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//��������, //������ ����� � �������� ��������� ����� � �������� ��� ����

	li_output->assign(buffer);

	//---> � �� ���� ���� ����������� �� ������� � ������, ������ ����������� ������ �� �����

	std::deque<uint32_t> strokes;
	//�������� ��������� �� �������
	for (uint32_t _index = 0; _index < *buffer_size; ++_index)
	{
		if (_index == 0)
			strokes.push_back(_index);
		if (buffer[_index] == '\n')
		{
			strokes.push_back(_index + 1);
		}
		else if (_index == *buffer_size - 1)
			strokes.push_back(_index);
	}

	listStr li_str;
	listStruct foundList;
	std::deque<listStruct> lists;

	char testpoint;

	bool list_keeps = 0;

	//���� �������� ������
	for (uint16_t _index = 0; _index < strokes.size(); ++_index)
	{
		//��������� ������� �������� "- " ��� "* " ��� "+ " � ���������� ���� �� �������
		testpoint = buffer[strokes.at(_index)];
		testpoint = buffer[strokes.at(_index)];
		if (buffer[strokes.at(_index) == '-'] && buffer[strokes.at(_index) + 1] == ' ')
		{
			if (foundList.li_start == -1)
				foundList.li_start = strokes.at(_index);
			if(!foundList.found)
				foundList.found = 1;
			li_str.li_str_start = strokes.at(_index);
			//���� �����
			for (uint32_t _fnd = strokes.at(_index); _fnd < *buffer_size; ++_fnd)
			{
				if (buffer[_fnd] == '\n' || _fnd == *buffer_size - 1)
				{
					_fnd == *buffer_size - 1 ? li_str.li_str_end = _fnd + 1 : li_str.li_str_end = _fnd;
					_fnd == *buffer_size - 1 ? foundList.li_end = _fnd + 1 : foundList.li_end = _fnd;
					//li_str.li_str_end = _fnd;
					//foundList.li_end = _fnd;
					break;
				}
			}
			foundList.strokes.push_back(li_str);
		}
		else if (foundList.found)
		{
			lists.push_front(foundList);
			foundList.found = 0;
			foundList.li_end = -1;
			foundList.li_start = -1;
			foundList.strokes.clear();
		}
	}

	//��������� ������
	for (int16_t _lists = 0; _lists < lists.size(); ++_lists)
	{
		li_output->insert(lists.at(_lists).li_end, li_list_iclose);
		for (int16_t _li_str = lists.at(_lists).strokes.size() - 1; _li_str >= 0; --_li_str)
		{
			li_output->insert(lists.at(_lists).strokes.at(_li_str).li_str_end, li_str_iclose);
			li_output->replace(lists.at(_lists).strokes.at(_li_str).li_str_start, 2, li_str_iopen);
		}
		li_output->insert(lists.at(_lists).li_start, li_list_iopen);
	}
	/*for (uint16_t _lists = 0; _lists < lists.size(); ++_lists)
	{
		li_output->insert(lists.at(_lists).li_end + 1, li_list_iclose);
		//������� ������ �������, ��-�� ��������� ������� ��� �� ����� ���� ���������� � �����
		li_output->insert(lists.at(_lists).li_end + 1, li_str_iclose);
		li_output->replace(lists.at(_lists).strokes.at(0), 2, li_str_iopen);
		for (uint16_t _li_str = 1; _li_str < lists.at(_lists).strokes.size(); ++_li_str)
		{
			li_output->insert(lists.at(_lists).strokes.at(_li_str - 1) - 1, li_str_iclose);
			li_output->replace(lists.at(_lists).strokes.at(_li_str), 2, li_str_iopen);
		}
		li_output->insert(lists.at(_lists).li_start + 1, li_list_iopen);
	}*/

	//��� ������ --->
	free(buffer);
	free(buffer_size);

	return li_output->c_str();
	//---> � �� ���� ���� ����������� �� ������� � ������, ������ ����������� ������ �� �����

	return rawInput;
}
