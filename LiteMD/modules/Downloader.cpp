#include <QtWidgets>
#include <qurl.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include "Downloader.h"
#include "logger_backend.h"
#include <boost/container/string.hpp>
//������� �����������, ������� ������ ����������
Downloader::Downloader(QObject* dobj) : QObject(dobj)
{
	push_log("[HTTP ���������]������������� �������");
	//�������������� �������� �����������
	netman = new QNetworkAccessManager(this);
	//��������� ������ ��������� � ����� ��������������� ������ ���������� ��������
	if (!QObject::connect(netman, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotFinished(QNetworkReply*))))
		QErrorMessage::qtHandler();
}
//��������� �������
void Downloader::download(const QUrl& url)
{	//��������� ��� ������� ���� ����� ��������� � ����
	boost::container::string* log_stroke = new boost::container::string;

	log_stroke->append("[HTTP ���������]�������� �� URL ");
	log_stroke->append(url.toString().toLocal8Bit());
	push_log(log_stroke->c_str());

	//������� ������ ���������� � �������� ������ �� ������� ����� ������
	QNetworkRequest netreq(url);
	//��������� ������ ������ ���� � ��������� ��� ��������� ��� ��������� ������� ���������
	QNetworkReply* netrep = netman->get(netreq);
	//��������� ������ ��������� �������� ������ ����� �� ���� � ����� ������� ����� ���������� �� ���������� ��������
	if (!QObject::connect(netrep, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64))))
		QErrorMessage::qtHandler();

	delete(log_stroke);
}
//����, ���������� �� ���������� ��������
void Downloader::slotFinished(QNetworkReply* netrep)
{
	//��������� ������ ������. ���� ���� ������ �� ���������� ������ �� ������
	if (netrep->error() != QNetworkReply::NoError)
		emit error();
	else
		//����� ���������� ������ ���������� ��������
		emit done(netrep->url(), netrep->readAll());
	//���������� ������ �� �������� ������� �� �����������
	netrep->deleteLater();
}
