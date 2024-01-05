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
		QComboBox* langList;

		std::map<uint8_t, QString>* loc_map;	//������ �������

		QTranslator lmd_lng;

		QTabWidget* settingsLister;	//�������� �������
		QLabel* workprogress;	//��������
		QPushButton* btnOk;		//������ �������������
		QPushButton* btnCancel;	//������ ������
		QPushButton* btnApply;	//������ ��������� ���������
		QHBoxLayout* controlBtnLay;	//��������� ���������� ������ �����
		QVBoxLayout* dialogWindow;	//�������� ������ ���������� ���������

		//������� "��������"
		QLabel* langlistHint;

		//ui_update_event* ui_event;

		void configureBasicSettingsTab();
	protected:
		void eventFilter(QEvent* event);
		void update_ui(/*ui_update_event* p_event*/);
	public:
		appSettings(QWidget* aWgt = 0);
	public slots:
		void slot_lang_selected(int);
		void slot_apply_settings();
};

/*class appSettings_filter : public QObject
{
	protected:
		virtual bool eventFilter(QObject*, QEvent*);
	public:
		appSettings_filter(QObject* podj = 0);
};*/
