#pragma once

#include <qabstractitemmodel.h>
#include "TableLinkData.h"
#include "ObjectProject.h"

class ModelLinkData: public QAbstractTableModel
{
public:
	ModelLinkData(QObject* pobj = 0);
	~ModelLinkData();

	//ответственный за определение данных, которые соответствуют в модели определенному индексу и роли
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	//возрат числа строк
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	//возврат числа колонок
	int columnCount(const QModelIndex& = QModelIndex()) const;
	//метод flags(), в котором устанавливаются значения флага, определяющие набор действий с моделью
	Qt::ItemFlags flags(const QModelIndex& index) const;
	//с помощью метода headerData() можно задать заголовки строк и столбцов таблицы отображающей данные
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void addObject(TableLinkData& newData);
	void delObject(const QModelIndex & index);
	void delData();
	TableLinkData& getObject(const QModelIndex & index) const;

private:
	QList<TableLinkData> *listOfParams;
	QVariant getData(int num, int position) const;
};

