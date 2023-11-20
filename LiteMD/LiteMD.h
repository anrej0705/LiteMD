#pragma once

#include <QtWidgets>
#include "ui_LiteMD.h"
#include "mdEditor.h"
#include "mdScreen.h"

class LiteMD : public QMainWindow
{
    Q_OBJECT
	private:
		Ui::LiteMDClass ui;
		QMenu* mHelp;
		QMenu* mFile;
		QMenu* mSettings;
		mdEditor* mde;
		mdScreen* mds;
	public:
		LiteMD(QWidget *parent = nullptr);
		~LiteMD();
	private slots:
		void slotAbout();
		void slot_mbar_send_string(const QString&);
	public slots:
		void slotTitleChanged(QString&);
};
