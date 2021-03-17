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
	a.setStyle(QStyleFactory::create("fusion"));

	QTranslator myTranslator;
	myTranslator.load(QLocale::system().name());
	a.installTranslator(&myTranslator);

	const QIcon mainIcon = QIcon::fromTheme("mainIcon", QIcon("icon1.ico"));
	a.setWindowIcon(mainIcon);

	EES excelExportScada;
	excelExportScada.setMinimumHeight(400);
	excelExportScada.setMinimumWidth(600);
	excelExportScada.show();

	return a.exec();
}