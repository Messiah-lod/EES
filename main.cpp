#include <QtWidgets/QApplication>
#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>

#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QtGui>
#include <QtSql>

#include "modelfillingdb.h"


int main(int argc, char *argv[])
{
//	QTranslator myTranslator;
//	myTranslator.load(QLocale::system().name());
//	a.installTranslator(&myTranslator);

//	const QIcon mainIcon = QIcon::fromTheme("mainIcon", QIcon("icon1.ico"));
//	a.setWindowIcon(mainIcon);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QApplication a(argc, argv);

    qmlRegisterType<ModelFillingDB>("ModelFillingDB", 1, 0, "ModelFillingDB");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    ModelFillingDB *model = new ModelFillingDB;
    engine.rootContext()->setContextProperty("model", model);


    engine.load(url);

//    qDebug() << "Доступные драйверы:" << QSqlDatabase::drivers();


    return app.exec();
}
