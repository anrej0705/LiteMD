#pragma once
#include <QtWidgets>
class mdEditor : public QTextEdit
{
	Q_OBJECT
	private:
	public:
		mdEditor(QWidget* mdWgt = 0);
	signals:
		void textEdited(const QString&);	//Отправляется при изменении текста
	private slots:
		void slotTextChanged();	//Принимает сигнал textChanged() от базового виджета QTextEdited
};