#pragma once
#include <QtWidgets>
class OrientablePushButton : public QPushButton
{
	Q_OBJECT
	public:
		enum Orientation
		{
			Horizontal,
			VerticalTopBottom,
			VerticalBottomTop
		};
	private:
		Orientation mOrientation = Horizontal;
	protected:
		void paintEvent(QPaintEvent*);
	public:
		OrientablePushButton(QWidget* oWgt = 0);
		OrientablePushButton(const QString&, QWidget* oWgt = 0);
		OrientablePushButton(const QIcon&, const QString&, QWidget* oWgt = 0);
		QSize sizeHint()const;
		OrientablePushButton::Orientation orientation()const;
		void setOrientation(const OrientablePushButton::Orientation&);
};
