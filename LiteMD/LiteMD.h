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
		mdEditor* mde;
		mdScreen* mds;
	public:
		LiteMD(QWidget *parent = nullptr);
		~LiteMD();
	private slots:
		void slotAbout();
};
