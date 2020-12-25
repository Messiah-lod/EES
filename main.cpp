#include "EES.h"
#include <QtWidgets/QApplication>

#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>

int main(int argc, char *argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	QApplication a(argc, argv);

	QTranslator myTranslator;
	myTranslator.load(QLocale::system().name());
//	myTranslator.load("RU_ru");
	a.installTranslator(&myTranslator);

	EES excelExportScada;
	excelExportScada.setMinimumHeight(400);
	excelExportScada.setMinimumWidth(600);
	excelExportScada.show();

	return a.exec();
}