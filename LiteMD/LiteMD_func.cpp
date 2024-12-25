#include "LiteMD.h"
#include "ui_update_event.h"
#include "GuiDownloader.h"
#include "dialogBoxes.h"
#include "exceptionHandler.h"
#include "logger_backend.h"
#include "LastFileManager.h"
#include <QtWidgets>
#include <boost/container/string.hpp>
extern "C"
{
	#include "globalFlags.h"
	#include "global_definitions.h"
}

inline QIcon setAppIcon()
{
	QPixmap appIcon(getAppPath() + "/icon.ico");
	appIcon.setMask(appIcon.createMaskFromColor(QColor(0, 0, 0)));
	return appIcon;
}

// Инициализирует список последних элементов.
void LiteMD::initLastFileMenu()
{
	// Получение списка файлов.
	LastFileManager lastFileManager;
	const std::deque<std::string>& lastFilePaths = lastFileManager.getFiles();

	QIcon ico;

	// Если список пустой или первый элемент пустой - завершить работу.
	if (lastFilePaths.empty() || lastFilePaths.front().empty())
		return;

	mFile->addSeparator();

	// Добавление меню.
	std::for_each(
		std::begin(lastFilePaths),
		std::end(lastFilePaths),
		[=](std::string lastFilePath) {
			QFileInfo fileInfo(lastFilePath.c_str());					//Объект, содержащий информацию о файле(прим. anrej0705)
			QAction* openLastfile = new QAction(fileInfo.fileName());	//Создаём действие, которое будет помещено в меню(прим. anrej0705)

			openLastfile->setData(fileInfo.filePath());					//Сохраняем путь до файла в QVariant(прим. anrej0705)

			openLastfile->setIcon(setAppIcon());						//Задаём иконку

			recentFiles->addAction(openLastfile);						//Добавляем действие в меню

			if (!connect(
				openLastfile,
				&QAction::triggered,
				mde,
				[=] { mde->slotOpen(fileInfo.filePath()); }))
				QErrorMessage::qtHandler();	//Соединяем сигнал со слотом вызова окна о программе
		}
	);
}
//О программе
void LiteMD::slotAbout()
{
	QMessageBox::about(this, "LiteMD", tr("Ver ") + APP_STAGE + APP_VERSION + (" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))
		+ tr("<BR>By anrej0705<BR>See me at Github:") + "<BR><A HREF=\"github.com/anrej0705\">github.com/anrej0705</A><BR><BR>"
		+ tr("This app is free for use,modify and reupload<BR><BR>LiteMD IS FREE SOFTWARE! IF YOU PAID FOR IT YOU HAVE BEEN SCAMMED!")
		+ "<BR>" + "<BR>| Qt 5.14.2 | C++17 | C11 | Boost 1.84.00 |<BR>" + "<BR>"
		+ tr("Github repo: ") + "<A HREF=\"https://github.com/anrej0705/LiteMD\">https://github.com/anrej0705/LiteMD</A><BR>"
		+ tr("Releases: ") + "<A HREF=\"https://github.com/anrej0705/LiteMD/releases\">https://github.com/anrej0705/LiteMD/releases</A>");
}
//Слот редактирования заголовка(добавления файла к концу)
void LiteMD::slotTitleChanged(const QString& title)
{
	//Контейнеры для помещения элементов заголовка
	std::string newTitle/* = defTitle.toStdString()*/;	//Патч 0.2.2 исправление заголовка
	fileFullPath = title.toStdString();
	if (title.isEmpty())
		fileFullPath = QString(tr("Untitled")).toStdString();
	//Формируем заголовок из контейнеров и устанавливаем в приложение
	newTitle.append((tr("LiteMD") + APP_STAGE + APP_VERSION + tr(" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))).toStdString() + " [" + fileFullPath.substr(fileFullPath.rfind('/') + 1, fileFullPath.size() - fileFullPath.rfind('/')) + "]");
	//defTitle = QString::fromStdString(newTitle);
	setWindowTitle(QString::fromStdString(newTitle));
}
//Сброс заголовка
void LiteMD::slotTitleReset()
{
	setWindowTitle(defTitle);
}
//Слот показа состояния на панели снизу
void LiteMD::slot_mbar_send_string(const QString& str)
{
	statusBar()->showMessage(str, 4000);
}
//Слот установки флага редактирования флага
void LiteMD::slotFileEdited()
{
	if (!appTitleUpdated)
	{
		QString title = windowTitle();
		title.insert(0, '*');
		setWindowTitle(title);
		appTitleUpdated = 1;
	}
}
//Перехватчик события закрытия
void LiteMD::closeEvent(QCloseEvent* ce)
{
	push_log("[QT]Вызвано событие закрытия приложения");
	//Если файл редактировался то спрашиваем нужно ли сохранить
	if (fileChangedState)
	{
		if (!confirmSave())
		{
			//Если пользователь не захотел сохранять то просто закрываемся
			appClose = 1;
			dwModule->close();
			ce->accept();
		}
		else
		{
			//Если пользователь захотел сохранить то закрывамся после сохранения
			appClose = 1;
			emit saveFile();
			dwModule->close();
			ce->accept();
		}
	}
	//Выставляем флаг закрытия и закрываем модуль если он открыт
	appClose = 1;
	dwModule->close();
}

//Вызов модуля загрузки
void LiteMD::httpModuleShow()
{
	//Если по указателю есть то что нам надо то показываем
	if (dynamic_cast<DownloaderGui*>(dwModule))
	{
		dwModule->slotShow();
		return;
	}
	//Иначе создаем объект и показываем
	dwModule = new DownloaderGui;
	dwModule->slotShow();
}
LiteMD::~LiteMD()
{
	//0.2.7 Позже поработаю здесь
	//free(chosenTheme);
	//deleteOnExit();
}

void LiteMD::slotCheckUpdates()
{
	throw(exceptionHandler(exceptionHandler::WARNING), "patch");	//0.3.7
}

void LiteMD::slotFileClose()
{
	//Закрываем файл и чистим текст
	mde->closeFile();
	fileFullPath.clear();

	//Сбрасываем заголовок
	setWindowTitle(defTitle);
}

//Прокрутка вниз через полосу прокрутки
void LiteMD::slotMdsDown()
{
	if (!managePrior)
	{
		//Определяем приоритет
		if (mdsArea->verticalScrollBar()->size().height() > mde->verticalScrollBar()->size().height())
		{
			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsArea->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() + mdsArea->verticalScrollBar()->size().height());
			mde->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() + mdsArea->verticalScrollBar()->size().height());
		}
		else
		{
			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsArea->verticalScrollBar()->setValue(mde->verticalScrollBar()->value() + mde->verticalScrollBar()->size().height());
			mde->verticalScrollBar()->setValue(mde->verticalScrollBar()->value() + mde->verticalScrollBar()->size().height());
		}
	}
	else
	{

	}
}

//Прокрутка вверх через полосу прокрутки
void LiteMD::slotMdsUp()
{
	float proport;		//Пропорции, по которым рассчитывается перемещение в окне
	float mdeHeight = mde->verticalScrollBar()->value();	//0 - кританёт делением на 0
	float mdsHeight = mdsArea->verticalScrollBar()->value();
	int mdsRes = 0;
	int mdeRes = 0;
	if (!managePrior)
	{
		//Определяем приоритет
		if (mdsHeight > mdeHeight)
		{
			proport = mdsHeight / mdeHeight;
			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsRes = mdsArea->verticalScrollBar()->value() * proport;
			mdsArea->verticalScrollBar()->setValue((mdsArea->verticalScrollBar()->value() * proport) - mdsArea->verticalScrollBar()->size().height());
			mde->verticalScrollBar()->setValue((mdsArea->verticalScrollBar()->value() * proport) - mdsArea->verticalScrollBar()->size().height());
		}
		else if(mdeHeight > mdsHeight)
		{
			proport = mdeHeight / mdsHeight;
			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsArea->verticalScrollBar()->setValue((mde->verticalScrollBar()->value() * proport) - mde->verticalScrollBar()->size().height());
			mde->verticalScrollBar()->setValue((mde->verticalScrollBar()->value() * proport) - mde->verticalScrollBar()->size().height());
		}
	}
	else
	{

	}
}

void LiteMD::slotSwitchDir()
{
	if (scrollPrior)			//Приоритет - редактор
	{
		scrollPrior = 0;
		dirSwitch1->setText("<<");
		dirSwitch2->setText("<<");
		for (uint8_t it = 0; it < 12; ++it)
			hintsList[it]->setText("<<");
		return;
	}
	else if (!scrollPrior)
	{
		scrollPrior = 1;		//Приоритет - монитор
		dirSwitch1->setText(">>");
		dirSwitch2->setText(">>");
		for (uint8_t it = 0; it < 12; ++it)
			hintsList[it]->setText(">>");
		return;
	}
}

void LiteMD::slotManageDir()
{
	if (managePrior)			//Режим - авто
	{
		managePrior = 0;
		manageDir->setText(tr("manageDirAuto"));
		dirSwitch1->setText("<>");
		dirSwitch2->setText("<>");
		dirSwitch1->setEnabled(0);
		dirSwitch2->setEnabled(0);
		if (mdsArea->size().height() > mde->verticalScrollBar()->size().height())
			scrollPrior = 0;	//Авто - приоритет - редактор
		else if (mdsArea->size().height() < mde->verticalScrollBar()->size().height())
			scrollPrior = 1;	//Авто - приоритет монитор
		return;
	}
	else if (!managePrior)		//Режим - ручной
	{
		managePrior = 1;
		manageDir->setText(tr("manageDirManual"));
		dirSwitch1->setEnabled(1);
		dirSwitch2->setEnabled(1);
		if (scrollPrior) //Предустанки кнопки направления
		{
			dirSwitch1->setText(">>");
			dirSwitch2->setText(">>");
			for (uint8_t it = 0; it < 12; ++it)
				hintsList[it]->setText(">>");
		}
		else if (!scrollPrior)
		{
			dirSwitch1->setText("<<");
			dirSwitch2->setText("<<");
			for (uint8_t it = 0; it < 12; ++it)
				hintsList[it]->setText("<<");
		}
		return;
	}
}

void LiteMD::slotSwitchSync()
{
	if (syncCtl)		//Авто
	{
		syncCtl = 0;
		syncCtlBtn->setText("==");
		return;
	}
	else if (!syncCtl)	//Ручной
	{
		syncCtl = 1;
		syncCtlBtn->setText("=|=");
		return;
	}
}

void LiteMD::slotScrollEvent(int scroll)
{
	if (scrollPrior && !syncCtl)			//Приоритет - редактор
	{
		mdsArea->verticalScrollBar()->setValue(scroll);
		return;
	}
	else if (!scrollPrior && !syncCtl)		//Приоритет монитор
	{
		mde->verticalScrollBar()->setValue(scroll);
		return;
	}
}
