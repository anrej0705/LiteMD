#pragma once
#include <QtWidgets>
class appSettings : public QDialog
{
	Q_OBJECT
	private:
		QWidget* basicSettings;	//Основные настройки
		QWidget* parserSettings;	//Настройки обработки текста
		QTabWidget* settingsLister;	//Менеджер вкладок
		QLabel* workprogress;	//Заглушка
		QPushButton* btnOk;		//Кнопка подтверждения
		QPushButton* btnCancel;	//Кнопка отмены
		QHBoxLayout* controlBtnLay;	//Менеджеры размещения кнопок снизу
		QVBoxLayout* dialogWindow;	//Менеджер общего размещения элементов
	public:
		appSettings(QWidget* aWgt = 0);
};
