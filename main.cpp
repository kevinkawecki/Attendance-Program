#include "cmmGui.h"
#include <QtWidgets/QApplication>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	cmmGui gui;

	return app.exec();
}
