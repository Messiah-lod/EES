#include "modelfillingdb.h"

//ModelFillingDB::ModelFillingDB(QWidget *parent)
//    : QWidget(parent)

ModelFillingDB::ModelFillingDB(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    tableView = new QTableView;
    tableData = new QStandardItemModel;

    tableData->setHorizontalHeaderLabels(QStringList() << "");
    QStandardItem *items = new QStandardItem;
    items->setData("Hi!");
    tableData->appendRow(items);

    tableView->setModel(tableData);

    this->setEnabled(true);
    this->setVisible(true);
    this->setFocus(true);
    this->setFlag(ItemHasContents); //означает, что наш QQuickItem имеет контент, который необходимо отрисовывать
}

void ModelFillingDB::paint(QPainter *painter)
{
    tableView->render(painter);
}


ModelFillingDB::~ModelFillingDB()
{
    delete tableView;
    delete tableData;
}

qreal ModelFillingDB::getWidgetWidth() const
{
    return this->width();
}

qreal ModelFillingDB::getWidgetHeight() const
{
    return this->height();
}

void ModelFillingDB::setWidgetWidth(const int width)
{
    this->setWidth(width);
}

void ModelFillingDB::setWidgetHeight(const int height)
{
    this->setHeight(height);
}
