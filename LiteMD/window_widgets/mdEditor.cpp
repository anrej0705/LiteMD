#include <QtWidgets>
#include "mdEditor.h"
#include "dialogBoxes.h"
#include <boost/container/string.hpp>
#include "logger_backend.h"
#include "LastFileManager.h"
extern "C"
{
	#include "globalFlags.h"
}

#define MAX_FILESIZE 4294967296	//Лимит 4Гб

mdEditor::mdEditor(QWidget* mdWgt) : QTextEdit(mdWgt)
{
	push_log("[QT]Инициализация окна редактора");
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
//Открытие файла полученного как аргумент
bool mdEditor::openFileArg(char* arg)
{	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	//ПроцеДУРА аналогична слоту открытия только здесь сразу передаётся путь до файла

	log_stroke->append("[mdEditor]Открываю файл ");

	mdFileName = QString::fromLocal8Bit(arg);
	//Отправляем в лог что собираемся открыть такой-то файл
	log_stroke->append(mdFileName.toUtf8());
	push_log(log_stroke->c_str());
	//Присваиваем имя файла к обработчику который будет открывать его
	mdObject.setFileName(mdFileName);
	//Если размер больше 4 гигабайт то файл не откроется
	if (mdObject.size() > MAX_FILESIZE)
	{
		QMessageBox::warning(this, tr("Oversize detected"), tr("Cannot open file because size of this is over ") + QString::number(MAX_FILESIZE) + tr(" bytes"));
		return 0;
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
	return 1;
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
	//Отправляем в лог что собираемся открыть такой-то файл
	log_stroke->append(mdFileName.toUtf8());
	push_log(log_stroke->c_str());
	//Присваиваем имя файла к обработчику который будет открывать его
	mdObject.setFileName(mdFileName);
	//Если размер больше 4 гигабайт то файл не откроется
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

	//Сохраняем файл в списке недавних
	saveLastFile();

	//Сбрасываем флаги
	fileChangedState = 0;
	appTitleUpdated = 0;

	delete(log_stroke);
}
//Открывает файл по пути (Временное говно т.к. сильно дублирует mdEditor::slotOpen()).
void mdEditor::slotOpen(const QString& mdFileName)
{
	//Контейнер для строчки лога перед отправкой в ядро
	boost::container::string* log_stroke = new boost::container::string;

	log_stroke->append("[mdEditor]Открываю файл из списка последних");

	//Отправляем в лог что собираемся открыть такой-то файл
	log_stroke->append(mdFileName.toUtf8());
	push_log(log_stroke->c_str());
	//Присваиваем имя файла к обработчику который будет открывать его
	mdObject.setFileName(mdFileName);
	//Если размер больше 4 гигабайт то файл не откроется
	if (mdObject.size() > MAX_FILESIZE)
	{
		QMessageBox::warning(this, tr("Oversize detected"), tr("Cannot open file because size of this is over ") + QString::number(MAX_FILESIZE) + tr(" bytes"));
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

	//Сохраняем файл в списке недавних
	saveLastFile();

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
	// Добавление файла в список последних.
	saveLastFile();
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
	// Добавление файла в список последних.
	saveLastFile();
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
	const QClipboard* clbd = QApplication::clipboard();
	const QMimeData* mmd = clbd->mimeData();

	QString procBuf = this->textCursor().selectedText();

	//Если юзер держит ссылку в буфере обмена(скопировал), то вставляем
	if (procBuf == "" && mmd->hasText())
	{
		//Вставляем тег простой ссылки
		procBuf.insert(0, "<");
		procBuf.insert(1, mmd->text());
		procBuf.insert(procBuf.size(), ">");
	}
	else//Иначе идём по старинке
	{
		if (procBuf == "")
			return;	//Если пользователь ничего не выделил - выходим

		//Вставляем тег простой ссылки
		procBuf.insert(0, "<");
		procBuf.insert(procBuf.size(), ">");
	}

	//Заменяем выделенный текст ссылкой
	this->textCursor().removeSelectedText();
	this->textCursor().insertText(procBuf);
	emit textChanged();
	emit titleChanged(mdFileName);
}

//Получаем выделенный пользователем текст, преобразуем в [альтернативную]<ссылку>
//И возвращаем на место
void mdEditor::convToAltUrl()
{
	const QClipboard* clbd = QApplication::clipboard();
	const QMimeData* mmd = clbd->mimeData();

	QString procBuf = this->textCursor().selectedText();

	//Если юзер держит ссылку в буфере обмена(скопировал), то вставляем

	if (procBuf == "" && mmd->hasText())
	{
		procBuf.insert(0, "[");
		procBuf.insert(procBuf.size(), tr("TYPE_NAME"));
		procBuf.insert(procBuf.size(), "]");
		procBuf.insert(procBuf.size(), "(");
		procBuf.insert(procBuf.size(), mmd->text());
		procBuf.insert(procBuf.size(), ")");
	}
	else
	{
		if (procBuf == "")
			return;	//Если пользователь ничего не выделил - выходим

		//Вставляем тег альтернативной ссылки
		procBuf.insert(0, "[");
		procBuf.insert(procBuf.size(), "]");

		//Вставляем шаблон имени ссылки
		procBuf.insert(procBuf.size(), "(" + tr("TYPE_NAME") + ")");
	}
	//Хандлер курсора
	QTextCursor tCursor = this->textCursor();

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
	emit textChanged();
	emit titleChanged(mdFileName);
}
//Вставляет '#' слева от курсора
void mdEditor::slotSetH1(){this->insertLattice(1);}
//Вставляет '##' слева от курсора
void mdEditor::slotSetH2(){this->insertLattice(2);}
//Вставляет '###' слева от курсора
void mdEditor::slotSetH3(){this->insertLattice(3);}
//Вставляет '####' слева от курсора
void mdEditor::slotSetH4(){this->insertLattice(4);}
//Вставляет '#####' слева от курсора
void mdEditor::slotSetH5(){this->insertLattice(5);}
//Вставляет '/' слева от курсора
void mdEditor::slotSetEscape()
{
	//Хандлер курсора
	QTextCursor tCursor = this->textCursor();

	//Получаем позицию курсора
	int cursorPosition = this->textCursor().position();

	//Вставляем символ экранирования
	this->insertPlainText("\\");
}
//Вставляет ** слева и справа от выделенной области
void mdEditor::slotSetBold()
{
	QString procBuf = this->textCursor().selectedText();
	if (procBuf == "")
		return;	//Если пользователь ничего не выделил - выходим

	//Вставляем тег жирного текста по обоим концам выделенной области
	procBuf.insert(0, "**");
	procBuf.insert(procBuf.size(), "**");

	//Заменяем выделенный текст ссылкой
	this->textCursor().removeSelectedText();
	this->textCursor().insertText(procBuf);

	//Получаем позицию курсора
	int cursorPosition = this->textCursor().position() - procBuf.size();
	
	//Шлём смску что текст изменился
	emit textChanged();
}
//Вставляет * слева и справа от выделенной области
void mdEditor::slotSetItalic()
{
	QString procBuf = this->textCursor().selectedText();
	if (procBuf == "")
		return;	//Если пользователь ничего не выделил - выходим

	//Вставляем тег курсива по обоим концам выделенной области
	procBuf.insert(0, "*");
	procBuf.insert(procBuf.size(), "*");

	//Заменяем выделенный текст ссылкой
	this->textCursor().removeSelectedText();
	this->textCursor().insertText(procBuf);

	//Получаем позицию курсора
	int cursorPosition = this->textCursor().position() - procBuf.size();

	//Шлём смску что текст изменился
	emit textChanged();
}
//Вставляет подчёркивание в текст
void mdEditor::slotSetUnrderline()
{
	QString procBuf = this->textCursor().selectedText();
	if (procBuf == "")
		return;	//Если пользователь ничего не выделил - выходим

	//Вставляем подчёркивание
	procBuf.insert(0, "<ins>");
	procBuf.insert(procBuf.size(), "</ins>");

	//Заменяем выделенный текст ссылкой
	this->textCursor().removeSelectedText();
	this->textCursor().insertText(procBuf);

	//Получаем позицию курсора
	int cursorPosition = this->textCursor().position() - procBuf.size();

	//Шлём смску что текст изменился
	emit textChanged();
}
//Отмечает тегом зачёркнутого текста
void mdEditor::slotSetStrikethrough()
{
	QString procBuf = this->textCursor().selectedText();
	if (procBuf == "")
		return;	//Если пользователь ничего не выделил - выходим

	//Вставляем тег зачёркнутого текста
	procBuf.insert(0, "~~");
	procBuf.insert(procBuf.size(), "~~");

	//Заменяем выделенный текст ссылкой
	this->textCursor().removeSelectedText();
	this->textCursor().insertText(procBuf);

	//Получаем позицию курсора
	int cursorPosition = this->textCursor().position() - procBuf.size();

	//Шлём смску что текст изменился
	emit textChanged();
}
//Вставляет литеру решётки в количестве заданным из аргумента
void mdEditor::insertLattice(uint8_t count)
{
	//Хандлер курсора
	QTextCursor tCursor = this->textCursor();

	//Получаем позицию курсора
	int cursorPosition = this->textCursor().position();

	//Сюда будет собираться заголовок нужного размера
	std::string _piece;

	//Вставляем символ символ решётки столько раз сколько выбрал юзер
	for (uint8_t _idx = 0; _idx < count; ++_idx)
	{
		_piece.append("#");
	}

	//Вставляем пробел как того требует стандарт
	_piece.append(" ");

	//Теперь просто вставляем
	this->insertPlainText(_piece.c_str());

	//Шлём смску что текст изменился
	emit textChanged();
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

//Слот, но как бы и не слот
//Закрывает файл если он открыт и сбрасывает его имя и флаги
void mdEditor::closeFile()
{
	if (mdObject.isOpen())
		mdObject.close();
	mdFileName.clear();
	fileOpenedState = 0;
	fileChangedState = 0;

	//Чистим поле ввода
	this->clear();

	//Шлём смску что текст изменился(очистился)
	emit textChanged();
}

//Сохраняем файл в списке недавних
void mdEditor::saveLastFile()
{
	LastFileManager lastFileManager("settings\\last_files", 5);
	lastFileManager.addFile(std::string(mdFileName.toUtf8()));
	lastFileManager.save();
}