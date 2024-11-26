#pragma once
#include <QtWidgets>
#include <map>
#include "ui_update_event.h"
#include "xmlWriter.h"
class appSettings : public QDialog
{
	Q_OBJECT
	private:
		QWidget* basicSettings;		//Основные настройки
		QWidget* renderSettings;	//Настройки обработки текста
		QWidget* downloaderSettings;//Настройка загрузчика по сети
		QWidget* tabLogs;			//Логи работы

		std::map<uint8_t, QString>* loc_map;	//Список локалей

		QWidget* capTab;	//Заглушка

		xmlWriter* xmlw;	//Менеджер XML

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

		QLabel* depFuncHint;	//Описание для галки устаревшего функционала
		QCheckBox* depFunc;		//Галка активации устаревшего функционала

		QLabel* inDevFuncHint;	//Описание для галки функционала находящегося в разработке
		QCheckBox* devFunc;		//Галка для активации функционала в разработке
		//-------------------------

		//Вкладка "Рендер"
		QLabel* reloadHint;			//Подсказка - настройки сработают после повторного открытия
		QLabel* parseSimplLinksHint;//Описание галки обработки ссылок <url>
		QLabel* parseAdvLinksHint;	//Описание галки обработки [link](url)
		QLabel* parseHeaderLvlHint;	//Описание галки обработки заголовков #...#####
		QCheckBox* parseSimplLinks;	//Галка обработки ссылок <url>
		QCheckBox* parseAdvLinksl;	//Галка обработки [link](url)
		QCheckBox* parseHeaderLvl;	//Галка обработки заголовков #...#####
		//-------------------------

		//Вкладка "Загрузчик"
		QLabel* allowWarningsHint;	//Разрешить предупреждать об устаревшем функционале
		QCheckBox* allowWarnings;	//Флажок

		QLabel* allowCacheHint;		//Разрешить кеширование из интернета(папка cache по умолчанию)
		QCheckBox* allowCache;		//Флажок
		//-------------------------

		//Вкладка "Логи"
		QLabel* logHint;			//Подпись окна с логами
		QPlainTextEdit* logBox;			//Контейнер для отображения логов
		QPushButton* clearLog;		//Кнопка "Очистить лог"
		QPushButton* saveLog;		//Кнопка "Сохранить лог"
		//-------------------------


		void configureBasicSettingsTab();
		void configureRenderSettingsTab();
		void configureDownloaderSettingsTab();
		void configureLogsTab();
		void titleChanged(const QString&);
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_interactive();
		void update_ui();
	public:
		appSettings(QWidget* aWgt = 0);
	public slots:
		void slot_lang_selected(int);
		void slot_apply_settings();
		void slot_switch_warn_allow(int);
		void slot_switch_deprecated(int);
		void slot_switch_features(int);
		void slot_switch_simple_url_parser(int);
		void slot_switch_adv_url_parser(int);
		void slot_switch_header_lvl_parser(int);
		void slot_tab_changed(int);
		void slot_clear_logs();
	signals:
		void signalTitleChanged(const QString&);
};
