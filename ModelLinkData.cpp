#include "ModelLinkData.h"



ModelLinkData::ModelLinkData(QObject* parent)
{
    listOfParams = new QList<TableLinkData>();
    Q_UNUSED(parent)
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

int ModelLinkData::rowCount(const QModelIndex &) const
{
    return static_cast<int>(listOfParams->size());
}

int ModelLinkData::columnCount(const QModelIndex &) const
{
    int sizeObjectProject = sizeof(TableLinkData) / sizeof(QVariant);
    return sizeObjectProject; //8
//	return 8;
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
    QString nameHeader;
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Vertical)
		//тут можно задать вертикальный хедер
		return QVariant(section + 1);
	else
		switch (section) {
		case 0:
            nameHeader = tr("Source Control");
            return QVariant(nameHeader);
		case 1:
            nameHeader = tr("Source Res");
            return QVariant(nameHeader);
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
    int indexStart = static_cast<int>(listOfParams->size());
    beginInsertRows(QModelIndex(), indexStart, indexStart);
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
        return listOfParams->at(num).SourceControl;
    case 1:
        return listOfParams->at(num).SourceRes;
    case 2:
        return listOfParams->at(num).SourceMark;
    case 3:
        return listOfParams->at(num).SourceChanel;
    case 4:
        return listOfParams->at(num).ReceiverControl;
    case 5:
        return listOfParams->at(num).ReceiverRes;
    case 6:
        return listOfParams->at(num).ReceiverMark;
    case 7:
        return listOfParams->at(num).ReceiverChanel;

    default:
        return QVariant();
    }
}

