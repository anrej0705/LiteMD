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

		QTabWidget* settingsLister;	//Менеджер вкладок
		QLabel* workprogress;	//Заглушка
		QPushButton* btnOk;		//Кнопка подтверждения
		QPushButton* btnCancel;	//Кнопка отмены
		QHBoxLayout* controlBtnLay;	//Менеджеры размещения кнопок снизу
		QVBoxLayout* dialogWindow;	//Менеджер общего размещения элементов
		void configureBasicSettingsTab();
	public:
		appSettings(QWidget* aWgt = 0);
	public slots:
		void slot_lang_selected(int);
};
