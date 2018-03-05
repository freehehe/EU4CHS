#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ZanataTool.h"


#include "Worker.h"

class ZanataTool : public QMainWindow
{
	Q_OBJECT

public:
	ZanataTool(QWidget *parent = Q_NULLPTR);

	void AddInput();

private:
	Ui::ZanataToolClass ui;

	Worker m_Instance;
};
