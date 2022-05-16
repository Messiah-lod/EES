#include "ModelObjectProject.h"

ModelObjectProject::ModelObjectProject(QObject* parent)
{
    Q_UNUSED(parent)
    listOfParams = new QList<ObjectProjectData>();
}

//метод data вызывается автоматически и не требует явного вызова, ровно как и rowCount с columnCount
//считаем эти методы параметрами таблицы

QVariant ModelObjectProject::data(const QModelIndex & index, int role) const
{
	if (index.isValid() && role == Qt::DisplayRole)
		return getData(index.row(), index.column());
	return QVariant();
}
/*
bool ObjectProject::setData(const QModelIndex & index, const QVariant & value, int nRole)
{
	if (index.isValid() && nRole == Qt::EditRole)
	{
		m_hash[index] = value;
		emit dataChanged(index, index);
		return true;
	}
	return false;
}
*/
int ModelObjectProject::rowCount(const QModelIndex &) const
{
    return static_cast<int>(listOfParams->size());
}

int ModelObjectProject::columnCount(const QModelIndex &) const
{
    int sizeObjectProject = sizeof(ObjectProjectData) / sizeof(QVariant);
    return sizeObjectProject; //18
//    return 18;
}

Qt::ItemFlags ModelObjectProject::flags(const QModelIndex & index) const
{
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	return index.isValid()
		? (flags | Qt::ItemIsEditable)
		: flags;
//	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ModelObjectProject::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Vertical)
		//тут можно задать вертикальный хедер
		return QVariant(section + 1);
	else 
		switch (section) {
		case 0:
			return QVariant(tr("Mark", "headerDataObjectProject"));
		case 1:
			return QVariant(tr("Name", "headerDataObjectProject"));
		case 2:
			return QVariant(tr("Description", "headerDataObjectProject"));
		case 3:
			return QVariant(tr("Object Type", "headerDataObjectProject"));
		case 4:
			return QVariant(tr("Digital", "headerDataObjectProject"));//добавили для цифры
		case 5:
			return QVariant(tr("Signature", "headerDataObjectProject"));
		case 6:
			return QVariant(tr("Controller", "headerDataObjectProject"));
		case 7:
			return QVariant(tr("PlcAdress", "headerDataObjectProject"));//добавили для цифры
		case 8:
			return QVariant(tr("PlcVarname", "headerDataObjectProject"));//добавили для цифры
		case 9:
			return QVariant(tr("Resource", "headerDataObjectProject"));
		case 10:
			return QVariant(tr("Group events", "headerDataObjectProject"));
		case 11:
			return QVariant(tr("KKS", "headerDataObjectProject"));
		case 12:
			return QVariant(tr("Template object", "headerDataObjectProject"));
		case 13:
			return QVariant(tr("Mnemonic Frame Name", "headerDataObjectProject"));
		case 14:
			return QVariant(tr("Mnemonic Frame Template", "headerDataObjectProject"));
		case 15:
			return QVariant(tr("Mnemonic Frame Parent", "headerDataObjectProject"));
		case 16:
			return QVariant(tr("Technical Program Name", "headerDataObjectProject"));
		case 17:
			return QVariant(tr("Technical Program Parent", "headerDataObjectProject"));
		default:
			return QVariant();
		}
}

QVariant ModelObjectProject::getData(int num, int position) const {
	//бегаем по колонке (position)

    switch (position) {
    case 0:
        return listOfParams->at(num).Mark;
    case 1:
        return listOfParams->at(num).Name;
    case 2:
        return listOfParams->at(num).Description;
    case 3:
        return listOfParams->at(num).ObjectType;
    case 4:
        return listOfParams->at(num).Digital;//добавили для цифры
    case 5:
        return listOfParams->at(num).Signature;
    case 6:
        return listOfParams->at(num).Controller;
    case 7:
        return listOfParams->at(num).PlcAdress;//добавили для цифры
    case 8:
        return listOfParams->at(num).PlcVarname;//добавили для цифры
    case 9:
        return listOfParams->at(num).Resource;
    case 10:
        return listOfParams->at(num).EventGroup;
    case 11:
        return listOfParams->at(num).KKS;
    case 12:
        return listOfParams->at(num).ObjectTemplate;
    case 13:
        return listOfParams->at(num).MnemonicFrameName;
    case 14:
        return listOfParams->at(num).MnemonicFrameTemplate;
    case 15:
        return listOfParams->at(num).MnemonicFrameParent;
    case 16:
        return listOfParams->at(num).TechnicalProgramName;
    case 17:
        return listOfParams->at(num).TechnicalProgramParent;
    default:
        return QVariant();
    }
}

ModelObjectProject::~ModelObjectProject()
{
	delData();
    delete listOfParams;
}

ObjectProjectData& ModelObjectProject::getObject(const QModelIndex & index) const
{
    return (*listOfParams)[index.row()];
}

void ModelObjectProject::addObject(ObjectProjectData& newData)
{
    int indexStart = static_cast<int>(listOfParams->size());
    beginInsertRows(QModelIndex(), indexStart, indexStart);
    listOfParams->append(newData);
    endInsertRows();
}

void ModelObjectProject::delObject(const QModelIndex & index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
    listOfParams->removeAt(index.row());
	endRemoveRows();
}

void ModelObjectProject::delData()
{
    for (int k = 0; k < listOfParams->count(); k++)
        listOfParams->removeAt(k);
    listOfParams->clear();
}
