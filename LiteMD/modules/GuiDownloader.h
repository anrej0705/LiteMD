#pragma once
#include <QtWidgets>
#include <qurl.h>
#include "Downloader.h"
//����������� �������� ��� ���������� �����������
class DownloaderGui : public QWidget
{
	Q_OBJECT
	private:
		//������ ����������
		Downloader* dw;
		//����� ���������
		QProgressBar* dwPb;
		//���� ����� ������
		QLineEdit* dwEt;
		//������ ������ ��������
		QPushButton* dwPt;
		//����� ��� ������ ����������� ��������
		void showPic(const QString&);
		//������� ������ �������
		QLabel* plbl;
		bool warned;
	protected:
		bool eventFilter(QObject* pobj, QEvent* event);
		void update_ui();
	public:
		//������� �����������
		DownloaderGui(QWidget* dwgt = 0);
		//������� ����������
		~DownloaderGui();
		//������� �������� ����
		void closeEvent(QCloseEvent*);
	private slots:
		//���� ��������� ��������
		void slotGo();
		//���� ����������� ������
		void slotError();
		//���� �������������� ������� ��������� ��������
		void slotDownloadProgress(qint64, qint64);
		//���� ������������ ��� �������� ���������
		void slotDone(const QUrl&, const QByteArray&);
	public slots:
		//����, ������������ ����
		void slotShow();
};
