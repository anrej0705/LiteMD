#pragma once
#include <QtWidgets>
#include <map>
#include "ui_update_event.h"
#include "xmlWriter.h"
class appSettings : public QDialog
{
	Q_OBJECT
	private:
		QWidget* basicSettings;			//Основные настройки
		QWidget* renderSettings;		//Настройки обработки текста
		QWidget* downloaderSettings;	//Настройка загрузчика по сети
		QWidget* tabLogs;				//Логи работы
		QWidget* extendedMd;			//Вкладка расширенные
		QWidget* hacks;					//Вкладка хаков

		std::map<uint8_t, QString>* loc_map;	//Список локалей

		QWidget* capTab;				//Заглушка

		xmlWriter* xmlw;				//Менеджер XML

		QTranslator lmd_lng;

		std::map<uint8_t, std::string> themeNamesList;	//Список тем

		QTabWidget* settingsLister;		//Менеджер вкладок
		QLabel* workprogress;			//Заглушка
		QPushButton* btnOk;				//Кнопка подтверждения
		QPushButton* btnCancel;			//Кнопка отмены
		QPushButton* btnApply;			//Кнопка применить изменения
		QPushButton* setDefault;		//Кнопка "По умолчанию"
		QHBoxLayout* controlBtnLay;		//Менеджеры размещения кнопок снизу
		QHBoxLayout* setDefBtnLay;		//Для особенной кнопочки запуска ракет по америке
		QHBoxLayout* btnComposer;		//Компонует два предыдущих - один для кнопки сброса настроек второй для остальных
		QVBoxLayout* dialogWindow;		//Менеджер общего размещения элементов

		//Вкладка "Основные"
		QLabel* langListHint;			//Описание выпадающего списка для смены языка
		QComboBox* langList;			//Выпадающий список для смены языка

		QLabel* themeHint;				//Описание выпадающего списка для смены темы
		QComboBox* themeList;			//Выпадающий список для смены темы

		QLabel* colorThemeHint;			//Описание выпадающего списка для цветовой схемы
		QComboBox* colorTheme;			//Выпадающий список для смены цветовой схемы

		QLabel* saveSettingsHint;		//Описание выпадающего списка для выбора способа сохранения настроек
		QComboBox* saveSettings;		//Выпадающий список для сохранения настроек

		QLabel* saveLocationHint;		//Описание выпадающего списка выбора пути сохранения
		QComboBox* saveLocation;		//Выпадающий список выбора пути сохранения

		QLabel* userPathHint;			//Описание к полю ввода пользовательского пути
		QLineEdit* userPath;			//Поле ввода пользовательского пути

		QLabel* saveFreqHint;			//Описание выпадающего списка для выбора частоты автосейва
		QComboBox* saveFreq;			//Выпадающий список выбора частоты сохранения

		QLabel* autoSaveHint;			//Описание для галки автосейва
		QCheckBox* autoSave;			//Галка для выбора автосейва

		QLabel* depFuncHint;			//Описание для галки устаревшего функционала
		QCheckBox* depFunc;				//Галка активации устаревшего функционала

		QLabel* inDevFuncHint;			//Описание для галки функционала находящегося в разработке
		QCheckBox* devFunc;				//Галка для активации функционала в разработке

		QLabel* extendedHind;			//Описание для чекбокса расширенных функций
		QCheckBox* extendedFunc;		//Чекбокс для расширенных фунцион

		QLabel* hacksHint;				//Описание для всяких взломов жопы стандарта
		QCheckBox* hacksEnable;			//Чекпук для активации взлома жопы MD

		QLabel* msgLimitHint;			//Описание поля максимума строк логов
		QSpinBox* limitSpinBox;			//Крутилка для задания лимита - по умолчанию 8192
		//-------------------------

		//Вкладка "Рендер"
		QLabel* reloadHint;				//Подсказка - настройки сработают после повторного открытия
		QLabel* parseSimplLinksHint;	//Описание галки обработки ссылок <url>
		QLabel* parseAdvLinksHint;		//Описание галки обработки [link](url)
		QLabel* parseHeaderLvlHint;		//Описание галки обработки заголовков #...#####
		QLabel* parseItalicHint;		//Описание галки оюработки курсива *...*
		QLabel* parseBoldHint;			//Описание галки обработки жирного текста **...**
		QCheckBox* parseSimplLinks;		//Галка обработки ссылок <url>
		QCheckBox* parseAdvLinksl;		//Галка обработки [link](url)
		QCheckBox* parseHeaderLvl;		//Галка обработки заголовков #...#####
		QCheckBox* parseItalic;			//Галка для обработки курсива *...*
		QCheckBox* parseBold;			//Галка для обработки жирного текста **...**
		QLabel* deprSyntaxPrepHint;		//Подпись к чекбоксу устаревшего препроцессора
		QLabel* deprSyntaxPostHint;		//Подпись к чекбоксу устаревшего пост процессора
		QLabel* deprUrlSimplParserHint;	//Подпись к чекбоксу упрощённого парсера 1
		QLabel* deprUrlAdvParserHint;	//Подпись к чекбоксу парсера форматированных ссылок
		QLabel* deprUrlBasParserHint;	//Подпись к чекбоксу упрощённого парсера 2
		QCheckBox* deprSyntaxPrep;		//Чекбокс устаревшего препроцессора
		QCheckBox* deprSyntaxPost;		//Чекбокс устаревшего пост процессора
		QCheckBox* deprUrlSimplParser;	//Чекбокс упрощённого парсера 1
		QCheckBox* deprUrlAdvParser;	//Чекбокс парсера форматированных ссылок
		QCheckBox* deprUrlBasParser;	//Чекбокс упрощённого парсера 2
		//-------------------------

		//Вкладка "Загрузчик"
		QLabel* allowWarningsHint;		//Разрешить предупреждать об устаревшем функционале
		QCheckBox* allowWarnings;		//Флажок предупреждения

		QLabel* allowCacheHint;			//Разрешить кеширование из интернета(папка cache по умолчанию)
		QCheckBox* allowCache;			//Флажок флажок разрешить кеширование
		//-------------------------

		//Вкладка "Логи"
		QLabel* logHint;				//Подпись окна с логами
		QPlainTextEdit* logBox;			//Контейнер для отображения логов
		QPushButton* clearLog;			//Кнопка "Очистить лог"
		QPushButton* saveLog;			//Кнопка "Сохранить лог"
		//-------------------------

		//Вкладка расширенные
		QLabel* parseStrikethroughHint;	//Подпись к чекбосу парсинга зачёркивания
		QCheckBox* parseStrikethrough;	//Чекбос парсинга зачёркивания
		//-------------------

		//Вкладка "Хаки"
		QLabel* parseUnderlinedHint;	//Подсказка к чекбоксу парсинга подчёркивания
		QLabel* compatilibtyUndrHint;	//Подсказка к чекбоксу совместимости с Qt5
		QCheckBox* parseUnderlined;		//Чекбокс парсинга подчёркивания
		QCheckBox* combatilibtyUndr;	//Чекбокс совместимости с рендером Qt5
		//--------------

		void configureBasicSettingsTab();
		void configureRenderSettingsTab();
		void configureDownloaderSettingsTab();
		void configureLogsTab();
		void configureExtendedTab();
		void configureHackTab();
		//void titleChanged(const QString&);
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
		void slot_save_logs();
		void slot_dparswitch_en_t_prep(int);
		void slot_en_t_post(int);
		void slot_en_url_bas_simple(int);
		void slot_en_url_bas(int);
		void slot_en_url_adv(int);
		void slot_switch_underlined(int);
		void slot_switch_strikethrough(int);
		void slot_set_limit(int);
		void slot_reset_settings();
		void slot_switch_compat(int);
		void slot_en_italic(int);
		void slot_ui_change(int);
		void slot_en_bold(int);
	signals:
		void signalTitleChanged(const QString&);
};
QString getConfigPath();
