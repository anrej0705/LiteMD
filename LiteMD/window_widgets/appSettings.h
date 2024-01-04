#pragma once
#include <QtWidgets>
#include <map>
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

		void configureBasicSettingsTab();
	public:
		appSettings(QWidget* aWgt = 0);
		void update_ui();
	public slots:
		void slot_lang_selected(int);
		void slot_apply_settings();
};
