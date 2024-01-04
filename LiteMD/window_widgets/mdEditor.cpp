#include <QtWidgets>
#include "mdEditor.h"
#include "dialogBoxes.h"
#include "..\LastFileManager.h"

extern "C"
{
	#include "globalFlags.h"
}

constexpr auto maxfileSize{ 65536 };

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
	//Если заголовок окна не содержит флага "*" то выставляем
	if (!appTitleUpdated)
		emit changeTitle();
	//Поднимаем флаг сигнализации о изменениий файла
	if (!fileChangedState)
		fileChangedState = 1;
	//Пересылаем текст в массив отрисовки
	emit textEdited(textToShow);
	//Если пользователь всё стёр то снимаем флаг
	if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		emit resetTitle();
	}
}
//Открытие файла
void mdEditor::slotOpen()
{
	// Вызываем диалоговое окно открытия.
	QString fileName = QFileDialog::getOpenFileName(0, tr("Open Text/Markdown"), "", tr("*.md ;; *.txt"));

	if (fileName.isEmpty())
		return;

	slotOpen(fileName);
}
//Открытие файла
void mdEditor::slotOpen(QString fileName)
{
	// Если файл был изменём(проверяем по флагу "*") то предлагаем сохранить.
	if (appTitleUpdated && confirmSave() == true)
		slotSave();

	//Присваиваем имя файла к обработчику который будет открывать его
	mdObject.setFileName(fileName);
	//Если размер больше 64 килобайт то файл не откроется
	if (mdObject.size() > maxfileSize)
	{
		QMessageBox::warning(this,tr("Oversize detected"), tr("Cannot open file because size of this is over ") + QString::number(maxfileSize) + tr(" bytes"));
		return;
	}
	mdFileName = fileName;
	//Если удалось то открыть то начинаем чтение
	if (mdObject.open(QIODevice::ReadOnly))
	{
		//Читаем в поток
		QTextStream mdStream(&mdObject);
		mdStream.setCodec("UTF-8");
		//Читаем из потока в поле редактирования(дальше сигнал отсылает содержимое в поле рендера)
		setPlainText(mdStream.readAll());
		//Закрываем файл и освобождаем его дескриптор
		mdObject.close();
		//Отправляем сигналы
		emit titleChanged(mdFileName);
		emit statusString(tr("Opened ") + mdFileName);
	}
	//Сбрасываем флаги
	fileChangedState = 0;
	appTitleUpdated = 0;
	// Добавление файла в список последних.
	LastFileManager lastFileManager("settings\\last_files");
	lastFileManager.addFile(mdFileName.toStdString());
	lastFileManager.save();
}
//Сохранение файла
void mdEditor::slotSave()
{
	//Если пользователь ничего не ввёл то отменяем процесс и сбрасываем флаги
	if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		return;
	}
	//Создаем поток для вывода и добавляем туда текст преобразованный в юникод
	QByteArray utf8out;
	utf8out.append(toPlainText().toUtf8());
	//Если файл ещё ни разу не сохранялся то переходим в слот "Сохранить как"
	if (mdFileName.isEmpty())
	{
		slotSaveAs();
		appTitleUpdated = 0;
		return;
	}
	//Присваиваем хандлеру имя файла
	mdObject.setFileName(mdFileName);
	//Если удалось открыть файл на запись то выполняем
	if (mdObject.open(QIODevice::WriteOnly))
	{
		//Присваиваем выходному потоку указатель на хандлер и задаем юникод и затем сохраняем
		QTextStream out(&mdObject);
		out.setCodec("UTF-8");
		out << toPlainText();
		//Закрываем файл, сбрасываем файл и отсылаем сигнал
		mdObject.close();
		fileChangedState = 0;
		emit titleChanged(mdFileName);
	}
	//Сбрасываем флаг даже если не удалось записать
	appTitleUpdated = 0;
	// Добавление файла в список последних.
	LastFileManager lastFileManager("settings\\last_files");
	lastFileManager.addFile(mdFileName.toStdString());
	lastFileManager.save();
}
//Сохранить как
void mdEditor::slotSaveAs()
{
	//Если пусто то выходим
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
	//Сбрасываем флаг в любом случае
	appTitleUpdated = 0;
	// Добавление файла в список последних.
	LastFileManager lastFileManager("settings\\last_files");
	lastFileManager.addFile(mdFileName.toStdString());
	lastFileManager.save();
}
//Новый документ
void mdEditor::slotNew()
{
	//Если пустой текст то скипаем
	if (appTitleUpdated&&this->toPlainText()!="")
	{
		//Иначе сохраняем
		if (confirmSave())
			slotSave();
	}
	//Сбрасываем флаги в любом случае
	fileOpenedState = 0;
	fileChangedState = 0;
	appTitleUpdated = 0;
	//Если что-то открыто то закрываем
	if (mdObject.isOpen())
		mdObject.close();
	//Сбрасываем имя файла
	mdFileName = "";
	//Сбрасываем содержимое поля ввода и заголовок
	this->setText("");
	emit resetTitle();
}
