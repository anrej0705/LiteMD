#pragma once
#include <QtWidgets>
#include "mdScreen.h"

class currentChangelog : public QDialog
{
	private:
		mdScreen* render;
	public:
		currentChangelog(QWidget* qwgt = 0);
};
