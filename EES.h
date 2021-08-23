#pragma once

#include "SQL_to_FB.h"
#include "BaseSQL.h"
#include "ObjectProject.h"
#include "ModelLinkData.h"
#include "Logger.h"

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
#include <conio.h> //��� getch

class EES : public QWidget
{
	Q_OBJECT

public:
	EES(QWidget *parent = 0);
	void retranslateUi();
	int initializingConnection(std::string path);

private:

	QSplitter *splitterVertical;

	QTabWidget *tabWidget; //���������� �������
	QWidget *tab_tableData;//������� ��������
	QWidget *tab_tableDataLink;//������� ������

	enum Tab {//������� ������������ ��� ����������� ����������������� �������
		enum_tableData = 0,
		enum_tableDataLink
	};
	
	QGridLayout *grid_tab_tableData; //������� ����, ������� ������ �� �������
	QTableView *tableView;//������� ���, ������� ������� �� ���� �������
	ObjectProject *tableData;//�������� ������, ������� ������ �� ���

	QGridLayout *grid_tableDataLink;//������� ����, ������� ������ �� �������
	QTableView *tableViewLink;//������� ���, ������� ������� �� ���� �������
	ModelLinkData *tableDataLink;//�������� ������, ������� ������ �� ���

	QPushButton *buttonLoad;
	QPushButton *buttonUpLoad;
	QPushButton *buttonConnect;
	QTextEdit *textEditLogs;
	QGridLayout *gridLayout;
	QTextEdit *txtPath;

	//���������� ��� ����������� � ��
	std::string webserver;
	std::string path;

	QTimer *timer;
	Logger logs{"logsQT"};
	QString temp;

	void setDataToModel(QString fileName, int currentTab = 0);

public slots:
	void on_buttonLoad_clicked();
	void on_buttonUpLoad_clicked();
	void on_buttonConnect_clicked();
	void slotTimerLogs();
	void closeTab(int index);
};
