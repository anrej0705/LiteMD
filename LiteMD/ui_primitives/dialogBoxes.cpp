#include <QtWidgets>
#include "dialogBoxes.h"
#include "LiteMD.h"
//Окно подтверждения сохранения
bool confirmSave()
{
	//Создаём окно спрашивающее подтверждение юзера
	QMessageBox qmsg(QMessageBox::Warning, QObject::tr("File not saved"), QObject::tr("You dont save changes in file, which is opened currently. Save?"), QMessageBox::Yes | QMessageBox::No);

	//Задаём моднявую иконку
	QPixmap appIcon(getAppPath() + "/icon.ico");
	appIcon.setMask(appIcon.createMaskFromColor(QColor(0, 0, 0)));
	qmsg.setWindowIcon(QIcon(appIcon));

	//Спрашиваем пользователя
	int confirm_status = qmsg.exec();
	//Если пользователь согласился то возвращаем 1, иначе 0
	if (confirm_status == QMessageBox::Yes)
		return 1;
	else if (confirm_status == QMessageBox::No)
		return 0;
	//Возвращаем 0 если пользователь отменил
	return 0;
}
