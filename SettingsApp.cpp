#include "SettingsApp.h"

SettingsApp::SettingsApp(QObject *parent)
    : QObject{parent}
{
    getDarkTheme();
    getLanguage();

    themeChange();
    languageChange();

//    timer = new QTimer();
//    timer->setInterval(1000);
//    QObject::connect(timer, &QTimer::timeout, this, &SettingsApp::updateTimer);
//    timer->start();
}


bool SettingsApp::getDarkTheme()
{
    m_darkTheme = mySetting.value("Theme").toBool();
    //    qDebug() << "Взяли параметр темной темы " + QString::number(theme);
    return m_darkTheme;
}

void SettingsApp::setDarkTheme(bool _darkTheme)
{

    mySetting.setValue("Theme", _darkTheme);
    m_darkTheme = _darkTheme;
    themeChange();
}

int SettingsApp::getLanguage()
{
    m_language = mySetting.value("Lang").toInt();
    //    qDebug() << "Взяли параметр языка " + QString::number(lang);
    return m_language;
}

void SettingsApp::setLanguage(int _language)
{
    qDebug() << "Смена языка";
    mySetting.setValue("Lang", _language);
    m_language = _language;
    languageChange();
    emit languageChanged();
 //   langGhg = true;
}

QPalette SettingsApp::getPalette()
{
    return paletteApp;
}

QString SettingsApp::getQss()
{
    return qssApp;
}

void SettingsApp::themeChange()
{
    //метод изменения палитры приложения
    if(m_darkTheme) {
        qssApp = "QWidget{""background-color: rgb(53, 53, 53);""text-color: rgb(255, 255, 255);""}"
                 "QTextEdit{""background-color: rgb(25, 25, 25);""border-color: rgb(0, 179, 255);"
                 "             ""border: 5px solid rgb(0, 179, 255);""text-color: rgb(255, 255, 255);""}"
                 "QPushButton{""border-radius: 00px;""background-color: rgb(53, 53, 53);""qproperty-iconSize: 28px 28px;""}"
                 "QPushButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}"
                 "QToolButton{""border-radius: 00px;""background-color: rgb(53, 53, 53);""qproperty-iconSize: 28px 28px;""}"
                 "QToolButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}";

        paletteApp.setColor(QPalette::Window, QColor(53, 53, 53));
        paletteApp.setColor(QPalette::WindowText, Qt::white);
        paletteApp.setColor(QPalette::Base, QColor(25, 25, 25));
        paletteApp.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        paletteApp.setColor(QPalette::ToolTipBase, Qt::white);
        paletteApp.setColor(QPalette::ToolTipText, Qt::white);
        paletteApp.setColor(QPalette::Text, Qt::white);
        paletteApp.setColor(QPalette::Button, QColor(53, 53, 53));
        paletteApp.setColor(QPalette::ButtonText, Qt::white);
        paletteApp.setColor(QPalette::BrightText, Qt::red);
        paletteApp.setColor(QPalette::Link, QColor(42, 130, 218));
        paletteApp.setColor(QPalette::Highlight, QColor(42, 130, 218));
        paletteApp.setColor(QPalette::HighlightedText, Qt::black);
    }
    else{
        qssApp =   "QWidget{""background-color: rgb(239, 239, 239);""}"
                   "QTextEdit{""background-color: rgb(255, 255, 255);""border-color: rgb(0, 179, 255);""border: 5px solid rgb(0, 179, 255);""}"
                   "QPushButton{""border-radius: 00px;""background-color: rgb(239, 239, 239);""qproperty-iconSize: 28px 28px;""}"
                   "QPushButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}"
                   "QToolButton{""border-radius: 00px;""background-color: rgb(239, 239, 239);""qproperty-iconSize: 28px 28px;""}"
                   "QToolButton:pressed{"" border-radius: 10px;""background-color: rgb(0, 179, 255);""}";

        paletteApp = qApp->style()->standardPalette();
    }

    qApp->setPalette(paletteApp);
}

void SettingsApp::languageChange()
{
    if (m_language == language_app::Russian){//русский
        qDebug() << "Смена языка на русский";
        static_cast<void>(translator.load(":/translator/RU_ru.qm"));
        static_cast<void>(translatorStandart.load(":/translator/qtbase_ru.qm"));

    }
    if (m_language == language_app::English){//английский
        qDebug() << "Смена языка на английский";
        static_cast<void>(translator.load(""));
        static_cast<void>(translatorStandart.load(""));
    }

    qApp->installTranslator(&translator);
    qApp->installTranslator(&translatorStandart);
}

//void SettingsApp::updateTimer()
//{
//    if(themeChg) {
//        qApp->setPalette(paletteApp);
//        emit themeChanged(qssApp, paletteApp);
//        themeChg = false;
//    }
//    if(langGhg) {
//        emit languageChanged();
//        langGhg = false;
//    }
//    //    if(!translator.isEmpty()) qDebug() << "Транслятор жив";
//}


//определение статических членов класса
//bool SettingsApp::themeChg;
//bool SettingsApp::langGhg;
QTranslator SettingsApp::translator;
QTranslator SettingsApp::translatorStandart;
QPalette SettingsApp::paletteApp;
QString SettingsApp::qssApp;
