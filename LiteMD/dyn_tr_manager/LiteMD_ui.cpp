#include "LiteMD.h"
#include "global_definitions.h"

bool LiteMD::eventFilter(QObject* pobj, QEvent* p_event)
{
	if (p_event->type() == static_cast<QEvent::Type>(QEvent::User + APP_EVENT_LiteMD_UPDATE_EVENT))
	{
		update_ui();
		return 1;
	}
	return QWidget::eventFilter(pobj, p_event);
}

void LiteMD::update_ui()
{
	editorWindow->setTitle(tr("Editor"));
	viewerWindow->setTitle(tr("Viewer"));
	actAbout->setText(tr("&About"));
	actOpen->setText(tr("&Open..."));
	actSave->setText(tr("&Save"));
	actSaveAs->setText(tr("S&ave As..."));
	actQuit->setText(tr("&Quit"));
	actDownloader->setText(tr("HTTP &Downloader module"));
	actSet->setText(tr("&Settings"));
	actNew->setText(tr("&New"));
	mFile->setTitle(tr("&File"));
	mSettings->setTitle(tr("&Service"));
	mHelp->setTitle(tr("&Help"));
	workProgressCap->setText(tr("work in progress"));
	setWindowTitle(tr("LiteMD alpha 0.0.0 build ") + QString::number(buildNumber) + tr("[MAX FILE SIZE 65K]"));
}
