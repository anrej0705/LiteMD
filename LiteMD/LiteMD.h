#pragma once
#include <QtWidgets>
#include "ui_LiteMD.h"
#include "mdEditor.h"
#include "mdScreen.h"
#include "GuiDownloader.h"
#include "appSettings.h"

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
