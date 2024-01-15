#pragma once
#ifndef GLOBALFLAGS_H_
#define GLOBALFLAGS_H_
#include <stdbool.h>
#include <stdint.h>
//����� ���������
extern bool fileOpenedState;
extern bool fileChangedState;
extern bool appTitleUpdated;
extern bool appClose;
//Deprecated
extern bool allowHttpWarn;
extern bool httpDerpWarned;

//���������
extern bool logReadState;

//��������� ��������� �������� ������(������ SilverWolf2k20)
#define NUMBER_OF_FILES	4	//������
#define FILENAME_SIZE	256	//�������
extern char** lastFiles;
void newRecentFilesArray();
void deleteOnExit();
void deleteName(uint8_t pos);
void setName(uint8_t pos, char*);
#endif
