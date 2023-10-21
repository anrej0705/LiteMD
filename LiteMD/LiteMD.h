#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LiteMD.h"

class LiteMD : public QMainWindow
{
    Q_OBJECT

public:
    LiteMD(QWidget *parent = nullptr);
    ~LiteMD();

private:
    Ui::LiteMDClass ui;
};
