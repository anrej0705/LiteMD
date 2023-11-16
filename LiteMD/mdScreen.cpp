#include <QtWidgets>
#include "mdScreen.h"
mdScreen::mdScreen(QWidget* scrWgt) :QLabel(scrWgt)
{
	setText("test");
}
//Простой слот - принимает сигнал и изменяет виджет
void mdScreen::slotSetText(const QString& str)
{
	this->setText(str);
}
