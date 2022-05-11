#include "ModelLinkData.h"



ModelLinkData::ModelLinkData(QObject* pobj)
{
	listOfParams = new QList<TableLinkData>();
}


ModelLinkData::~ModelLinkData()
{
	delData();
	delete listOfParams;
}

QVariant ModelLinkData::data(const QModelIndex & index, int role) const
{
	if (index.isValid() && role == Qt::DisplayRole)
		return getData(index.row(), index.column());
	return QVariant();
}

int ModelLinkData::rowCount(const QModelIndex & parent) const
{
	return listOfParams->size();
}

int ModelLinkData::columnCount(const QModelIndex &) const
{
	return 8;
}

Qt::ItemFlags ModelLinkData::flags(const QModelIndex & index) const
{
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	return index.isValid()
		? (flags | Qt::ItemIsEditable)
		: flags;
}

QVariant ModelLinkData::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Vertical)
		//тут можно задать вертикальный хедер
		return QVariant(section + 1);
	else
		switch (section) {
		case 0:
			return QVariant(tr("Source Control", "headerLinkData"));
		case 1:
			return QVariant(tr("Source Res", "headerLinkData"));
		case 2:
			return QVariant(tr("Source Mark", "headerLinkData"));
		case 3:
			return QVariant(tr("Source Chanel", "headerLinkData"));
		case 4:
			return QVariant(tr("Receiver Control", "headerLinkData"));
		case 5:
			return QVariant(tr("Receiver Res", "headerLinkData"));
		case 6:
			return QVariant(tr("Receiver Mark", "headerLinkData"));
		case 7:
			return QVariant(tr("Receiver Chanel", "headerLinkData"));
		default:
			return QVariant();
		}
}

void ModelLinkData::addObject(TableLinkData & newData)
{
	beginInsertRows(QModelIndex(), listOfParams->size(), listOfParams->size());
	listOfParams->append(newData);
	endInsertRows();
}

void ModelLinkData::delObject(const QModelIndex & index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	listOfParams->removeAt(index.row());
	endRemoveRows();
}

void ModelLinkData::delData()
{
	for (int k = 0; k < listOfParams->count(); k++)
		listOfParams->removeAt(k);
	listOfParams->clear();
}

TableLinkData & ModelLinkData::getObject(const QModelIndex & index) const
{
	return (*listOfParams)[index.row()];
}

QVariant ModelLinkData::getData(int num, int position) const
{
	//бегаем по колонке (position)

	switch (position) {
	case 0:
		return listOfParams->at(num).getSourceControl();
	case 1:
		return listOfParams->at(num).getSourceRes();
	case 2:
		return listOfParams->at(num).getSourceMark();
	case 3:
		return listOfParams->at(num).getSourceChanel();
	case 4:
		return listOfParams->at(num).getReceiverControl();
	case 5:
		return listOfParams->at(num).getReceiverRes();
	case 6:
		return listOfParams->at(num).getReceiverMark();
	case 7:
		return listOfParams->at(num).getReceiverChanel();

	default:
		return QVariant();
	}
}

