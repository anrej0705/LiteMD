#pragma once
#include <QtWidgets>
#include <map>
#include "ui_update_event.h"
class appSettings : public QDialog
{
	Q_OBJECT
	private:
		QWidget* basicSettings;	//Основные настройки
		QWidget* renderSettings;	//Настройки обработки текста
		QWidget* downloaderSettings;//Настройка загрузчика по сети

		std::map<uint8_t, QString>* loc_map;	//Список локалей

		QWidget* capTab;	//Заглушка

		QTranslator lmd_lng;

		QTabWidget* settingsLister;	//Менеджер вкладок
		QLabel* workprogress;	//Заглушка
		QPushButton* btnOk;		//Кнопка подтверждения
		QPushButton* btnCancel;	//Кнопка отмены
		QPushButton* btnApply;	//Кнопка применить изменения
		QHBoxLayout* controlBtnLay;	//Менеджеры размещения кнопок снизу
		QVBoxLayout* dialogWindow;	//Менеджер общего размещения элементов

		//Вкладка "Основные"
		QLabel* langListHint;	//Описание выпадающего списка для смены языка
		QComboBox* langList;	//Выпадающий список для смены языка

		QLabel* themeHint;		//Описание выпадающего списка для смены темы
		QComboBox* themeList;	//Выпадающий список для смены темы

		QLabel* saveSettingsHint;//Описание выпадающего списка для выбора способа сохранения настроек
		QComboBox* saveSettings;//Выпадающий список для сохранения настроек

		QLabel* saveFreqHint;	//Описание выпадающего списка для выбора частоты автосейва
		QComboBox* saveFreq;	//Выпадающий список выбора частоты сохранения

		QLabel* autoSaveHint;	//Описание для галки автосейва
		QCheckBox* autoSave;	//Галка для выбора автосейва
		//-------------------------

		//Вкладка "Рендер"
		QLabel* parseLinksHint;	//Описание галки обработки ссылок
		QCheckBox* parseLinks;	//Галка обработки ссылок
		//-------------------------

		//Вкладка "Загрузчик"
		QLabel* allowWarningsHint;//
		QCheckBox* allowWarnings;//

		QLabel* allowCacheHint;	//
		QCheckBox* allowCache;	//

		void configureBasicSettingsTab();
		void configureRenderSettingsTab();
		void configureDownloaderSettingsTab();
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		appSettings(QWidget* aWgt = 0);
	public slots:
		void slot_lang_selected(int);
		void slot_apply_settings();
};
