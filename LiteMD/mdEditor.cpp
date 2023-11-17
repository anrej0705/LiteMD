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
void mdEditor::slotOpen()
{
	QString filename = QFileDialog::getOpenFileName();
	if (filename.isEmpty())
		return;
	QFile fileObject(filename);
	if (fileObject.open(QIODevice::ReadOnly))
	{
		QTextStream mdStream(&fileObject);
		setPlainText(mdStream.readAll());
		fileObject.close();
		mdFileName = filename;
		emit titleChanged(mdFileName);
	}
}
void mdEditor::slotSave()
{
	if (mdFileName.isEmpty())
	{
		slotSaveAs();
		return;
	}
	QFile saveObject(mdFileName);
	if (saveObject.open(QIODevice::WriteOnly))
	{
		QTextStream(&saveObject) << toPlainText();
		saveObject.close();
		emit titleChanged(mdFileName);
	}
}
void mdEditor::slotSaveAs()
{
	QString mdSave = QFileDialog::getSaveFileName(0, ("Save Markdown"), "Readme", "*.md ;; *.txt");
	if (!mdSave.isEmpty())
	{
		mdFileName = mdSave;
		slotSave();
	}
}
