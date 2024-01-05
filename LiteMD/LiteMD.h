#pragma once
#include <QtWidgets>
#include "ui_LiteMD.h"
#include "mdEditor.h"
#include "mdScreen.h"
#include "GuiDownloader.h"
#include "appSettings.h"

//Номер билда, пока задаётся вручную
#define buildNumber 1006

class LiteMD : public QMainWindow
{
    Q_OBJECT
	private:
		Ui::LiteMDClass ui;				//Не используется
		QDockWidget* quick_access_dock;	//Док для кнопок
		QMenu* mHelp;					//Меню помощи
		QMenu* mFile;					//Меню файла
		QMenu* mSettings;				//Меню настроек
		mdEditor* mde;					//Редактор
		mdScreen* mds;					//Рендер текста
		DownloaderGui* dwModule;		//Модуль загрузчика
		QString defTitle;				//Контейнер дефолтного заголовка
		QLabel* workProgressCap;		//Затычка на недоделанные элементы
		appSettings* mdlSet;			//Диалог настроек приложения
		QGroupBox* editorWindow;		//Окно редактора
		QGroupBox* viewerWindow;		//Окно рендера
		QAction* actAbout;				//Пункт меню о программе
		QAction* actOpen;				//Пункт меню открыть
		QAction* actSave;				//Пункт меню сохранить
		QAction* actSaveAs;				//Пункт меню сохранить как
		QAction* actQuit;				//Пункт меню выход
		QAction* actDownloader;			//Пункт вызова загрузчика
		QAction* actSet;				//Пункт вызова настроек
		QAction* actNew;				//Пункт создрания нового документа
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		void closeEvent(QCloseEvent*);		//Перехватчик закрытия приложения
		LiteMD(QWidget *parent = nullptr);	//Базовый конструкор
		~LiteMD();
	signals:
		void saveFile();	//Сохранение файла
	private slots:
		void slotAbout();	//Слот для вызова окна о программе
		void httpModuleShow();	//Сигнал для вызова модуля загрузчика
		void slot_mbar_send_string(const QString&);	//Слот для смены заголовка
	public slots:
		void slotTitleChanged(const QString&);	//Слот для изменения заголовка
		void slotFileEdited();	//Слот для установки флага "*"
		void slotTitleReset();	//Слот для сброса заголовка к дефолтному
};
