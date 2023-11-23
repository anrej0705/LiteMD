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
		Ui::LiteMDClass ui;
		QDockWidget* quick_access_dock;
		QMenu* mHelp;
		QMenu* mFile;
		QMenu* mSettings;
		mdEditor* mde;
		mdScreen* mds;
		DownloaderGui* dwModule;
		QString defTitle;
		QLabel* workProgressCap;
		appSettings* mdlSet;
	public:
		void closeEvent(QCloseEvent*);
		LiteMD(QWidget *parent = nullptr);
		~LiteMD();
	signals:
		void saveFile();
	private slots:
		void slotAbout();
		void slot_mbar_send_string(const QString&);
	public slots:
		void slotTitleChanged(const QString&);
		void slotFileEdited();
		void slotTitleReset();
};
