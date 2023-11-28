#pragma once
#include <QtWidgets>
//Спизжено с SO
class OrientablePushButton : public QPushButton
{
	//Менеджер для размещения вертикальных кнопок
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
		//Конструкторы для инициализации
		OrientablePushButton(QWidget* oWgt = 0);
		OrientablePushButton(const QString&, QWidget* oWgt = 0);
		OrientablePushButton(const QIcon&, const QString&, QWidget* oWgt = 0);
		//Метод возврата размера кнопок
		QSize sizeHint()const;
		OrientablePushButton::Orientation orientation()const;
		//Метод установки положения кнопок
		void setOrientation(const OrientablePushButton::Orientation&);
};
