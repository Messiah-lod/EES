#pragma once

#include <qabstractitemmodel.h>

struct TableLinkData{
    QVariant SourceControl;
    QVariant SourceRes;
    QVariant SourceMark;
    QVariant SourceChanel;

    QVariant ReceiverControl;
    QVariant ReceiverRes;
    QVariant ReceiverMark;
    QVariant ReceiverChanel;

    void setData(int numParam, QVariant value)
    {
        switch (numParam) {
        case 0:
            SourceControl = value; break;
        case 1:
            SourceRes = value; break;
        case 2:
            SourceMark = value; break;
        case 3:
            SourceChanel = value; break;
        case 4:
            ReceiverControl = value; break;
        case 5:
            ReceiverRes = value; break;
        case 6:
            ReceiverMark = value; break;
        case 7:
            ReceiverChanel = value; break;
        }
    }
};

class ModelLinkData: public QAbstractTableModel
{
public:
    ModelLinkData(QObject* parent = nullptr);
    ~ModelLinkData() override;

	//ответственный за определение данных, которые соответствуют в модели определенному индексу и роли
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
	//возрат числа строк
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	//возврат числа колонок
    int columnCount(const QModelIndex& = QModelIndex()) const override;
	//метод flags(), в котором устанавливаются значения флага, определяющие набор действий с моделью
    Qt::ItemFlags flags(const QModelIndex& index) const override;
	//с помощью метода headerData() можно задать заголовки строк и столбцов таблицы отображающей данные
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addObject(TableLinkData& newData);
	void delObject(const QModelIndex & index);
	void delData();
    TableLinkData& getObject(const QModelIndex & index) const;

private:
    QList<TableLinkData> *listOfParams;
	QVariant getData(int num, int position) const;
};

