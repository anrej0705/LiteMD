#include "quoteParser.h"
//��� ������ --->
#include <boost/container/string.hpp>
#include "exceptionHandler.h"
#include "logger_backend.h"
extern "C"
{
#include "global_definitions.h"
}
//---> � �� ���� ���� ����������� �� ������� � ������, ������ ����������� ������ �� �����

struct quoteStr
{
	int32_t quote_str_start = -1;
	int32_t quote_str_end = -1;
};

//boost::container::string* quote_output;
std::string* quote_output;

std::string quoteParser(std::string& rawInput)
{
	//��� ������ --->
	boost::container::string* log_out = new boost::container::string;

	uint32_t* buffer_size = (uint32_t*)malloc(sizeof(uint32_t));
	*buffer_size = rawInput.size();	//������ ���������� � ����������� ��������

	push_log(std::string("[������]������ ����� " + std::to_string(*buffer_size)));

	quote_output = new std::string;
	//li_output = new boost::container::string;

	char* buffer = (char*)calloc(*buffer_size + 1, sizeof(char));
	strcpy(buffer, rawInput.c_str());	//��������, //������ ����� � �������� ��������� ����� � �������� ��� ����

	quote_output->assign(buffer);

	//---> � �� ���� ���� ����������� �� ������� � ������, ������ ����������� ������ �� �����

	push_log("[quoteParser]������� ������� �����");

	uint32_t magic_counter = 0;

	std::deque<uint32_t> strokes;
	//�������� ��������� �� �������
	for (uint32_t _index = 0; _index < *buffer_size; ++_index)
	{
		if (_index == 0)
		{
			strokes.push_back(_index);
			++magic_counter;
		}
		if (buffer[_index] == '\n')
		{
			++magic_counter;
			strokes.push_back(_index + 1);
		}
		else if (_index == *buffer_size - 1)
		{
			strokes.push_back(_index);
			++magic_counter;
		}
	}

	push_log(std::string("[quoteParser]������� " + std::to_string(magic_counter) + " �������"));
	push_log("[quoteParser]��������� ������");
	magic_counter = 0;

	bool quote_found = 0;

	quoteStr quote_str;
	std::deque<quoteStr> lists;

	//���� '>' � ������ �������
	for (volatile uint32_t _idx = 0; _idx < strokes.size(); ++_idx)
	{
		//���� ������� ����������� �� ���� ������ �� ������� ��� ������ �����������
		if (quote_found && buffer[strokes.at(_idx)] != '>')
		{
			quote_found = 0;							//��������� ����
			quote_str.quote_str_end = _idx;				//���������� ������� ��� ���������
			lists.push_back(quote_str);					//��������� ���������
			++magic_counter;							//������� ��������
		}
		//���� ������ ������ '>' �� �������� ����
		if (buffer[strokes.at(_idx)] == '>')
		{
			quote_found = 1;							//�������� ����
			quote_str.quote_str_start = _idx - 1;		//��������� ������� ��� ������
		}
	}

	push_log(std::string("[quoteParser]������� " + std::to_string(magic_counter) + " �����"));
	
	int testpoint2 = 0;

	//��������� ������
	if (lists.size() > 0)
	{
		for (volatile int32_t _idx = lists.size() - 1; _idx >= 0; --_idx)
		{
			testpoint2 = strokes.at(lists.at(_idx).quote_str_start);
			testpoint2 = strokes.at(lists.at(_idx).quote_str_end);
			push_log(std::string("[quoteParser]������� (" + std::to_string(lists.at(_idx).quote_str_start) + "-" + std::to_string(lists.at(_idx).quote_str_end) + ")"));
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_end), "\n");
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_end), blockqoute_str_iclose);
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_start), "\n");
			quote_output->insert(strokes.at(lists.at(_idx).quote_str_start), blockqoute_str_iopen);

		}
	}

	char testpoint1;

	//��� ������ --->
	free(buffer);
	free(buffer_size);

	//return li_output->c_str();
	//---> � �� ���� ���� ����������� �� ������� � ������, ������ ����������� ������ �� �����

	return rawInput;
}
