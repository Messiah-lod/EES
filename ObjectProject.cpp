#include "ObjectProject.h"

ObjectProject::ObjectProject(QObject* pobj)
{
	listOfParams = new QList<ObjectProjectData>();
}

void ObjectProject::retranslateUi()
{

}

//метод data вызывается автоматически и не требует явного вызова, ровно как и rowCount с columnCount
//считаем эти методы параметрами таблицы

QVariant ObjectProject::data(const QModelIndex & index, int role) const
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
int ObjectProject::rowCount(const QModelIndex &) const
{
	return listOfParams->size();
}

int ObjectProject::columnCount(const QModelIndex &) const
{
	//return m_nColumns;
	return 18;
}

Qt::ItemFlags ObjectProject::flags(const QModelIndex & index) const
{
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	return index.isValid()
		? (flags | Qt::ItemIsEditable)
		: flags;
//	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ObjectProject::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant ObjectProject::getData(int num, int position) const {
	//бегаем по колонке (position)

	switch (position) {
	case 0:
		return listOfParams->at(num).getMark();
	case 1:
		return listOfParams->at(num).getName();
	case 2:
		return listOfParams->at(num).getDescription();
	case 3:
		return listOfParams->at(num).getObjectType();
	case 4:
		return listOfParams->at(num).getDigital();//добавили для цифры
	case 5:
		return listOfParams->at(num).getSignature();
	case 6:
		return listOfParams->at(num).getController();
	case 7:
		return listOfParams->at(num).getPlcAdress();//добавили для цифры
	case 8:
		return listOfParams->at(num).getPlcVarname();//добавили для цифры
	case 9:
		return listOfParams->at(num).getResource();
	case 10:
		return listOfParams->at(num).getEventGroup();
	case 11:
		return listOfParams->at(num).getKKS();
	case 12:
		return listOfParams->at(num).getObjectTemplate();
	case 13:
		return listOfParams->at(num).getMnemonicFrameName();
	case 14:
		return listOfParams->at(num).getMnemonicFrameTemplate();
	case 15:
		return listOfParams->at(num).getMnemonicFrameParent();
	case 16:
		return listOfParams->at(num).getTechnicalProgramName();
	case 17:
		return listOfParams->at(num).getTechnicalProgramParent();
	default:
		return QVariant();
	}
}

ObjectProject::~ObjectProject()
{
	delData();
	delete listOfParams;
}

ObjectProjectData& ObjectProject::getObject(const QModelIndex & index) const
{
	return (*listOfParams)[index.row()];
}

void ObjectProject::addObject(ObjectProjectData& newData)
{
	beginInsertRows(QModelIndex(), listOfParams->size(), listOfParams->size());
	listOfParams->append(newData);
	endInsertRows();
}

void ObjectProject::delObject(const QModelIndex & index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	listOfParams->removeAt(index.row());
	endRemoveRows();
}

void ObjectProject::delData()
{
	for (int k = 0; k < listOfParams->count(); k++)
		listOfParams->removeAt(k);
	listOfParams->clear();
}
