#include "ZanataTool.h"
#include <QApplication>
#include <QFileDialog>
#include <QSettings>

ZanataTool::ZanataTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void ZanataTool::AddInput()
{
	QSettings settings;

	ui.lstInputList->add
}
