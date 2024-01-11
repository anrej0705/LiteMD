#pragma once
#include <QtWidgets>
//#include "ui_LiteMD.h"
#include "mdEditor.h"
#include "mdScreen.h"
#include "GuiDownloader.h"
#include "appSettings.h"
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}

//Íîìåð áèëäà, ïîêà çàäà¸òñÿ âðó÷íóþ

class LiteMD : public QMainWindow
{
    Q_OBJECT
	private:
		//Ui::LiteMDClass ui;				//Íå èñïîëüçóåòñÿ
		QToolBar* quick_tb;				//Тулбар для кнопок-ярлыков
		QMenu* mHelp;					//Ìåíþ ïîìîùè
		QMenu* mFile;					//Ìåíþ ôàéëà
		QMenu* mEdit;					//Меню "Правка"
		QMenu* mSettings;				//Ìåíþ íàñòðîåê
		mdEditor* mde;					//Ðåäàêòîð
		mdScreen* mds;					//Ðåíäåð òåêñòà
		DownloaderGui* dwModule;		//Ìîäóëü çàãðóç÷èêà
		QString defTitle;				//Êîíòåéíåð äåôîëòíîãî çàãîëîâêà
		QLabel* workProgressCap;		//Çàòû÷êà íà íåäîäåëàííûå ýëåìåíòû
		appSettings* mdlSet;			//Äèàëîã íàñòðîåê ïðèëîæåíèÿ
		QGroupBox* editorWindow;		//Îêíî ðåäàêòîðà
		QGroupBox* viewerWindow;		//Îêíî ðåíäåðà

		//Действия всякие, в меню и в док
		QAction* actAbout;				//Ïóíêò ìåíþ î ïðîãðàììå
		QAction* actOpen;				//Ïóíêò ìåíþ îòêðûòü
		QAction* actSave;				//Ïóíêò ìåíþ ñîõðàíèòü
		QAction* actSaveAs;				//Ïóíêò ìåíþ ñîõðàíèòü êàê
		QAction* actQuit;				//Ïóíêò ìåíþ âûõîä
		QAction* actDownloader;			//Ïóíêò âûçîâà çàãðóç÷èêà
		QAction* actSet;				//Ïóíêò âûçîâà íàñòðîåê
		QAction* actNew;				//Ïóíêò ñîçäðàíèÿ íîâîãî äîêóìåíòà
		QAction* actPlaceUrl;			//Преобразовать в (ссылку)
		QAction* actPlaceAltUrl;		//Преобразовать в [альтернативную](ссылку)
		QAction* actSetTextFormat;		//Выбрать форматирование выделенного текста
		QAction* actHelp;
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		void closeEvent(QCloseEvent*);		//Ïåðåõâàò÷èê çàêðûòèÿ ïðèëîæåíèÿ
		LiteMD(QWidget *parent = nullptr);	//Áàçîâûé êîíñòðóêîð
		~LiteMD();
	signals:
		void saveFile();	//Ñîõðàíåíèå ôàéëà
	private slots:
		void slotAbout();	//Ñëîò äëÿ âûçîâà îêíà î ïðîãðàììå
		void httpModuleShow();	//Ñèãíàë äëÿ âûçîâà ìîäóëÿ çàãðóç÷èêà
		void slot_mbar_send_string(const QString&);	//Ñëîò äëÿ ñìåíû çàãîëîâêà
	public slots:
		void slotTitleChanged(const QString&);	//Ñëîò äëÿ èçìåíåíèÿ çàãîëîâêà
		void slotFileEdited();	//Ñëîò äëÿ óñòàíîâêè ôëàãà "*"
		void slotTitleReset();	//Ñëîò äëÿ ñáðîñà çàãîëîâêà ê äåôîëòíîìó
};
