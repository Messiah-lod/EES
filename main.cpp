//#include <QtWidgets/QApplication>

//#include <ActiveQt/QAxObject>
//#include <QtGui>
//#include <QtSql>

//#include <QGuiApplication>

//#include <QQmlApplicationEngine>
//#include <QQmlContext>

#include <QtWidgets>
#include <QApplication>

#include "EES.h"

int main(int argc, char *argv[])
{
//    SetConsoleCP(1251);
//    SetConsoleOutputCP(1251);
//    setlocale(LC_ALL,"RUS");

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));

    QTranslator *translator = new QTranslator(qApp);
    static_cast<void>(translator->load(":/translator/RU_ru.qm"));
    QTranslator *translatorStandart = new QTranslator(qApp);
    static_cast<void>(translatorStandart->load(":/translator/qtbase_ru.qm"));
    qApp->installTranslator(translator);
    qApp->installTranslator(translatorStandart);

    const QIcon mainIcon = QIcon::fromTheme("mainIcon", QIcon(":/pic/icon.ico"));
    a.setWindowIcon(mainIcon);

    EES excelExportScada;
    excelExportScada.setMinimumHeight(800);
    excelExportScada.setMinimumWidth(1000);
    excelExportScada.show();

    return a.exec();

//#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif
//    QGuiApplication app(argc, argv);
//    QApplication a(argc, argv);

//    qmlRegisterType<ModelFillingDB>("ModelFillingDB", 1, 0, "ModelFillingDB");

//    QQmlApplicationEngine engine;
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);


//    ModelFillingDB *model = new ModelFillingDB;
//    engine.rootContext()->setContextProperty("model", model);


//    engine.load(url);

////    qDebug() << "Доступные драйверы:" << QSqlDatabase::drivers();


//    return app.exec();
}
