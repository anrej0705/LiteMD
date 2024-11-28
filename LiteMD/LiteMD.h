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
		//Ui::LiteMDClass ui;			//Не используется
		QToolBar* quick_tb;				//Тулбар для кнопок-ярлыков
		QToolBar* serv_tb;				//Тулбар для кнопок помощи, о проге, Bug!
		QMenu* mHelp;					//Меню помощи
		QMenu* mFile;					//Меню файла
		QMenu* mEdit;					//Меню "Правка"
		QMenu* mSettings;				//Меню настроек
		mdEditor* mde;					//Редактор
		mdScreen* mds;					//Рендер текста
		DownloaderGui* dwModule;		//Модуль загрузчика
		QString defTitle;				//Контейнер дефолтного заголовка
		QLabel* workProgressCap;		//Затычка на недоделанные элементы
		appSettings* mdlSet;			//Диалог настроек приложения
		QGroupBox* editorWindow;		//Окно редактора
		QGroupBox* viewerWindow;		//Окно рендера
		currentChangelog* cLog;			//Список изменений
		xmlWriter* xmlW;				//Класс для записи настроек в файл
		xmlReader* xmlR;				//Читалка настроек
		QTimer* showTim;				//Таймер на отображение окна
		logger* logWindow;				//Окно логов которое появляется нажатием на жука
		QToolButton* actPlaceHeader;	//Кнопка "превратить в заголовок"
		QToolButton* actSetTextFormat;	//Выбрать форматирование выделенного текста

		//Действия всякие, в меню и в док
		QAction* actAbout;				//Пункт меню о программе
		QAction* actOpen;				//Пункт меню открыть
		QAction* actSave;				//Пункт меню сохранить
		QAction* actSaveAs;				//Пункт меню сохранить как
		QAction* actQuit;				//Пункт меню выход
		QAction* actDownloader;			//Кнопка вызова загрузчика
		QAction* actSet;				//Кнопка вызова настроек
		QAction* actNew;				//Кнопка создания нового документа
		QAction* actPlaceUrl;			//Преобразовать в (ссылку)
		QAction* actPlaceAltUrl;		//Преобразовать в [альтернативную](ссылку)
		QAction* actHelp;				//Открывает справку
		QAction* actOpenChangelog;		//Открывает список изменений текущей версии
		QAction* actBugReport;			//Кнопка открытия окна логов
		QAction* actShieldSymbol;		//Кнопка экранирования символа на котором стоит курсор

		//Действия менюшки заголовков
		QAction* actSetH1;				//H1
		QAction* actSetH2;				//H2
		QAction* actSetH3;				//H3
		QAction* actSetH4;				//H4
		QAction* actSetH5;				//H5

		//Действия менюшки форматирования
		QAction* setBold;				//Жирный
		QAction* setItalic;				//Курсив
		QAction* setUnderlined;			//Подчёркнутый
		QAction* setStrikethrough;		//Зачёркнутый

		//Выпадающие менюшки из тулбара
		QMenu* headersMenu;				//Выпадающая менюшка выбора заголовков
		QMenu* formatStyle;				//Выпадающая менюшка выбора стиля форматирования
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
		void update_interactive();
		void localeDetector(void);
	public:
		void closeEvent(QCloseEvent*);		//Перехватчик закрытия приложения
		LiteMD(QWidget *parent = nullptr);	//Базовый конструкор
		~LiteMD();
	signals:
		void saveFile();		//Сохранение файла
		void switch_lang(int);	//Сигнал смены языка
	private slots:
		void slotAbout();	//Слот для вызова окна о программе
		void httpModuleShow();	//Сигнал для вызова модуля загрузчика
		void slot_mbar_send_string(const QString&);	//Слот для смены заголовка
	public slots:
		void slotTitleChanged(const QString&);	//Слот для изменения заголовка
		void slotFileEdited();	//Слот для установки флага "*"
		void slotTitleReset();	//Слот для сброса заголовка к дефолтному
};
