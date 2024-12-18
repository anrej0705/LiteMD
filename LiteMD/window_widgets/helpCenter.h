#pragma once
#include <QtWidgets>
#include "mdScreen.h"

class helpCenter : public QDialog
{
	Q_OBJECT
	private:
		mdScreen* render;					//Рендер окна справки
		QString helpFileName;				//Имя файла справки
		QGroupBox* renderFrame;
		QVBoxLayout* renderComposeManager;
		QHBoxLayout* btnLayout;
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		helpCenter(QWidget* qwgt = 0);
	public slots:
		void slotShowWindow();
		void slotHideWindow();
};
