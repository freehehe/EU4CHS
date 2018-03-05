#include "ZanataTool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ZanataTool w;
	w.show();
	return a.exec();
}
