#pragma once

#include "SQL_to_FB.h"
#include "ObjectProject.h"
#include "ObjectProjectData.h"

#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 

#include <iostream>
#include "Windows.h"
#include <string>
#include <any>
#include <vector>
#include <fstream>
#include <conio.h> //для getch

class EES : public QWidget
{
	Q_OBJECT

public:
	EES(QWidget *parent = 0);
	void retranslateUi();
	void create_page(SQL_to_FB &DB_conn, std::string page_name, std::string parent_page, std::string template_page, std::string event_group, int id_subject = 0);
	int initializingConnection(std::string path);

private:
	QSplitter *splitterVertical;
	ObjectProject *tableData;
	QTableView *tableView;
	QPushButton *buttonLoad;
	QPushButton *buttonUpLoad;
	QPushButton *buttonConnect;
	QTextEdit *textEditLogs;
	QGridLayout *gridLayout;
	QTextEdit *txtPath;
	SQL_to_FB dataBase;

	QTimer *timer;
	std::ofstream logs;
	QString temp;

	void setDataToModel(QString fileName);

public slots:
	void on_buttonLoad_clicked();
	void on_buttonUpLoad_clicked();
	void on_buttonConnect_clicked();
	void slotTimerLogs();
	
};
