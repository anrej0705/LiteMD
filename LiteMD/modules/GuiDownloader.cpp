#include <QtWidgets>
#include <qurl.h>
#include "Downloader.h"
#include "GuiDownloader.h"
extern "C"
{
	#include "globalFlags.h"
}
//������� �����������
DownloaderGui::DownloaderGui(QWidget* dwgt) : QWidget(dwgt)
{
	warned = 0;
	//������������� ��������� ���� ������
	setWindowTitle(tr("HTTP Download module GUI(Deprecated)"));
	//�������������� ������� ����������
	dw = new Downloader(this);
	//������� ������
	plbl = new QLabel;
	plbl->setWindowIcon(QIcon("icon.ico"));
	plbl->hide();
	//�������������� ����������� ��������
	dwPb = new QProgressBar;
	dwEt = new QLineEdit;
	dwPt = new QPushButton("&Go");
	//������� ������ � ���������� ������-������, 04.01.2023 ��������
	QString strDownloadLink = "http://i122.fastpic.org/thumb/2023/1127/35/_d9628b5877c863ff532cc98daa37b735.jpeg";
	//�������� ������ � ���� �����
	dwEt->setText(strDownloadLink);
	//��������� ������ ������ �� ������ ���������� ��������
	if (!connect(dwPt, SIGNAL(clicked()), SLOT(slotGo())))
		QErrorMessage::qtHandler();
	//��������� ������ ��������� �������� �� ������ ������������ �������� ��������
	if (!connect(dw, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(slotDownloadProgress(qint64, qint64))))
		QErrorMessage::qtHandler();
	//��������� ������ ���������� �� ������
	if (!connect(dw, SIGNAL(done(const QUrl&, const QByteArray&)), this, SLOT(slotDone(const QUrl&, const QByteArray&))))
		QErrorMessage::qtHandler();
	//�������������� ����� ��� ���������� ��������
	QGridLayout* gLay = new QGridLayout;
	//��������� �������
	gLay->addWidget(dwEt, 0, 0);
	gLay->addWidget(dwPt, 0, 1);
	gLay->addWidget(dwPb, 1, 0, 1, 1);
	//������������� ��� ����
	setLayout(gLay);
}
//������� ����������
DownloaderGui::~DownloaderGui()
{
	if (appClose)
	{
		plbl->close();
		close();
	}
	else
	{
		hide();
	}
}
//���� ���������� ��������
void DownloaderGui::slotGo()
{
	//�������� ����� ������ �������� � �������� ������ �� ���� �����
	dw->download(QUrl(dwEt->text()));
}
//���� ����������� ������� ��������� ��������
void DownloaderGui::slotDownloadProgress(qint64 nRecs, qint64 nTotal)
{
	//���� n ������ 0 ������ � ��� ������
	if (nTotal <= 0)
	{
		//�������� ���� ������ � ���������� ��������
		slotError();
		return;
	}
	//����� ���������� �������� � ���������
	dwPb->setValue(100 * nRecs / nTotal);
}
//���� ����������� ������ � ���������� ��������
void DownloaderGui::slotDone(const QUrl& url, const QByteArray& ba)
{
	//������� ��������� ����� �����, �������������� ������� �� ���� ������ �������
	QString strFileName = url.path().section('/', -1);
	//������� ������ �����
	QFile file(url.path().section('/', -1));
	//���� ������ �������� � ������ �� ���������
	if (file.open(QIODevice::WriteOnly))
	{
		//���������� ���������� ������ � ����
		file.write(ba);
		//��������� ����
		file.close();
		//���� ����� ���� ���� ����� ���������� jpg ��� png �� ������ ��� ��������
		if(strFileName.endsWith(".jpg") || strFileName.endsWith(".png") || strFileName.endsWith(".jpeg"))
			//���������� ��������
			showPic(strFileName);
	}
	dwPb->setValue(0);
}
//����� ��������� �� ����� ��������
void DownloaderGui::showPic(const QString& strFileName)
{
	//������� ������ ��������
	QPixmap pix(strFileName);
	//����������� ��� �������� ������ � 2(1) ���� �� ����� ��������. ���������� ����� ����������� � ������������� �����.������
	pix = pix.scaled(pix.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	//�������� ��������
	plbl->setPixmap(pix);
	//������������ ��������� �������
	plbl->setFixedSize(pix.size());
	//������������� ��������� ����
	plbl->setWindowTitle(tr("View"));
	//���������� �� �����
	plbl->show();
}
//���� ����������� ������
void DownloaderGui::slotError()
{
	//�������� ��������� � ������� ������ ��� ��������� ��� ��������
	QMessageBox::critical(0, tr("Error"), tr("An error while download is occured"));
}
void DownloaderGui::slotShow()
{
	this->move(190, 80);
	this->resize(800, 80);
	this->show();
	if (!httpDerpWarned)
	{
		QMessageBox::information(0, tr("And he won't live long..."), tr("This module was added experimentally. In the future it will undergo changes or disappear"));
		httpDerpWarned = 1;
	}
}
void DownloaderGui::closeEvent(QCloseEvent* ce)
{
	if (appClose)
	{
		plbl->close();
		ce->accept();
	}
	else
	{
		hide();
	}
}
