#pragma once

#include <QtWidgets>
#include "qmicroz.h"	//DLL!!!

//���������� �� ������� � �������� ������ ������ � ��������	
//�������� ���������� �������� ����������, ������� LiteMD.exe 
//���������������� � LiteMD_old.exe
// 
//���� ������������ ��������� �� ���������� �� ��������� ������ ��������� �
//�������� ����� ������������ �������
// 
//���� ������������ ����������� �� ��������������� �����, 
//����������� ������� commands.txt � LiteMD_old.exe ��������� ������ � �������
//� � ����� ������������ ���������� ��� ���������� �� ���� �����
// 
//��������� ������
//���_�������1 ��������1 ��������2
//���_�������2 ��������1
//���_�������3 ��������1
//���_�������4 ��������1 ��������2
// 
//��������� 1: ������� � 1 ���������� �������� �� �������� ����������
//del "/LiteMD.exe" ����� ���������� ��� "C:/LiteMD/LiteMD.exe"
// 
//��������� 2: ������� � 2 ����������� �������� �� �������� ����� � �������� ����������
//move "/LiteMD.exe" "/LiteMD.exe" �������� ���
//����������� "C:/LiteMD/patch/LiteMD.exe" � "C:/LiteMD/LiteMD.exe"
//
//��������� 3: ������� ren �������� ��� ���������� �� ������ ��������� 2
// 
//��������� 4: ������� lmd_being � lmd_end ����� ��� ������������� ����� ������
//������ ��������� lmd_begin � ������ � lmd_end � �����
// 
//������� � ����� commands.txt ����� ���� ���������� ����:
//	ren "file1" "file2"		: ������������� file1 � file2
//	del "file"				: ������� file
//	move "file" "dir2"		: ����������� ���� file � dir2
//	copy "file" "dir2"		: ����������� ���� file � dir2
//	md "dir"				: ������� ������� dir
//	dd "dir"				: ������� ������� dir
//	log "string"			: �������� � ��� ������ "string"
//	lmd_begin				: ������ ���������
//	lmd_end					: ����� ���������
//������ ������� ����������� ������ \n
//� ������ ������� ���� ����� � map ������
//��������� ����������� ��������
//� ���������� ���������� ������� ����� ������� ��� ������ update.log

class update_manager : public QDialog
{
	Q_OBJECT
	private:
		//���������
		std::vector<std::string> commands_set;				//������ ������ ����������� �� ����� commands.txt
		std::string working_folder;							//��� ����� � ������� ����� ������������ ��������� �����
		std::string command;								//�������
		std::string arg1;									//�������� 1
		std::string arg2;									//�������� 2

		//���� � ������
		int curBuild;										//������� ���� �� globalDefinition
		int fndBuild;										//���� �� �������� ������
		QString patch_name;									//�������� ������ � ������

		//������� �� ������� ������ � �� �������� ����������
		QTableWidget* table;											//�������
		QTableWidgetItem* tabItm = 0;									//������� �������

		//����� �������
		std::map<uint8_t, QString>* lloc_map;							//������ �������
		QTranslator llmd_lng;											//�������� �����������

		//����� ��� �������� ������ ����������
		QDir* updateDir;												//����� ��� ����������
		ZipContentsList* _zip;											//������ ������
		QMicroz* _qmz;													//�����

		//�����
		QTextEdit* logger;												//����� ��� ����� ��� �� ������������ � �������

		//��������� �����������
		QHBoxLayout* buttons;
		QVBoxLayout* layers;											//������ + �������� ���

		//����������
		QGroupBox* main;												//����� � ��������

		QPushButton* btn_confirm;										//�������������
		QPushButton* btn_decline;										//���� ������������
		QPushButton* btn_done;											//������ ������
		QProgressBar* update_progress;									//��� �����, ����� ���� �� ��� ������ �����
		QLabel* question;												//������ "��������?"
	protected:
		void insert_log(std::string input);								//��������� ������ ��� �������� ��������� � ���� � �����
	public:
		update_manager(QString p_name, QWidget* uWgt = 0);
		update_manager::~update_manager();
		bool execute_command(QString command, uint16_t no);				//������ ������
	public slots:
		void slot_confirm();											//������������ ����������
		void slot_decline();											//���� ��������� �� ����������
		void slot_done();												//��������� ������ ���������
		void load_land(int);											//����������� �� appSettings
};
