#include "globalFlags.h"
#include <stdbool.h>

//settings settingsStruct;

bool fileOpenedState=0;
bool fileChangedState=0;
bool appTitleUpdated=0;
bool appClose = 0;
//Deprecated
bool allowHttpWarn = 1;
bool httpDerpWarned = 0;

//���������
bool logReadState = 0;

//��������� �������������� �����, ��� ���� SilverWolf2k20 -> ��������� �����
char** lastFiles = NULL;
void newRecentFilesArray()
{
	lastFiles = (char**)malloc(NUMBER_OF_FILES * sizeof(char*));	//�������������� ������
	for (uint8_t index = 0; index < NUMBER_OF_FILES; ++index)	//�������������� ������ � �������
	{
		lastFiles[index] = (char*)malloc(FILENAME_SIZE * sizeof(char));
		for (uint16_t colIndex = 0; colIndex < FILENAME_SIZE; ++colIndex)
			lastFiles[index][colIndex] = (char)NULL;  //������ �� ������
	}
}

void deleteOnExit()
{
	//������� ������
	for (uint8_t index = 0; index < NUMBER_OF_FILES; ++index)
	{
		free(lastFiles[index]);
	}
	free(lastFiles);
}

void deleteName(uint8_t pos)
{
	//������ ������ �� �������, memset ��������� ������(������� ���������)
	for (uint16_t subIndex = 0; subIndex < FILENAME_SIZE; ++subIndex)
	{
		lastFiles[pos][subIndex] = (char)NULL;
	}
}

void setName(uint8_t pos, char* source)
{
	//�������� ������
	strcpy(lastFiles[pos], source);
}
