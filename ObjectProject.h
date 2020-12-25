#pragma once
#include <qabstractitemmodel.h>
#include "ObjectProjectData.h"

class ObjectProject :
	public QAbstractTableModel
{
private:
	QList<ObjectProjectData> *listOfParams;
	QVariant getData(int num, int position) const;

public:
	ObjectProject(QObject* pobj = 0);
	void retranslateUi();
	//������������� �� ����������� ������, ������� ������������� � ������ ������������� ������� � ����
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	//����� setData(), ��� ����������� ��������� ��������������� ���������� ������
//	bool setData(const QModelIndex& index, const QVariant& value, int nRole = Qt::EditRole);
	//������ ����� �����
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	//������� ����� �������
	int columnCount(const QModelIndex& = QModelIndex()) const;
	//����� flags(), � ������� ��������������� �������� �����, ������������ ����� �������� � �������
	Qt::ItemFlags flags(const QModelIndex& index) const;
	//� ������� ������ headerData() ����� ������ ��������� ����� � �������� ������� ������������ ������
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void addObject(ObjectProjectData& newData);
	void delObject(const QModelIndex & index);
	void delData();
	ObjectProjectData& getObject(const QModelIndex & index) const;

	~ObjectProject();
};

