#pragma once
#include <QtWidgets>
#include "ui_update_event.h"
#include "mdScreen.h"
#include "xmlWriter.h"

class currentChangelog : public QDialog
{
	Q_OBJECT
	private:
		QVBoxLayout* compositionManager;
		QVBoxLayout* renderComposeManager;
		QHBoxLayout* btnLayout;
		QGroupBox* renderFrame;
		mdScreen* render;
		QPushButton* dismissButton;
		xmlWriter* xmlW;
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		currentChangelog(QWidget* qwgt = 0);
	public slots:
		void slotShowWindow();
		void slotHideWindow();
};
