#include "LiteMD.h"
#include "global_definitions.h"
#include "logger_backend.h"

bool LiteMD::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_LiteMD_UPDATE_EVENT))
	{
		update_ui();
		update_interactive();
		return 1;
	}
	return QWidget::eventFilter(pobj, p_event);
}

void LiteMD::update_ui()
{
	push_log("[LiteMD_ui]Обновление интерфейса");

	//Создаём список потомков
	QList<QWidget*> wList = this->findChildren<QWidget*>(); 
		
	//Дрюкаем новым стилем каждого по очереди
	foreach(QWidget * sWgt, wList)
	{
		sWgt->setStyle(QStyleFactory::create(chosenTheme));
	}

	//Сбрасываем флаг
	//uiChanged = 0;

	editorWindow->setTitle(tr("Editor"));
	viewerWindow->setTitle(tr("Viewer"));
	workProgressCap->setText(tr("work in progress"));
	setWindowTitle(tr("LiteMD") + APP_STAGE + APP_VERSION + tr(" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))/* + tr("[MAX FILE SIZE 65K]")*/ + " [" + tr("Untitled") + "]");

	//Обновляем перевод меню
	mFile->setTitle(tr("&File"));
	mEdit->setTitle(tr("&Edit"));
	mSettings->setTitle(tr("&Service"));
	mHelp->setTitle(tr("&Help"));
	headersMenu->setTitle(tr("Set headers"));
	formatStyle->setTitle(tr("Set format style"));
	//----------------------

	//Обновляем перевод кнопок
	actAbout->setText(tr("&About"));
	actOpen->setText(tr("&Open..."));
	actSave->setText(tr("&Save"));
	actSaveAs->setText(tr("S&ave As..."));
	actQuit->setText(tr("&Quit"));
	actDownloader->setText(tr("HTTP &Downloader module"));
	actSet->setText(tr("&Settings"));
	actNew->setText(tr("&New"));
	actPlaceUrl->setText(tr("Make &URL"));
	actPlaceAltUrl->setText(tr("Make alt&enate URL"));
	actSetTextFormat->setText(tr("Te&xt Format"));
	actOpenChangelog->setText(tr("Sh&ow changelog"));
	actHelp->setText(tr("&Help"));
	actBugReport->setText(tr("&Bug!"));
	actSetH1->setText(tr("Set H1"));
	actSetH2->setText(tr("Set H2"));
	actSetH3->setText(tr("Set H3"));
	actSetH4->setText(tr("Set H4"));
	actSetH5->setText(tr("Set H5"));
	actShieldSymbol->setText(tr("Es&cape character"));
	setBold->setText(tr("Set bold"));
	setItalic->setText(tr("Set italic"));
	setUnderlined->setText(tr("Set underlined"));
	setStrikethrough->setText(tr("Set strikethrough"));
	checkUpdates->setText(tr("checkUpdates"));
	actClose->setText(tr("Close"));
	recentFiles->setTitle(tr("recentFiles"));
	actclearRecent->setText(tr("actclearRecent"));
	actInsertLi->setText(tr("actInsertLi"));
	//------------------------
}

void LiteMD::update_interactive()
{
	//Тернарками обновляем менюшки
	enableIndevFeatures == 0 ? actHelp->setDisabled(1) : actHelp->setEnabled(1);
	enableIndevFeatures == 0 ? actHelp->setDisabled(1) : actHelp->setEnabled(1);
	enableIndevFeatures == 0 ? checkUpdates->setDisabled(1) : checkUpdates->setEnabled(1);
}
