#include <QtWidgets>
#include "mdEditor.h"
mdEditor::mdEditor(QWidget* mdWgt) : QTextEdit(mdWgt)
{
	//Соединяем базовый сигнал со слотом который будет формировать сигнал высылки текста
	if (!connect(this, SIGNAL(textChanged()), this, SLOT(slotTextChanged())))
		QErrorMessage::qtHandler();
}
//Слот генерирующий сигнал с текущим текстом в виджете
void mdEditor::slotTextChanged()
{
	//Создаем контейнер, помещаем содержимое и высылаем
	QString textToShow = QString(this->toPlainText());
	emit textEdited(textToShow);
}
