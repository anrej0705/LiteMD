#pragma once
#include <QtWidgets>
//#include "ui_LiteMD.h"
#include "mdEditor.h"
#include "mdScreen.h"
#include "GuiDownloader.h"
#include "appSettings.h"
#include "frameChangelog.h"
#include "xmlWriter.h"
#include "xmlReader.h"
#include "logger.h"
#include "event_id_constructor.h"
#include "customToolButton.h"
#include "OrientalPushButton.h"
#include "helpCenter.h"
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}

//Номер билда, пока задаётся вручную
//UPD patch 0.1.9 теперь вынесен в global_definitions.h

class LiteMD : public QMainWindow
{
    Q_OBJECT
	private:
		OrientablePushButton* btnDown;		//Пролистывание страницы вниз
		OrientablePushButton* btnUp;		//Пролистывание страницы вверх
		//Ui::LiteMDClass ui;				//Не используется
		QToolBar* quick_tb;					//Тулбар для кнопок-ярлыков
		QToolBar* serv_tb;					//Тулбар для кнопок помощи, о проге, Bug!
		QMenu* mHelp;						//Меню помощи
		QMenu* mFile;						//Меню файла
		QMenu* mEdit;						//Меню "Правка"
		QMenu* mSettings;					//Меню настроек
		QMenu* recentFiles;					//Меню "Недавние файлы"
		mdEditor* mde;						//Редактор
		mdScreen* mds;						//Рендер текста
		DownloaderGui* dwModule;			//Модуль загрузчика
		QString defTitle;					//Контейнер дефолтного заголовка
		QLabel* workProgressCap;			//Затычка на недоделанные элементы
		appSettings* mdlSet;				//Диалог настроек приложения
		QGroupBox* editorWindow;			//Окно редактора
		QGroupBox* viewerWindow;			//Окно рендера
		currentChangelog* cLog;				//Список изменений
		xmlWriter* xmlW;					//Класс для записи настроек в файл
		xmlReader* xmlR;					//Читалка настроек
		helpCenter* help;					//Окно справки
		QTimer* showTim;					//Таймер на отображение окна
		logger* logWindow;					//Окно логов которое появляется нажатием на жука
		CustomToolButton* actPlaceHeader;	//Кнопка "превратить в заголовок"
		CustomToolButton* actSetTextFormat;	//Выбрать форматирование выделенного текста
		QScrollArea* mdsArea;				//Хандлер полосы прокрутки
		QPushButton* dirSwitch1;			//Кнопка переключения направления 1
		QPushButton* dirSwitch2;			//Кнопка переключения направления 2
		QPushButton* manageDir;				//Кнопка переключения управление направлением авто/ручное
		QPushButton* syncCtlBtn;			//Управление синхронным пролистыванием

		QLabel* hintsList[12];				//Визуал между окнами рендера и редактора

		//Действия всякие, в меню и в док
		QAction* actAbout;					//Пункт меню о программе
		QAction* actOpen;					//Пункт меню открыть
		QAction* actSave;					//Пункт меню сохранить
		QAction* actSaveAs;					//Пункт меню сохранить как
		QAction* actClose;					//Пунктик закрытия файла(PS 1917 - давно уже пора было добавить)
		QAction* actQuit;					//Пункт меню выход
		QAction* actDownloader;				//Кнопка вызова загрузчика
		QAction* actSet;					//Кнопка вызова настроек
		QAction* actNew;					//Кнопка создания нового документа
		QAction* actPlaceUrl;				//Преобразовать в (ссылку)
		QAction* actPlaceAltUrl;			//Преобразовать в [альтернативную](ссылку)
		QAction* actHelp;					//Открывает справку
		QAction* actOpenChangelog;			//Открывает список изменений текущей версии
		QAction* actBugReport;				//Кнопка открытия окна логов
		QAction* actShieldSymbol;			//Кнопка экранирования символа на котором стоит курсор
		QAction* checkUpdates;				//Пункт меню проверки обновлений
		QAction* actclearRecent;			//Чистит список последних файлов

		//Действия менюшки заголовков
		QAction* actSetH1;					//H1
		QAction* actSetH2;					//H2
		QAction* actSetH3;					//H3
		QAction* actSetH4;					//H4
		QAction* actSetH5;					//H5

		//Действия менюшки форматирования
		QAction* setBold;					//Жирный
		QAction* setItalic;					//Курсив
		QAction* setUnderlined;				//Подчёркнутый
		QAction* setStrikethrough;			//Зачёркнутый

		//Выпадающие менюшки из тулбара
		QMenu* headersMenu;					//Выпадающая менюшка выбора заголовков
		QMenu* formatStyle;					//Выпадающая менюшка выбора стиля форматирования
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
		void update_interactive();
		void localeDetector(void);
	public:
		std::string fileFullPath;			//Путь до файла
		void closeEvent(QCloseEvent*);		//Перехватчик закрытия приложения
		//bool addRecentString(std::string&);	//Добавляет пункт меню
		//bool removeRecentString();			//Удаляет последний пункт меню
		LiteMD(int, char**, QWidget* parent = nullptr);	//Конструктор для получения аргументов
		~LiteMD();
	private:
		void initLastFileMenu();			//Инициализирует список последних файлов.
	signals:
		void saveFile();					//Сохранение файла
		void switch_lang(int);				//Сигнал смены языка
	private slots:
		void slotAbout();					//Слот для вызова окна о программе
		void httpModuleShow();				//Сигнал для вызова модуля загрузчика
		void slot_mbar_send_string(const QString&);	//Слот для смены заголовка
	public slots:
		void slotTitleChanged(const QString&);	//Слот для изменения заголовка
		void slotFileEdited();				//Слот для установки флага "*"
		void slotTitleReset();				//Слот для сброса заголовка к дефолтному
		void slotCheckUpdates();			//Слот для проверки обновлений
		void slotFileClose();				//Закрытие файла и очистка поля ввода
		void slotMdsDown();					//Слот прокрутки вниз
		void slotMdsUp();					//Слот прокрутки вверх
		void slotSwitchDir();				//Переключение направления
		void slotManageDir();				//Переключение режима руч/авт
		void slotSwitchSync();				//Переключение режима сихронизации
		void slotScrollEvent(int);			//Слот обработки событий листания
		void slotRemoveRf();				//Чистит список файлов
};
//Простой, как сатиновые трусы, метод возвращает QString каталог
QString getAppPath();
QIcon setAppIcon();
