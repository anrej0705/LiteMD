#include "customToolButton.h"
#include <qerrormessage.h>

CustomToolButton::CustomToolButton(QWidget* qwgt) : QToolButton(qwgt)
{
	setPopupMode(QToolButton::MenuButtonPopup);
	if (!connect(this, SIGNAL(triggered(QAction*)), this, SLOT(setDefaultAction(QAction*))))
		QErrorMessage::qtHandler();
}
