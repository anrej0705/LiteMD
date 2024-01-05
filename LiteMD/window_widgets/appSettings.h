#pragma once
#include <QtWidgets>
#include <map>
#include "ui_update_event.h"
class appSettings : public QDialog
{
	Q_OBJECT
	private:
		QWidget* basicSettings;	//�������� ���������
		QWidget* parserSettings;	//��������� ��������� ������

		std::map<uint8_t, QString>* loc_map;	//������ �������

		QGroupBox* basic_box;

		QTranslator lmd_lng;

		QTabWidget* settingsLister;	//�������� �������
		QLabel* workprogress;	//��������
		QPushButton* btnOk;		//������ �������������
		QPushButton* btnCancel;	//������ ������
		QPushButton* btnApply;	//������ ��������� ���������
		QHBoxLayout* controlBtnLay;	//��������� ���������� ������ �����
		QVBoxLayout* dialogWindow;	//�������� ������ ���������� ���������

		//������� "��������"
		QLabel* langListHint;	//�������� ����������� ������ ��� ����� �����
		QComboBox* langList;	//���������� ������ ��� ����� �����

		QLabel* themeHint;		//�������� ����������� ������ ��� ����� ����
		QComboBox* themeList;	//���������� ������ ��� ����� ����

		QLabel* saveSettingsHint;//�������� ����������� ������ ��� ������ ������� ���������� ��������
		QComboBox* saveSettings;//���������� ������ ��� ���������� ��������

		QLabel* saveFreqHint;	//�������� ����������� ������ ��� ������ ������� ���������
		QComboBox* saveFreq;	//���������� ������ ������ ������� ����������

		QLabel* autoSaveHint;	//�������� ��� ����� ���������
		QCheckBox* autoSave;	//����� ��� ������ ���������

		//ui_update_event* ui_event;

		void configureBasicSettingsTab();
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		appSettings(QWidget* aWgt = 0);
	public slots:
		void slot_lang_selected(int);
		void slot_apply_settings();
};
