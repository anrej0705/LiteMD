#include "frameChangelog.h"
currentChangelog::currentChangelog(QWidget* qwgt) : QDialog(qwgt)
{
	render = new mdScreen(this);
}
