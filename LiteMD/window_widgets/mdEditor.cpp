#include <QtWidgets>
#include "mdEditor.h"
#include "dialogBoxes.h"
extern "C"
{
	#include "globalFlags.h"
}

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
	if (!appTitleUpdated)
		emit changeTitle();
	if (!fileChangedState)
		fileChangedState = 1;
	emit textEdited(textToShow);
	if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		emit resetTitle();
	}
}
void mdEditor::slotOpen()
{
	bool save_accept = 0;
	if (appTitleUpdated)
	{
		save_accept = confirmSave();
		if (save_accept)
			slotSave();
	}
	//Вызываем диалоговое окно открытия
	mdFileName = QFileDialog::getOpenFileName(0, tr("Open Text/Markdown"), "", tr("*.md ;; *.txt"));
	if (mdFileName.isEmpty())
		return;
	mdObject.setFileName(mdFileName);
	//Если размер больше 64 килобайт то файл не откроется
	if (mdObject.size() > MAX_FILESIZE)
	{
		QMessageBox::warning(this,tr("Oversize detected"), tr("Cannot open file because size of this is over ") + QString::number(MAX_FILESIZE) + tr(" bytes"));
		return;
	}
	if (mdObject.open(QIODevice::ReadOnly))
	{
		QTextStream mdStream(&mdObject);
		mdStream.setCodec("UTF-8");
		setPlainText(mdStream.readAll());
		mdObject.close();
		//mdFileName = filename;
		emit titleChanged(mdFileName);
		emit statusString(tr("Opened ") + mdFileName);
	}
	fileChangedState = 0;
	appTitleUpdated = 0;
}
void mdEditor::slotSave()
{
	if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		return;
	}
	QByteArray utf8out;
	utf8out.append(toPlainText().toUtf8());
	//Если файл ещё ни разу не сохранялся то переходим в слот "Сохранить как"
	if (mdFileName.isEmpty())
	{
		slotSaveAs();
		appTitleUpdated = 0;
		return;
	}
	mdObject.setFileName(mdFileName);
	if (mdObject.open(QIODevice::WriteOnly))
	{
		QTextStream out(&mdObject);
		out.setCodec("UTF-8");
		out << toPlainText();
		mdObject.close();
		fileChangedState = 0;
		emit titleChanged(mdFileName);
	}
	appTitleUpdated = 0;
}
void mdEditor::slotSaveAs()
{
	if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		return;
	}
	//Вызываем диалоговое окно сохранения
	mdFileName = QFileDialog::getSaveFileName(0, tr("Save Text/Markdown"), "Readme", tr("*.md ;; *.txt"));
	if (!mdFileName.isEmpty())
	{
		//mdFileName = mdSave;
		slotSave();
		emit statusString(tr("Saved ") + mdFileName);
	}
	appTitleUpdated = 0;
}
void mdEditor::slotNew()
{
	bool save_accept = 0;
	if (appTitleUpdated&&this->toPlainText()!="")
	{
		save_accept = confirmSave();
		if (save_accept)
			slotSave();
	}
	fileOpenedState = 0;
	fileChangedState = 0;
	appTitleUpdated = 0;
	if (mdObject.isOpen())
		mdObject.close();
	mdFileName = "";
	this->setText("");
	appTitleUpdated = 0;
	emit resetTitle();
}
