#pragma once

#include <qabstractitemmodel.h>
#include "TableLinkData.h"
#include "ObjectProject.h"

class ModelLinkData: public QAbstractTableModel
{
public:
	ModelLinkData(QObject* pobj = 0);
	~ModelLinkData();

	//������������� �� ����������� ������, ������� ������������� � ������ ������������� ������� � ����
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	//������ ����� �����
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	//������� ����� �������
	int columnCount(const QModelIndex& = QModelIndex()) const;
	//����� flags(), � ������� ��������������� �������� �����, ������������ ����� �������� � �������
	Qt::ItemFlags flags(const QModelIndex& index) const;
	//� ������� ������ headerData() ����� ������ ��������� ����� � �������� ������� ������������ ������
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void addObject(TableLinkData& newData);
	void delObject(const QModelIndex & index);
	void delData();
	TableLinkData& getObject(const QModelIndex & index) const;

private:
	QList<TableLinkData> *listOfParams;
	QVariant getData(int num, int position) const;
};

