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
	QString filename = QFileDialog::getOpenFileName(0, "Open Text/Markdown", "", "*.md ;; *.txt");
	if (filename.isEmpty())
		return;
	QFile fileObject(filename);
	if (fileObject.open(QIODevice::ReadOnly))
	{
		QTextStream mdStream(&fileObject);
		mdStream.setCodec("UTF-8");
		setPlainText(mdStream.readAll());
		fileObject.close();
		mdFileName = filename;
		emit titleChanged(mdFileName);
	}
}
void mdEditor::slotSave()
{
	QByteArray utf8out;
	utf8out.append(toPlainText().toUtf8());
	if (mdFileName.isEmpty())
	{
		slotSaveAs();
		return;
	}
	QFile saveObject(mdFileName);
	if (saveObject.open(QIODevice::WriteOnly))
	{
		QTextStream out(&saveObject);
		out.setCodec("UTF-8");
		out << toPlainText();
		saveObject.close();
		emit titleChanged(mdFileName);
	}
}
void mdEditor::slotSaveAs()
{
	QString mdSave = QFileDialog::getSaveFileName(0, "Save Text/Markdown", "Readme", "*.md ;; *.txt");
	if (!mdSave.isEmpty())
	{
		mdFileName = mdSave;
		slotSave();
	}
}
