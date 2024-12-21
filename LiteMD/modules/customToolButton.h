#pragma once
#ifndef CUSTOMTOOLBUTTON_H
#define CUSTOMTOOLBUTTON_H

#include <qtoolbutton.h>

class CustomToolButton : public QToolButton
{
	Q_OBJECT
	public:
		explicit CustomToolButton(QWidget* qwt = 0);
};

#endif // ! CUSTOMTOOLBUTTON_H
