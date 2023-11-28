#include <QtWidgets>
#include "dialogBoxes.h"
//Окно подтверждения сохранения
bool confirmSave()
{
	//Спрашиваем пользователя
	int confirm_status = QMessageBox(QMessageBox::Warning, QObject::tr("File not saved"), QObject::tr("You dont save changes in file, which is opened currently. Save?"), QMessageBox::Yes | QMessageBox::No).exec();
	//Если пользователь согласился то возвращаем 1, иначе 0
	if (confirm_status == QMessageBox::Yes)
		return 1;
	else if (confirm_status == QMessageBox::No)
		return 0;
	//Возвращаем 0 если пользователь отменил
	return 0;
}
