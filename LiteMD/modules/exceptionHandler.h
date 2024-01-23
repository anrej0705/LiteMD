#pragma once
#include <QtWidgets>

class exceptionHandler
{
	public:
		enum exceptType{
			INFO = 0,
			WARNING = 1,
			FATAL = 2
		};
		exceptionHandler(exceptType type);
		exceptionHandler(exceptType type, QString reason);
};
