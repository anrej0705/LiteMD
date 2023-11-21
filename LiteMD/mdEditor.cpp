#include <QtWidgets>
#include "mdEditor.h"

#define MAX_FILESIZE 65536

mdEditor::mdEditor(QWidget* mdWgt) : QTextEdit(mdWgt)
{
	setAcceptRichText(0);
	//Соединяем базовый сигнал со слотом который будет формировать сигнал высылки текста
	if (!connect(this, SIGNAL(textChanged()), this, SLOT(slotTextChanged())))
		QErrorMessage::qtHandler();
}
//Слот генерирующий сигнал с текущим текстом в виджете
void mdEditor::slotTextChanged()
{
	int searchIndex = 0;
	//Создаем контейнер, помещаем содержимое и высылаем
	QString textToShow = QString(this->toPlainText());
	emit textEdited(textToShow);
}
void mdEditor::slotOpen()
{
	//Вызываем диалоговое окно открытия
	QString filename = QFileDialog::getOpenFileName(0, tr("Open Text/Markdown"), "", tr("*.md ;; *.txt"));
	if (filename.isEmpty())
		return;
	QFile fileObject(filename);
	//Если размер больше 64 килобайт то файл не откроется
	if (fileObject.size() > MAX_FILESIZE)
	{
		QMessageBox::warning(this,tr("Oversize detected"), tr("Cannot open file because size of this is over ") + QString::number(MAX_FILESIZE) + tr(" bytes"));
		return;
	}
	if (fileObject.open(QIODevice::ReadOnly))
	{
		QTextStream mdStream(&fileObject);
		mdStream.setCodec("UTF-8");
		setPlainText(mdStream.readAll());
		fileObject.close();
		mdFileName = filename;
		emit titleChanged(mdFileName);
		emit statusString(tr("Opened ") + mdFileName);
	}
}
void mdEditor::slotSave()
{
	QByteArray utf8out;
	utf8out.append(toPlainText().toUtf8());
	//Если файл ещё ни разу не сохранялся то переходим в слот "Сохранить как"
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
	//Вызываем диалоговое окно сохранения
	QString mdSave = QFileDialog::getSaveFileName(0, tr("Save Text/Markdown"), "Readme", tr("*.md ;; *.txt"));
	if (!mdSave.isEmpty())
	{
		mdFileName = mdSave;
		slotSave();
		emit statusString(tr("Saved ") + mdSave);
	}
}
