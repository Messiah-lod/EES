#ifndef CHANGELINE_HPP
#define CHANGELINE_HPP


#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <QtGui>
#include <QPushButton>
#include <QLineEdit>


class ChangeLine : public QWidget
{
    Q_OBJECT

public:
    ChangeLine(QWidget *parent = nullptr)  : QWidget(parent)
    {
        QGridLayout *mainGrid = new QGridLayout(this);
        line = new QLineEdit;
        getPath = new QPushButton;
        getPath->setText("...");
        getPath->setMaximumWidth(50);
        btnOk = new QPushButton;
        mainGrid->addWidget(line, 0,0,1,3);
        mainGrid->addWidget(getPath, 0,3,1,1);
        mainGrid->addWidget(btnOk, 0,4,1,1);

        QObject::connect(getPath, SIGNAL(clicked()), this, SLOT(on_getPath_clicked()));
        QObject::connect(btnOk, SIGNAL(clicked()), this, SLOT(on_btnOk_clicked()));

        this->setLayout(mainGrid);
        retranslateUi();
    }

    void showLine(QString path, int currLine){
        currentLine = currLine;
        line->setText(path);
        this->show();
    }

    void retranslateUi(){
        btnOk->setText(tr("OK"));
        this->setWindowTitle(tr("Path to DB"));
    }


private:
    QLineEdit *line {nullptr};
    QPushButton *getPath {nullptr};
    QPushButton *btnOk {nullptr};
    int currentLine = -1;

private slots:
    void on_btnOk_clicked(){
        emit changeLineComplete(line->text(), currentLine);
        currentLine = -1;
        line->clear();
        this->close();
    }


    void on_getPath_clicked(){
        QString fileName;
        try
        {
            fileName = QFileDialog::getOpenFileName(this,
                            tr("Open file"), "", tr("Data base (*.gdb)"));
        }
        catch (const std::exception&) { fileName = ""; }
        line->setText(fileName);
    }

signals:
    void changeLineComplete(QString, int);

};




#endif // CHANGELINE_HPP
