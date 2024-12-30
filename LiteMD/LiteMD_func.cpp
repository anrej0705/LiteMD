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

QIcon setAppIcon()
{
	//Подгружаем иконку
	QPixmap input(getAppPath() + "/ress/icon_about.png");
	QPixmap appIcon(input.size());	//Создаём объект в памяти по размеру картинки
	appIcon.fill(Qt::transparent);	//Заливаем прозрачным(делаем альфа канал)
	QPainter p(&appIcon);			//Линкуем нашу загруженную иконку
	p.setOpacity(1);				//Врубаем альфа-канал
	p.drawPixmap(0, 0, input);		//Рисуем картинку с альфа-каналом
	p.end();
	return appIcon;
}

// Инициализирует список последних элементов.
void LiteMD::initLastFileMenu()
{
	// Получение списка файлов.
	LastFileManager lastFileManager;
	const std::deque<std::string>& lastFilePaths = lastFileManager.getFiles();

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

			recentFiles->addAction(openLastfile);					//Добавляем действие в меню

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

void LiteMD::slotCheckUpdates()
{
	//Разрешаем проверку и установку обновлений
	enableUpdate = 1;	//0.3.6
	//0.3.7 добавится загрузка JSON с тегами
	//0.3.8 процесс будет полностью автоматизирован
	//exit(0);			//Выходим так как дальше нужно начать процесс обновления
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
	float mdeVal, mdeHgh, mdsVal, mdsHgh, proport = 0.0f;
	int scrollVal = -1;	//Багфикс, не надо лямбды - код должен быть легко читаем даже для нуба
	if (!managePrior)
	{
		//Проверяем длину писюнов выясняя чей длиннее
		if (mde->verticalScrollBar()->maximum() > mdsArea->verticalScrollBar()->maximum())
		{
			//Если окно редактора имеет бОльшую абсолютную высоту, то получает приоритет
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->maximum());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->maximum());

			//Расчёт пропорции ползунка
			proport = mdeHgh / mdsHgh;

			//Расчёт смещения ползунка окна рендера с учётом пропории
			mdsVal = static_cast<float>(mdsArea->verticalScrollBar()->size().height()) / proport;

			//Багфикс - значения нужно складывать в отдельной переменной иначе баганётся
			//Например ползунок на 672, на него приходит +672 и в итоге должно получиться 1344
			//Но получится 1025. Я не исследовал природу бага но нашёл способ его обойти
			scrollVal = mde->verticalScrollBar()->value() + mde->verticalScrollBar()->size().height();

			mdsArea->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() + mdsVal);
			mde->verticalScrollBar()->setValue(scrollVal);
		}
		else if (mde->verticalScrollBar()->maximum() < mdsArea->verticalScrollBar()->maximum())
		{
			//Если окно рендера имеет бОльшую абсолютную высоту, то получает приоритет
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->maximum());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->maximum());

			//Считаем пропорцию от размера окна рендера
			proport = mdsHgh / mdeHgh;

			//Расчёт смещения ползунка окна рендера с учётом пропории
			mdeVal = static_cast<float>(mdsArea->verticalScrollBar()->size().height()) / proport;

			scrollVal = mdsArea->verticalScrollBar()->value() + mdsArea->verticalScrollBar()->size().height();

			//Выполняем смещения для окон
			mdsArea->verticalScrollBar()->setValue(scrollVal);
			mde->verticalScrollBar()->setValue(mde->verticalScrollBar()->value() + mdeVal);
		}
	}
	else
	{
		//Если редактор
		if (scrollPrior)
		{
			//Если окно редактора имеет бОльшую абсолютную высоту, то получает приоритет
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->maximum());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->maximum());

			//Расчёт пропорции ползунка
			proport = mdeHgh / mdsHgh;

			//Расчёт смещения ползунка окна рендера с учётом пропории
			mdsVal = static_cast<float>(mdsArea->verticalScrollBar()->size().height()) / proport;

			scrollVal = mde->verticalScrollBar()->value() + mde->verticalScrollBar()->size().height();

			mdsArea->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() + mdsVal);
			mde->verticalScrollBar()->setValue(scrollVal);
		}
		else if (!scrollPrior)	//Иначе монитор
		{
			//Если окно рендера имеет бОльшую абсолютную высоту, то получает приоритет
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->maximum());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->maximum());

			//Считаем пропорцию от размера окна рендера
			proport = mdsHgh / mdeHgh;

			//Расчёт смещения ползунка окна рендера с учётом пропории
			mdeVal = static_cast<float>(mdsArea->verticalScrollBar()->size().height()) / proport;

			scrollVal = mdsArea->verticalScrollBar()->value() + mdsArea->verticalScrollBar()->size().height();

			//Выполняем смещения для окон
			mdsArea->verticalScrollBar()->setValue(scrollVal);
			mde->verticalScrollBar()->setValue(mde->verticalScrollBar()->value() + mdeVal);
		}
	}
}

//Прокрутка вверх через полосу прокрутки
void LiteMD::slotMdsUp()
{
	float mdeVal, mdeHgh, mdsVal, mdsHgh, proport = 0.0f;
	if (!managePrior)
	{
		//Определяем приоритет
		if (mde->verticalScrollBar()->value() < mdsArea->verticalScrollBar()->value())
		{
			//Узнаём значение абсолютной высоты и текущее значение полузнка
			mdsVal = static_cast<float>(mdsArea->verticalScrollBar()->value());
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->size().height());
			try
			{
				//Вычисляем процентный сдвиг ведущей стороны(абс.высота/значение ползунка)
				proport = static_cast<float>(mdsHgh) / static_cast<float>(mdsVal);
			}
			catch (std::overflow_error& e)
			{
				mdsVal = 0.000001f;
				proport = static_cast<float>(mdsHgh) / static_cast<float>(mdsVal);
			}

			//Смещаем на полученный процент ведомую сторону
			mdeVal = static_cast<float>(mde->verticalScrollBar()->value());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->size().height()) * proport;

			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsArea->verticalScrollBar()->setValue(mdeVal - mdeHgh);
			mde->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() - mdsArea->verticalScrollBar()->size().height());
		}
		else if(mde->verticalScrollBar()->value() > mdsArea->verticalScrollBar()->value())
		{
			//Узнаём значение абсолютной высоты и текущее значение полузнка
			mdeVal = static_cast<float>(mde->verticalScrollBar()->value());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->size().height());
			try
			{
				//Вычисляем процентный сдвиг ведущей стороны(абс.высота/значение ползунка)
				proport = static_cast<float>(mdeHgh) / static_cast<float>(mdeVal);
			}
			catch (std::overflow_error& e)
			{
				mdeVal = 0.000001f;
				proport = static_cast<float>(mdeHgh) / static_cast<float>(mdeVal);
			}

			//Смещаем на полученный процент ведомую сторону
			mdsVal = static_cast<float>(mdsArea->verticalScrollBar()->value());
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->size().height()) * proport;

			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsArea->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() - mdsArea->verticalScrollBar()->size().height());
			mde->verticalScrollBar()->setValue(mdsVal - mdsHgh);
		}
	}
	else
	{
		//Если редактор
		if (scrollPrior)
		{
			//Узнаём значение абсолютной высоты и текущее значение полузнка
			mdeVal = static_cast<float>(mde->verticalScrollBar()->value());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->size().height());
			try
			{
				//Вычисляем процентный сдвиг ведущей стороны(абс.высота/значение ползунка)
				proport = static_cast<float>(mdeHgh) / static_cast<float>(mdeVal);
			}
			catch (std::overflow_error& e)
			{
				mdeVal = 0.000001f;
				proport = static_cast<float>(mdeHgh) / static_cast<float>(mdeVal);
			}

			//Смещаем на полученный процент ведомую сторону
			mdsVal = static_cast<float>(mdsArea->verticalScrollBar()->value());
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->size().height()) * proport;

			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsArea->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() - mdsArea->verticalScrollBar()->size().height());
			mde->verticalScrollBar()->setValue(mdsVal - mdsHgh);
		}
		else if (!scrollPrior)	//Иначе монитор
		{
			//Узнаём значение абсолютной высоты и текущее значение полузнка
			mdsVal = static_cast<float>(mdsArea->verticalScrollBar()->value());
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->size().height());
			try
			{
				//Вычисляем процентный сдвиг ведущей стороны(абс.высота/значение ползунка)
				proport = static_cast<float>(mdsHgh) / static_cast<float>(mdsVal);
			}
			catch (std::overflow_error& e)
			{
				mdsVal = 0.000001f;
				proport = static_cast<float>(mdsHgh) / static_cast<float>(mdsVal);
			}

			//Смещаем на полученный процент ведомую сторону
			mdeVal = static_cast<float>(mde->verticalScrollBar()->value());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->size().height()) * proport;

			//Увеличиваем значение полосы прокрутки, там самым крутим вниз
			mdsArea->verticalScrollBar()->setValue(mdeVal - mdeHgh);
			mde->verticalScrollBar()->setValue(mdsArea->verticalScrollBar()->value() - mdsArea->verticalScrollBar()->size().height());
		}
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
	//Фильтр вызова рекурсии. Например юзер пролистал редактор, изменился рендер, рендер послал
	//сигнал изменения и был принят здесь и изменится редактор, редактор снова пошлёт сигнал
	//и так по кругу. Чтобы такого не было подымается флаг фильтра, если он поднят то фильтр
	//сбрасывает его у себя в коде и выходит из функции убирая "звон"
	static bool ringFilter = 0;

	//Если синхронизация автоматическая, то синхронизироваться будет по окну из которого вызвано
	if (!syncCtl)
	{
		float mdeHgh, mdsHgh, proport = 0.0f;
		//Вычисляем хулигана по ip
		QObject* whoMeCall = sender();
		if (whoMeCall == mdsArea->verticalScrollBar())
		{
			if (ringFilter)
			{
				ringFilter = 0;
				return;
			}

			//Подымаем флаг фильтра рекурсионного вызова
			ringFilter = 1;

			//Если вызов от редактора - синхронизируем от него
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->maximum());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->maximum());

			//Рассчитываем пропорцию относительно редактора
			proport = mdsHgh / mdeHgh;

			mde->verticalScrollBar()->setValue(scroll / proport);
		}
		else if (whoMeCall == mde->verticalScrollBar())
		{
			if (ringFilter)
			{
				ringFilter = 0;
				return;
			}

			//Подымаем флаг фильтра рекурсионного вызова
			ringFilter = 1;

			//Если вызов от рендера - синхронизируемся
			mdsHgh = static_cast<float>(mdsArea->verticalScrollBar()->maximum());
			mdeHgh = static_cast<float>(mde->verticalScrollBar()->maximum());

			//Рассчитываем пропорцию относительно рендера
			proport = mdeHgh / mdsHgh;

			mdsArea->verticalScrollBar()->setValue(scroll / proport);
		}
	}
}

void LiteMD::slotRemoveRf()
{
	//Получаем список подпунктов меню
	QList<QAction*> actions(recentFiles->actions());

	//Чистим менюшку
	foreach(QAction * act, actions)
	{
		if (act != actclearRecent && act->isSeparator() == 0)
		{
			recentFiles->removeAction(act);
		}
	}

	//Чистим хранилище
	LastFileManager lfm;
	lfm.removeList();
}
