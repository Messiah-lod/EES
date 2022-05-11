#ifndef MODELFILLINGDB_H
#define MODELFILLINGDB_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QGraphicsProxyWidget>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickPaintedItem>
//#include <qqml.h>


Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
Q_PROPERTY(int width READ getWidgetWidth WRITE setWidgetWidth)
Q_PROPERTY(int height READ getWidgetHeight WRITE setWidgetHeight)


class ModelFillingDB : public QQuickPaintedItem
{
    Q_OBJECT
public:

//    ModelFillingDB(QWidget *parent = nullptr);
    ModelFillingDB(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override; // Переопределяем метод, в котором будет отрисовываться наш объект
    ~ModelFillingDB() override;

  //  QString getText() const {return QString();}
    qreal getWidgetWidth() const;
    qreal getWidgetHeight() const;
//    void setText(const QString& text) {Q_UNUSED(text)}
    void setWidgetWidth(const int width);
    void setWidgetHeight(const int height);

private:
 //   QGridLayout *gridMainWidget;
    QTableView *tableView;
    QStandardItemModel *tableData;
};

#endif // MODELFILLINGDB_H
