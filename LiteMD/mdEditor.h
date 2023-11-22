#pragma once
#include <QtWidgets>
class mdEditor : public QTextEdit
{
	Q_OBJECT
	private:
		QString mdFileName;
		QRegExp* hyperlink;
	public:
		mdEditor(QWidget* mdWgt = 0);
	signals:
		void textEdited(const QString&);	//Отправляется при изменении текста
		void titleChanged(const QString&);	//Отправлятся при открытии/сохранении файла
		void hyperlinkDetected(int,int);	//Отправляется при обнаружении конструкции http://* и аналогов
		void hyperlinkRemoved(int);
		void statusString(const QString&);	//Сигнал для отправки в строку состояния
	private slots:
		void slotTextChanged();	//Принимает сигнал textChanged() от базового виджета QTextEdited
	public slots:
		void slotOpen();	//Вызывает диалоговое окно открытия нового файла
		void slotSave();	//Вызывает диалоговое окно выбора названия файла для сохранения
		void slotSaveAs();	//Вызывает окно для набора названия файла для сохранения
		void slotNew();		//Очищает поле ввода
};
