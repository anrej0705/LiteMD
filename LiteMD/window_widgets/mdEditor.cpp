#include <QtWidgets>
#include "mdEditor.h"
#include "dialogBoxes.h"
#include <boost/container/string.hpp>
#include "logger_backend.h"
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

	qApp->installEventFilter(new mdEditor_filter(this));
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
		appTitleUpdated = 0;
		emit resetTitle();
	}
}
//Открытие файла
void mdEditor::slotOpen()
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	log_stroke->append("[mdEditor]Открываю файл ");

	//Если файл был изменём(проверяем по флагу "*") то предлагаем сохранить
	if (appTitleUpdated)
		if (confirmSave())
			slotSave();
	//Вызываем диалоговое окно открытия
	mdFileName = QFileDialog::getOpenFileName(0, tr("Open Text/Markdown"), "", tr("*.md ;; *.txt"));
	if (mdFileName.isEmpty())
		return;
	log_stroke->append(mdFileName.toUtf8());
	push_log(log_stroke->c_str());
	//Присваиваем имя файла к обработчику который будет открывать его
	mdObject.setFileName(mdFileName);
	//Если размер больше 64 килобайт то файл не откроется
	if (mdObject.size() > MAX_FILESIZE)
	{
		QMessageBox::warning(this,tr("Oversize detected"), tr("Cannot open file because size of this is over ") + QString::number(MAX_FILESIZE) + tr(" bytes"));
		return;
	}
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

	delete(log_stroke);
}
//Сохранение файла
void mdEditor::slotSave()
{
	//Если пользователь ничего не ввёл то отменяем процесс и сбрасываем флаги
	/*if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		return;
	}*/
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
}
//Сохранить как
void mdEditor::slotSaveAs()
{
	//Если пусто то выходим
	/*if (this->toPlainText() == "")
	{
		fileChangedState = 0;
		return;
	}*/
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
	//Если что-то открыто то закрываем
	if (mdObject.isOpen())
		mdObject.close();
	//Сбрасываем имя файла
	mdFileName = "";
	//Сбрасываем содержимое поля ввода и заголовок
	this->setText("");
	emit resetTitle();
	appTitleUpdated = 0;
}

//Получаем выделенный пользователем текст, преобразуем в <ссылку>
//И возвращаем на место
void mdEditor::convertToUrl()
{
	QString procBuf = this->textCursor().selectedText();
	if (procBuf == "")
		return;	//Если пользователь ничего не выделил - выходим

	//Вставляем тег простой ссылки
	procBuf.insert(0, "<");
	procBuf.insert(procBuf.size(), ">");

	//Заменяем выделенный текст ссылкой
	this->textCursor().removeSelectedText();
	this->textCursor().insertText(procBuf);
	emit textChanged();
}

//Получаем выделенный пользователем текст, преобразуем в [альтернативную]<ссылку>
//И возвращаем на место
void mdEditor::convToAltUrl()
{
	QString procBuf = this->textCursor().selectedText();
	if (procBuf == "")
		return;	//Если пользователь ничего не выделил - выходим

	//Хандлер курсора
	QTextCursor tCursor = this->textCursor();

	//Вставляем тег альтернативной ссылки
	procBuf.insert(0, "(");
	procBuf.insert(procBuf.size(), ")");

	//Вставляем шаблон имени ссылки
	procBuf.insert(0, tr("[TYPE_NAME]"));

	//Получаем позицию конца шаблона
	int bumpEnd = 0;

	//Ищем закрывающую квадратную скобку
	for (int index = 0; index < procBuf.size(); ++index)
		if (procBuf.at(index) == "]")
		{
			bumpEnd = index;
			break;
		}

	//Заменяем выделенный текст ссылкой
	this->textCursor().removeSelectedText();
	this->textCursor().insertText(procBuf);

	//Получаем позицию курсора
	int cursorPosition = this->textCursor().position() - procBuf.size();;

	//Шлём сигнал что текст изменился
	emit textChanged();

	//Ставим указатель на позицию шаблона и затем на позицию конца шаблона
	tCursor.setPosition(cursorPosition + 1);
	tCursor.setPosition(cursorPosition + bumpEnd, QTextCursor::KeepAnchor);

	//Отмечаем текст выделенным
	this->setTextCursor(tCursor);
}

mdEditor_filter::mdEditor_filter(QObject* pobj) : QObject(pobj)
{}
bool mdEditor_filter::eventFilter(QObject* podj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + 33))
	{
		event(p_event);
		return 1;
	}
	//ui_event_filter(p_event);
	return 0;
}