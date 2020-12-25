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
	//ответственный за определение данных, которые соответствуют в модели определенному индексу и роли
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	//метод setData(), где описывается изменение соответствующих параметров модели
//	bool setData(const QModelIndex& index, const QVariant& value, int nRole = Qt::EditRole);
	//возрат числа строк
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	//возврат числа колонок
	int columnCount(const QModelIndex& = QModelIndex()) const;
	//метод flags(), в котором устанавливаются значения флага, определяющие набор действий с моделью
	Qt::ItemFlags flags(const QModelIndex& index) const;
	//с помощью метода headerData() можно задать заголовки строк и столбцов таблицы отображающей данные
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void addObject(ObjectProjectData& newData);
	void delObject(const QModelIndex & index);
	void delData();
	ObjectProjectData& getObject(const QModelIndex & index) const;

	~ObjectProject();
};

