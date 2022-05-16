#pragma once

#include "SQL_to_FB.h"
#include "BaseSQL.h"
#include "ModelObjectProject.h"
#include "ModelLinkData.h"
#include "Logger.h"

#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 
#include <QAxObject>

#include <iostream>
#include "windows.h"
#include <string>
#include <any>
#include <vector>
#include <conio.h> //для getch
#include <thread>

class EES : public QWidget
{
	Q_OBJECT

public:
    EES(QWidget *parent = nullptr);
	void retranslateUi();
	int initializingConnection(std::string path);

private:

	QSplitter *splitterVertical;

	QTabWidget *tabWidget; //организуем вкладки
	QWidget *tab_tableData;//вкладка создания
	QWidget *tab_tableDataLink;//вкладка связки

	enum Tab {//добавим перечисления для уникального идентифицирования вкладок
		enum_tableData = 0,
		enum_tableDataLink
	};
	
	QGridLayout *grid_tab_tableData; //создаем грид, который кладем на вкладку
	QTableView *tableView;//создаем вью, который положим на грид вкладки
    ModelObjectProject *tableData;//создадим модель, которую вложим во вью

	QGridLayout *grid_tableDataLink;//создаем грид, который кладем на вкладку
	QTableView *tableViewLink;//создаем вью, который положим на грид вкладки
    ModelLinkData *tableDataLink;//создадим модель, которую вложим во вью

	QPushButton *buttonLoad;
	QPushButton *buttonUpLoad;
	QPushButton *buttonConnect;
	QTextEdit *textEditLogs;
	QGridLayout *gridLayout;
	QTextEdit *txtPath;

	//переменные для подключения к БД
	std::string webserver;
	std::string path;

	QTimer *timer;
    Logger logs{"logsSQL"};
	QString temp;

    QPalette darkPalette;
    std::vector<int> numberOfRows; //перечень колонок для заливки

	void setDataToModel(QString fileName, int currentTab = 0);

public slots:
	void on_buttonLoad_clicked();
	void on_buttonUpLoad_clicked();
	void on_buttonConnect_clicked();
	void slotTimerLogs();
	void closeTab(int index);
    void onRowChangeTable(QModelIndex,QModelIndex);
    void onRowChangeLink(QModelIndex,QModelIndex);
};
