#pragma once

#include "SQL_to_FB.h"
#include "BaseSQL.h"
#include "ModelObjectProject.h"
#include "ModelLinkData.h"
#include "Logger.h"
#include "FileJob.h"
#include "ChangeLine.hpp"
#include "SettingsApp.h"

#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 
#include <QAxObject>
#include <QQuickView>
#include <QQmlEngine>
#include <QSettings>
#include <QComboBox>
#include <QObject>
#include <QQuickItem>

#include <iostream>
#include "windows.h"
#include <string>
#include <any>
#include <vector>
#include <conio.h> //для getch
#include <thread>
#include <time.h>
#include <cctype>

class EES : public QMainWindow
{
    Q_OBJECT

public:
    EES(QWidget *parent = nullptr);
    ~EES();

    int initializingConnection(std::string path);

private:

    QWidget *centreWidget {nullptr};
    QSplitter *splitterVertical {nullptr};

    QTabWidget *tabWidget {nullptr}; //организуем вкладки
    QWidget *tab_tableData {nullptr};//вкладка создания
    QWidget *tab_tableDataLink {nullptr};//вкладка связки

    enum Tab {//добавим перечисления для уникального идентифицирования вкладок
        enum_tableData = 0,
        enum_tableDataLink
    };

    QGridLayout *grid_tab_tableData {nullptr}; //создаем грид, который кладем на вкладку
    QTableView *tableView {nullptr};//создаем вью, который положим на грид вкладки
    ModelObjectProject *tableData {nullptr};//создадим модель, которую вложим во вью

    QGridLayout *grid_tableDataLink {nullptr};//создаем грид, который кладем на вкладку
    QTableView *tableViewLink {nullptr};//создаем вью, который положим на грид вкладки
    ModelLinkData *tableDataLink {nullptr};//создадим модель, которую вложим во вью

    QPushButton *buttonLoad {nullptr};
    QPushButton *buttonUpLoad {nullptr};
    QPushButton *buttonConnect {nullptr};
    QPushButton *buttonAbout {nullptr};
    QPushButton *buttonSettings {nullptr};
    QPushButton *buttonSelectAll {nullptr};

    QTreeView *textTreeLogs {nullptr};
    QStandardItemModel* textTreeLogsModel {nullptr};
    QGridLayout *gridLayout {nullptr};
    QComboBox *txtPath {nullptr};
    QToolBar *aboutToolBar {nullptr};

    //переменные для подключения к БД
    std::string webserver;
    std::string path;

    Logger logs{"logsSQL"};

    QPalette darkPalette;
    std::vector<int> numberOfRows; //перечень колонок для заливки

    void setDataToModel(QString fileName, int currentTab = 0);
    void fillingToDB(std::vector<int> rows);

    QQuickView *viewSettings{nullptr};
    QWidget *containerSettings{nullptr};
    QQuickView *viewAbout{nullptr};
    QWidget *containerAbout{nullptr};

    QSettings mySetting {"settings.ini", QSettings::Format::IniFormat, this};
    SettingsApp *settingsApp {nullptr};

    QStringList listPath;
    QAction *actEdit;
    QAction *actDel;
    int selectedIndexPath = 0;
    ChangeLine *changeLine {nullptr};

    bool eventFilter(QObject *o, QEvent *e);


    QTranslator *translator = nullptr;
    QTranslator *translatorStandart = nullptr;

public slots:
    void retranslateUi();
//    void redrawUI(QString _qss, QPalette _palette);
    void redrawUI();

    void on_buttonLoad_clicked();
    void on_buttonUpLoad_clicked();
    bool on_buttonConnect_clicked();
    void on_buttonAbout_clicked();
    void on_buttonSettings_clicked();
    void on_buttonSelectAll_clicked();

    void closeTab(int index);

    void closeAbout();

    void list_context_menu(QPoint pos);
    void changePath(int index);
    void selectedPath(int index);
    void on_actionEdit_clicked();
    void on_actionDel_clicked();
    void addChangePath(QString path, int index);

    void slotFinishSetDataToModel();
    void slotFinishFillingToDB();


signals:
    void finishSetDataToModel();
    void finishFillingToDB();
};
