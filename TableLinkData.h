#pragma once

#include <QString>
#include <QDate> 
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <QtGui>

class TableLinkData
{
public:
	TableLinkData();
	~TableLinkData();


	QVariant getSourceControl() const;
	QVariant getSourceRes() const;
	QVariant getSourceMark() const;
	QVariant getSourceChanel() const;

	QVariant getReceiverControl() const;
	QVariant getReceiverRes() const;
	QVariant getReceiverMark() const;
	QVariant getReceiverChanel() const;

	void setData(int numParam, QVariant value);

private:
	QVariant data[8];
};

