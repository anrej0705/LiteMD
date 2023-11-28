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
	//Получаем размер кнопки
	QSize size = QPushButton::sizeHint();
	//Если кнопк горизонтальна то преобразуем
	if (mOrientation != OrientablePushButton::Horizontal)
		size.transpose();
	//Возвращаем размер
	return size;
}
//Отрисовщик
void OrientablePushButton::paintEvent(QPaintEvent* pe)
{
	Q_UNUSED(pe);	//Какая-то хуйня
	//Создаем кастомный отрисовкщик
	QStylePainter painter(this);
	QStyleOptionButton option;
	initStyleOption(&option);
	//Если кнопка вертикальная сверху вниз то переворачиваем
	if (mOrientation == OrientablePushButton::VerticalTopBottom)
	{
		painter.rotate(90);
		painter.translate(0, -1 * width());
		option.rect = option.rect.transposed();
	}
	//Иначе рисуем снизу вверх
	else if (mOrientation == OrientablePushButton::VerticalBottomTop)
	{
		painter.rotate(-90);
		painter.translate(-1 * height(), 0);
		option.rect = option.rect.transposed();
	}
	//Рисуем на виджете
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
