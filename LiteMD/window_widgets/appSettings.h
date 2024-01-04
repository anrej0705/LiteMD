#pragma once
#include <QtWidgets>
#include <map>
class appSettings : public QDialog
{
	Q_OBJECT
	private:
		QWidget* basicSettings;	//Основные настройки
		QWidget* parserSettings;	//Настройки обработки текста
		QComboBox* langList;

		std::map<uint8_t, QString>* loc_map;	//Список локалей

		QTranslator lmd_lng;

		QTabWidget* settingsLister;	//Менеджер вкладок
		QLabel* workprogress;	//Заглушка
		QPushButton* btnOk;		//Кнопка подтверждения
		QPushButton* btnCancel;	//Кнопка отмены
		QPushButton* btnApply;	//Кнопка применить изменения
		QHBoxLayout* controlBtnLay;	//Менеджеры размещения кнопок снизу
		QVBoxLayout* dialogWindow;	//Менеджер общего размещения элементов

		//Вкладка "Основные"
		QLabel* langlistHint;

		void configureBasicSettingsTab();
	public:
		appSettings(QWidget* aWgt = 0);
		void update_ui();
	public slots:
		void slot_lang_selected(int);
		void slot_apply_settings();
};
