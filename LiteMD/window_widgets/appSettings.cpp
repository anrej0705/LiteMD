#include "appSettings.h"
#include <QtWidgets>
appSettings::appSettings(QWidget* aWgt) : QTabWidget(aWgt)
{
	addTab(new QLabel("Basic", this), "Basic");
	resize(800, 600);
}
