#ifndef SETTINGSAPP_H
#define SETTINGSAPP_H

#include <QObject>
#include <QWidget>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QStyle>

enum language_app{
    English = 0,
    Russian
};

class SettingsApp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool darkTheme
               MEMBER m_darkTheme
               READ getDarkTheme
               WRITE setDarkTheme
               NOTIFY themeChanged)

    Q_PROPERTY(int language
               MEMBER m_language
               READ getLanguage
               WRITE setLanguage
               NOTIFY languageChanged)

public:
    explicit SettingsApp(QObject *parent = nullptr);

    Q_INVOKABLE bool getDarkTheme();
    void setDarkTheme(bool _darkTheme);

    Q_INVOKABLE int getLanguage();
    void setLanguage(int _language);

    QPalette getPalette();
    QString getQss();


private:

    static QTranslator translator;
    static QTranslator translatorStandart;
//    static bool themeChg;
//    static bool langGhg;

    static QPalette paletteApp;
    static QString qssApp;

    void themeChange();
    void languageChange();

    bool m_darkTheme;
    int m_language;


    QSettings mySetting {"settings.ini", QSettings::Format::IniFormat, this};

private slots:


signals:
    void themeChanged(QString, QPalette);
    void languageChanged();
};



#endif // SETTINGSAPP_H
