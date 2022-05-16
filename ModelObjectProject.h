#pragma once
#include <qabstractitemmodel.h>


struct ObjectProjectData{
public:
    QVariant Mark;
    QVariant Name;
    QVariant Description;
    QVariant ObjectType;
    QVariant Digital; //добавили для цифры
    QVariant Signature;
    QVariant Controller;
    QVariant PlcAdress;//добавили для цифры
    QVariant PlcVarname;//добавили для цифры
    QVariant Resource;
    QVariant EventGroup;
    QVariant KKS;
    QVariant ObjectTemplate;
    QVariant MnemonicFrameName;
    QVariant MnemonicFrameTemplate;
    QVariant MnemonicFrameParent;
    QVariant TechnicalProgramName;
    QVariant TechnicalProgramParent;

    void setData(int numParam, QVariant value)
    {
            switch (numParam) {
        case 0:
            Mark = value; break;
        case 1:
            Name = value; break;
        case 2:
            Description = value; break;
        case 3:
            ObjectType = value; break;
        case 4:
            Digital = value; break;
        case 5:
            Signature = value; break;
        case 6:
            Controller = value; break;
        case 7:
            PlcAdress = value; break;
        case 8:
            PlcVarname = value; break;
        case 9:
            Resource = value; break;
        case 10:
            EventGroup = value; break;
        case 11:
            KKS = value; break;
        case 12:
            ObjectTemplate = value; break;
        case 13:
            MnemonicFrameName = value; break;
        case 14:
            MnemonicFrameTemplate = value; break;
        case 15:
            MnemonicFrameParent = value; break;
        case 16:
            TechnicalProgramName = value; break;
        case 17:
            TechnicalProgramParent = value; break;
        }
    }
};

class ModelObjectProject :
	public QAbstractTableModel
{
public:
    ModelObjectProject(QObject* parent = nullptr);

	//ответственный за определение данных, которые соответствуют в модели определенному индексу и роли
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
	//метод setData(), где описывается изменение соответствующих параметров модели
//    bool setData(const QModelIndex& index, const QVariant& value, int nRole = Qt::EditRole) override;
	//возрат числа строк
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	//возврат числа колонок
    int columnCount(const QModelIndex& = QModelIndex()) const override;
	//метод flags(), в котором устанавливаются значения флага, определяющие набор действий с моделью
    Qt::ItemFlags flags(const QModelIndex& index) const override;
	//с помощью метода headerData() можно задать заголовки строк и столбцов таблицы отображающей данные
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addObject(ObjectProjectData& newData);
	void delObject(const QModelIndex & index);
	void delData();
    ObjectProjectData& getObject(const QModelIndex & index) const;

    ~ModelObjectProject() override;

private:
    QList<ObjectProjectData> *listOfParams;
    QVariant getData(int num, int position) const;

};

