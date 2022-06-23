//#include <QtWidgets/QApplication>

//#include <ActiveQt/QAxObject>
//#include <QtGui>
//#include <QtSql>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QtWidgets>
#include <QApplication>
#include <QPluginLoader>
#include <QQuickStyle>

#include "EES.h"
#include "FileJob.h"
#include "SettingsApp.h"


int main(int argc, char *argv[])
{
    //    SetConsoleCP(1251);
    //    SetConsoleOutputCP(1251);
    //    setlocale(LC_ALL,"RUS");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif


    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<FileAbout>("FileJob", 1, 0, "FileJob");
    qmlRegisterType<SettingsApp>("SettingsApp", 1, 0, "SettingsApp");

    const QUrl url(QStringLiteral(""));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    //    engine.load(url);



    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
//    QQuickStyle::setStyle("Material");

    qDebug() << "Used drivers: :" << QSqlDatabase::drivers();

//    QTranslator *translator = new QTranslator(qApp);
//    static_cast<void>(translator->load(":/translator/RU_ru.qm"));
//    QTranslator *translatorStandart = new QTranslator(qApp);
//    static_cast<void>(translatorStandart->load(":/translator/qtbase_ru.qm"));
//    qApp->installTranslator(translator);
//    qApp->installTranslator(translatorStandart);

    const QIcon mainIcon = QIcon::fromTheme("mainIcon", QIcon(":/pic/icon.ico"));
    a.setWindowIcon(mainIcon);


//    QQuickView *view = new QQuickView;
//    view->setSource(QUrl(QLatin1String("qrc:/Splash.qml")));
//    view->show();
 //   view->close();


    EES excelExportScada;
    excelExportScada.setMinimumWidth(800);
    excelExportScada.setMinimumHeight(600);
    excelExportScada.show();





    //    qDebug() << "Доступные драйверы:" << QSqlDatabase::drivers();




    //    return app.exec();

    return a.exec();
}
