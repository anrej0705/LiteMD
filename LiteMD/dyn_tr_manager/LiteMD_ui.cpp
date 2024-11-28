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
	editorWindow->setTitle(tr("Editor"));
	viewerWindow->setTitle(tr("Viewer"));
	workProgressCap->setText(tr("work in progress"));
	setWindowTitle(tr("LiteMD") + APP_STAGE + APP_VERSION + tr(" build ") + QString::number(static_cast<uint32_t>(BUILD_NUMBER))/* + tr("[MAX FILE SIZE 65K]")*/);

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
	//------------------------
}

void LiteMD::update_interactive()
{
	//Тернарками обновляем менюшки
	enableIndevFeatures == 0 ? actSetTextFormat->setDisabled(1) : actSetTextFormat->setEnabled(1);
	enableIndevFeatures == 0 ? actHelp->setDisabled(1) : actHelp->setEnabled(1);
	enableIndevFeatures == 0 ? setBold->setDisabled(1) : setBold->setEnabled(1);
	enableIndevFeatures == 0 ? setItalic->setDisabled(1) : setItalic->setEnabled(1);
	enableIndevFeatures == 0 ? actHelp->setDisabled(1) : actHelp->setEnabled(1);
	enableIndevFeatures == 0 ? setStrikethrough->setDisabled(1) : setStrikethrough->setEnabled(1);
	enableIndevFeatures == 0 ? formatStyle->setDisabled(1) : formatStyle->setEnabled(1);
	enableIndevFeatures == 0 ? actSetTextFormat->setDisabled(1) : actSetTextFormat->setEnabled(1);
}
