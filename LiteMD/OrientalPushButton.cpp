#include "OrientalPushButton.h"
#include <QtWidgets>
OrientablePushButton::OrientablePushButton(QWidget* oWgt) : QPushButton(oWgt)
{}
OrientablePushButton::OrientablePushButton(const QString& text, QWidget* oWgt) : QPushButton(text, oWgt)
{}
OrientablePushButton::OrientablePushButton(const QIcon& icon, const QString& text, QWidget* oWgt) : QPushButton(icon, text, oWgt)
{}
QSize OrientablePushButton::sizeHint()const
{
	QSize size = QPushButton::sizeHint();
	if (mOrientation != OrientablePushButton::Horizontal)
		size.transpose();
	return size;
}
void OrientablePushButton::paintEvent(QPaintEvent* pe)
{
	Q_UNUSED(pe);
	QStylePainter painter(this);
	QStyleOptionButton option;
	initStyleOption(&option);
	if (mOrientation == OrientablePushButton::VerticalTopBottom)
	{
		painter.rotate(90);
		painter.translate(0, -1 * width());
		option.rect = option.rect.transposed();
	}
	else if (mOrientation == OrientablePushButton::VerticalBottomTop)
	{
		painter.rotate(-90);
		painter.translate(-1 * height(), 0);
		option.rect = option.rect.transposed();
	}
	painter.drawControl(QStyle::CE_PushButton, option);
}
OrientablePushButton::Orientation OrientablePushButton::orientation()const
{
	return mOrientation;
}
void OrientablePushButton::setOrientation(const OrientablePushButton::Orientation& orientation)
{
	mOrientation = orientation;
}
