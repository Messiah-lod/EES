#include "EES.h"
//#pragma warning(disable : 4715)

EES::EES(QWidget *parent) : QMainWindow(parent)
{	
    //инициализируем элементы графического интерфейса
    centreWidget = new QWidget(this);
    gridLayout = new QGridLayout(centreWidget);
    splitterVertical = new QSplitter(Qt::Vertical);

    tabWidget = new QTabWidget; //организуем вкладки
    tab_tableData = new QWidget;//вкладка создания
    tab_tableData->setAccessibleName(QString::number(enum_tableData));//добавим номер перечисения к вкладке, для уникального идентифицирования
    tab_tableDataLink = new QWidget;//вкладка связки
    tab_tableDataLink->setAccessibleName(QString::number(enum_tableDataLink));

    tableData = new ModelObjectProject;
    tableView = new QTableView;
    grid_tab_tableData = new QGridLayout(tab_tableData);
    grid_tab_tableData->addWidget(tableView);

    tableDataLink = new ModelLinkData;
    tableViewLink = new QTableView;
    grid_tableDataLink = new QGridLayout(tab_tableDataLink);
    grid_tableDataLink->addWidget(tableViewLink);

    buttonLoad = new QPushButton;
    buttonUpLoad = new QPushButton;
    buttonConnect = new QPushButton;
    buttonSelectAll = new QPushButton;

    buttonAbout = new QPushButton;
    buttonAbout->setIcon(QIcon(":/pic/information.png"));
    buttonAbout->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttonAbout->setFixedSize(QSize(30,30));

    txtPath = new QComboBox;

    textTreeLogs = new QTreeView;
    textTreeLogs->setMinimumHeight(100);
    //Установили запрет на редактирование текста
    textTreeLogs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Инициализируем модель, добавим ее во вью
    textTreeLogsModel = new QStandardItemModel();
    textTreeLogs->setModel(textTreeLogsModel);
    textTreeLogs->setHeaderHidden(true); //скрыли хедер


    txtPath->setMaximumHeight(25);
    buttonConnect->setMaximumHeight(25);
    //свяжем модель таблицы с представлением
    tableView->setModel(tableData);
    tableViewLink->setModel(tableDataLink);
    //тригер запрета редактирования таблицы
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableViewLink->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //разделим основные элементы сплиттером
    splitterVertical->addWidget(tabWidget);
    splitterVertical->addWidget(textTreeLogs);
    //наполним табы
    tabWidget->setMinimumHeight(300);
    tabWidget->tabBar()->setExpanding(false);
    tabWidget->addTab(tab_tableData, QString());
    tabWidget->addTab(tab_tableDataLink, QString());
    //выберем вкладку, которая открывается по умолчанию, счет с нуля
    tabWidget->setCurrentIndex(0);
    tabWidget->setDocumentMode(false);//безрамочный режим таба
    tabWidget->setMovable(true);//метод подвижности вкладок
    //	tabWidget->setTabsClosable(true);//добавит кнопки закрытия вкладок

    //Добавим на грид необходимые элементы
    gridLayout->addWidget(txtPath, 0, 0, 1, 4);
    gridLayout->addWidget(buttonConnect, 0, 4, 1, 1);
    gridLayout->addWidget(buttonLoad, 1,0,1,1);
    gridLayout->addWidget(buttonUpLoad, 1, 1, 1, 1);
    gridLayout->addWidget(buttonSelectAll, 1, 2, 1, 1);
    gridLayout->addWidget(splitterVertical, 2, 0, 1, 5);
    //Связь сигналов кнопок с методами
    QObject::connect(buttonLoad, SIGNAL(clicked()), this, SLOT(on_buttonLoad_clicked()));
    QObject::connect(buttonUpLoad, SIGNAL(clicked()), this, SLOT(on_buttonUpLoad_clicked()));
    QObject::connect(buttonConnect, SIGNAL(clicked()), this, SLOT(on_buttonConnect_clicked()));
    QObject::connect(buttonAbout, SIGNAL(clicked()), this, SLOT(on_buttonAbout_clicked()));
    QObject::connect(buttonSelectAll, SIGNAL(clicked()), this, SLOT(on_buttonSelectAll_clicked()));

    QObject::connect(this, SIGNAL(finishSetDataToModel()), this, SLOT(slotFinishSetDataToModel()));
    QObject::connect(this, SIGNAL(finishFillingToDB()), this, SLOT(slotFinishFillingToDB()));

    aboutToolBar = new QToolBar;
    this->addToolBar(aboutToolBar);
    aboutToolBar->addWidget(buttonAbout);

    //связка для закрытия вкладки или смены
    //	QObject::connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    //    QObject::connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(closeTab(int)));//сигнал при смене вкладке, отдает индекс выбранной вкладки



    //подгоним ширину столбцов под хедеры
    tableView->resizeColumnsToContents();
    tableViewLink->resizeColumnsToContents();

    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);

    //отобразим грид с виджетами
    //   centreWidget->setLayout(gridLayout);
    this->setCentralWidget(centreWidget);


    //механизм для отображения окна "О программе", написанного на QML
    view = new QQuickView();
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/About.qml")); // зададим адрес к ресурсу
    container = QWidget::createWindowContainer(view, this, Qt::Window);
    container->setMinimumSize(600, 400);

    //   container->setMaximumSize(300, 200);
    //   container->setFocusPolicy(Qt::TabFocus);

    //связка для закрытия окна из QML
    QObject::connect(view->engine(), SIGNAL(quit()), this, SLOT(closeAbout()));


    //механизм для считывания из файла настроек всех путей и добавление их в выпадающий список
    for (QString path: mySetting.value("path").toStringList()) listPath << path;
    listPath << tr("Add...");
    txtPath->addItems(listPath);

    //для реализации контекстного меню для выпадающего списка с путями
    QAbstractItemView* view = txtPath->view();
    view->viewport()->installEventFilter(this);
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(list_context_menu(QPoint)));
    QObject::connect(txtPath, SIGNAL(currentIndexChanged(int)), this, SLOT(changePath(int)));
    QObject::connect(txtPath, SIGNAL(highlighted(int)), this, SLOT(selectedPath(int)));
    actEdit = new QAction;
    actEdit->setIcon(QIcon(":/pic/007-edit.png"));
    QObject::connect(actEdit, SIGNAL(triggered()), this, SLOT(on_actionEdit_clicked()));
    actDel = new QAction;
    actDel->setIcon(QIcon(":/pic/036-trash.png"));
    QObject::connect(actDel, SIGNAL(triggered()), this, SLOT(on_actionDel_clicked()));

    //окно для редактирования и добавления путей
    changeLine = new ChangeLine(this);
    changeLine->setWindowFlags(Qt::Window);
    changeLine->setFixedSize(QSize(500, 80));
    QObject::connect(changeLine, SIGNAL(changeLineComplete(QString, int)), this, SLOT(addChangePath(QString, int)));

    //механизм открытия пути, который использовался в последний раз
    QVariant curIndPath = mySetting.value("last_DB");
    txtPath->setCurrentIndex(curIndPath.toInt());

    retranslateUi();
}

EES::~EES()
{
    listPath.clear();
    for(int i = 0; i < txtPath->count()-1; i++) listPath << txtPath->itemText(i);
    mySetting.setValue("path", listPath);
    mySetting.setValue("last_DB", txtPath->currentIndex());
}

void EES::retranslateUi()
{
    setWindowTitle(QApplication::translate("EESClass", "Excel Export Scada v2.1.0", nullptr));
    buttonLoad->setText(QApplication::translate("buttonLoad", "Open file", nullptr));
    buttonUpLoad->setText(QApplication::translate("buttonUpLoad", "Upload", nullptr));
    buttonConnect->setText(QApplication::translate("buttonConnect", "Connect", nullptr));
    buttonSelectAll->setText(QApplication::translate("buttonSelectAll", "Select all", nullptr));
    //    buttonAbout->setText(QApplication::translate("buttonAbout", "About", nullptr));

    tabWidget->setTabText(tabWidget->indexOf(tab_tableData), QApplication::translate("tab_tableData", "Filling", nullptr));
    tabWidget->setTabText(tabWidget->indexOf(tab_tableDataLink), QApplication::translate("tab_tableDataLink", "Link", nullptr));

    actEdit->setText(tr("Edit"));
    actDel->setText(tr("Delete"));
    //   txtPath->setPlaceholderText(QStringLiteral("--Select path--"));

} // retranslateUi

void EES::on_buttonLoad_clicked()
{
    QWidget* widget = tabWidget->currentWidget();//получили текущий выбраный виджет
    QString name = widget->accessibleName();//получили уникальное имя виджета из enum Tab
    if (name.toInt() == enum_tableData) {
        tableData->delData();//добавить очистку модели
    } else if (name.toInt() == enum_tableDataLink) {
        tableDataLink->delData();
    }

    QString fileName;
    try
    {
        fileName = QFileDialog::getOpenFileName(nullptr,
                                                tr("Open file"), QDir::currentPath(), tr("Data Files (*.xls *.xlsx *.xlsm)"));
    }
    catch (const std::exception&)
    {
        fileName = "";
    }

    if (fileName.isEmpty())
    {
        QStandardItem* item = new QStandardItem(QIcon(":/pic/warning.png"), tr("No file chosen!"));
        textTreeLogsModel->appendRow(item);
    }
    else
    {
        QStandardItem* item = new QStandardItem(tr("File open: ") + fileName);
        textTreeLogsModel->appendRow(item);

        //запуск заполнения модели в отдельном потоке
        std::thread t1(&EES::setDataToModel, this, fileName, name.toInt());
        t1.detach();
    }
}

void EES::on_buttonUpLoad_clicked()
{
    if(!on_buttonConnect_clicked()) return;
    QString logString;

    //механизм заливки только выделенных строк
    QModelIndexList indexes;
    indexes = tableView->selectionModel()->selectedIndexes();
    numberOfRows.clear();
    for (QModelIndex index : indexes) numberOfRows.push_back(index.row());
    if(numberOfRows.size() != 0){
        std::sort(numberOfRows.begin(), numberOfRows.end());
        std::vector<int>::iterator it;
        it = std::unique(numberOfRows.begin(), numberOfRows.end());
        numberOfRows.resize(distance(numberOfRows.begin(),it));
    }
    else{
        for (int i = 0; i < tableData->rowCount(); i++) numberOfRows.push_back(i);
    }
    //    for (int i : numberOfRows) qDebug() << QString::number(i);
    //    qDebug() << "Начало выгрузки: " + QString::number(numberOfRows[0]) + " , конец выгрузки: " +
    //            QString::number(numberOfRows[numberOfRows.size()-1]) + " , количество строк для загрузки: " +
    //            QString::number(numberOfRows.size());

    logString = tr("Upload START!", "txt_connect");
    QStandardItem* item = new QStandardItem(logString);
    textTreeLogsModel->appendRow(item);
    logs << logString;

    //запуск заполнения модели в отдельном потоке
    std::thread t1(&EES::fillingToDB, this, numberOfRows);
    t1.detach();
}

bool EES::on_buttonConnect_clicked()
{
    std::string tmp = txtPath->currentText().toStdString();//toPlainText().toStdString();
    QString fileName;

    if(tmp[0] == '\\' || tmp[0] == '/' || std::isdigit(tmp[0]))
        initializingConnection(tmp);
    else{
        webserver = "127.0.0.1";
        path = tmp;
    }

    SQL_to_FB dataBase(webserver, path, "SYSDBA", "masterkey");

    if (dataBase.connected())
    {
        QStandardItem* item = new QStandardItem(QIcon(":/pic/check.png"), tr("Connection successful!"));
        textTreeLogsModel->appendRow(item);
        QStandardItem* itemChild = new QStandardItem(tr("Database connected: ") + txtPath->currentText());
        item->appendRow(itemChild);
        return true;
    }
    else
    {
        QStandardItem* item = new QStandardItem(QIcon(":/pic/warning.png"), tr("Connection failed! Path: ") + txtPath->currentText());
        textTreeLogsModel->appendRow(item);
        logs << (tr("Connection failed!", "txt_connect") + txtPath->currentText());
        return false;
    }
}

void EES::on_buttonAbout_clicked()
{
    qDebug() << "about";
    container->show();
}

void EES::on_buttonSelectAll_clicked()
{
    if (tabWidget->currentIndex() == enum_tableData) {
        tableView->selectAll();
    } else if (tabWidget->currentIndex() == enum_tableDataLink) {
        tableViewLink->selectAll();
    }
}


void EES::closeTab(int index)
{
    //QMessageBox::warning(this, "Active filter", "Close tab index = " + QString::number(index));
    tabWidget->removeTab(index);//сам виджет жив, можно в любой момент вернуть обратно
}

void EES::closeAbout()
{
    container->close();
}

void EES::list_context_menu(QPoint pos)
{
    QAbstractItemView* view = txtPath->view();
    QModelIndex index = view->indexAt(pos);
    if (!index.isValid()) { return; }
    QMenu menu;
    //    QString item = txtPath->model()->data(index, Qt::DisplayRole).toString();


    menu.addAction(actEdit);
    menu.addAction(actDel);
    menu.exec(view->mapToGlobal(pos));
}

void EES::changePath(int index)
{
    if (index == txtPath->count()-1) {
        //  qDebug() << "Выбран пункт добавить";
        //Открыть окно с добавлением пункта в list
        changeLine->show();
    }
}

void EES::selectedPath(int index)
{
    selectedIndexPath = index;
}

void EES::on_actionEdit_clicked()
{
    if (selectedIndexPath == txtPath->count()-1) return;
    //   qDebug() << "Зашли в редактирование элемента: " + txtPath->itemText(selectedIndexPath);

    // Открыть окно с отредактировать выбранный элемент
    changeLine->showLine(txtPath->itemText(selectedIndexPath), selectedIndexPath);
}

void EES::on_actionDel_clicked()
{
    if (selectedIndexPath == txtPath->count()-1) return;
    //    qDebug() << "Зашли в удаление элемента: " + txtPath->itemText(selectedIndexPath);
    txtPath->removeItem(selectedIndexPath);
}

void EES::addChangePath(QString path, int index)
{
    if(path.isEmpty()) return;
    //    qDebug() << "Попали в изменение записи";
    if (index == -1) {
        txtPath->insertItem(txtPath->count()-1,path);//добавили перед Add...
        txtPath->setCurrentIndex(txtPath->count()-2);
    }
    else {
        txtPath->removeItem(index);
        txtPath->insertItem(index,path);
        txtPath->setCurrentIndex(index);
    }
}

void EES::slotFinishSetDataToModel()
{
    //   qDebug() << "Зашли в слот окончания загрузки";
    QMessageBox::information(this, tr("Information"), tr("Data from the file has been uploaded"));
}

void EES::slotFinishFillingToDB()
{
    QMessageBox::information(this, tr("Information"), tr("The data has been uploaded to the database"));
}

int EES::initializingConnection(std::string _path)
{
    webserver.clear();
    path.clear();
    //Запишем WEB-сервер в строку параметра
    for (size_t i = 0; i < _path.size(); i++)
    {
        if (!std::isalpha(_path[i])) webserver.push_back(_path[i]);
        else {
            webserver.push_back(_path[i]);
            break;
        }
    }
    //вырежем web-сервер из параметра "путь"
    try
    {
        _path.erase(0, webserver.size());
    }
    catch (std::out_of_range&)  //oops str is too short!!!
    {
        return 2;
    }
    path = _path;


    //    webserver = "192.168.37.3\\D";
    //    path = "ScadaPr\\ASU_ETO\\Scada 2.8\\Pr_KirGRES_ASU_ETO\\scadabd.gdb";
    //    библиотека сама добавляет двоеточие между сервером и путем

    return 0;
}

void EES::setDataToModel(QString fileName, int currentTab)
{
    //Получили дир. запуска программы
    //QString tmp_s = QApplication::applicationDirPath();
    // получаем указатель на Excel
    QAxObject *mExcel = new QAxObject("Excel.Application", nullptr);
    // на книги
    QAxObject *workbooks = mExcel->querySubObject("Workbooks");
    // на директорию, откуда грузить книг
    QAxObject *workbook = workbooks->querySubObject("Open(const QString&)", fileName);
    // на листы
    QAxObject *mSheets = workbook->querySubObject("Sheets");
    // указываем, какой лист выбрать
    QAxObject *StatSheet = mSheets->querySubObject("Item(int)", 1);

    //Кол-во листов в книге
    //int countSheet = mSheets->property("Count").toInt();
    //Количество используемых на листе строк можно получить с помощью такого кода
    QAxObject* usedRange = StatSheet->querySubObject("UsedRange");
    QAxObject* rows = usedRange->querySubObject("Rows");
    int countRows = rows->property("Count").toInt() - 1;//вычитаем 1, т.к.заголовок не учитываем

    //Соответственно, столбцов:
    QAxObject* usedRange1 = StatSheet->querySubObject("UsedRange");
    QAxObject* columns = usedRange1->querySubObject("Columns");
    Q_UNUSED(columns)
    //	int countCols = columns->property("Count").toInt();
    int countCols = tableData->columnCount();

    //Имея количество строк и столбцов можно заполнить таблицу значениями из документа.Делается это так :
    //	tableData->setColumnCount(countCols);
    //	tableData->setRowCount(countRows);


    ObjectProjectData rowData;
    TableLinkData rowDataLink;


    for (int nRow = 0; nRow < countRows; ++nRow)
    {
        for (int nCol = 1; nCol <= countCols; ++nCol)
        {
            QAxObject* cell = StatSheet->querySubObject("Cells(int,int)", nRow + 2, nCol+1);
            //метод записи в выбранную ячейку
            //			cell->setProperty("Value", QVariant("Some value"));
            //метод чтения
            QVariant value = cell->property("Value");

            //заполним строку данных
            if (currentTab == enum_tableData) {
                rowData.setData(nCol - 1, value);
            } else if (currentTab == enum_tableDataLink) {
                rowDataLink.setData(nCol - 1, value);
            }

            //кончили заполнять
        }
        //запись в модель
        if (currentTab == enum_tableData) {
            tableData->addObject(rowData);
        } else if (currentTab == enum_tableDataLink) {
            tableDataLink->addObject(rowDataLink);
        }
    }

    /*



    //	QTableView treeView1;
    //	treeView1.setModel(modelEvent);
        //тригер запрета редактирования таблицы
    //	treeView1.setEditTriggers(QAbstractItemView::NoEditTriggers);
    //	treeView1.show();

        // получение указателя на ячейку [row][col] ((!)нумерация с единицы)
    //	QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 1);
        // вставка значения переменной data (любой тип, приводимый к QVariant) в полученную ячейку
    //	cell->setProperty("Value", QVariant("Some value"));
    //	mExcel->setProperty("DisplayAlerts", 0);

        //говорим excel что всякие служебные сообщения выводить не надо
    //	mExcel->setProperty("DisplayAlerts", 0);
        //сохраняем наш файл под сгенерированным именем в каталог с программой, хотя никто не    //запрещает в другое место
    //	workbook->dynamicCall("SaveAs (const QString&)", tmp_s);

    //	mExcel->setProperty("DisplayAlerts", 1);

    */

    //зачем-то надо, канонично сразу не закомменитировал, теперь не помню
    workbook->dynamicCall("Close(Boolean)", false);
    //закрываем приложение
    mExcel->dynamicCall("Quit(void)");

    delete(mExcel);

    tableView->resizeColumnsToContents();
    tableViewLink->resizeColumnsToContents();
    //    QMessageBox::information(this, tr("Info"), tr("Data uploaded successfully"));

    emit finishSetDataToModel();
}

void EES::fillingToDB(std::vector<int> rows)
{
    SQL_to_FB dataBase(webserver, path, "SYSDBA", "masterkey");
    QModelIndex index;
    QString logString;
    std::string mark;
    std::string name;
    std::string description;
    std::string objectType;
    std::string digital;//добавлен для цифры
    std::string signature;
    std::string controller;
    std::string plcAdress;//добавлен для цифры
    std::string plcVarname;//добавлен для цифры
    std::string resource;
    std::string eventGroup;
    std::string KKS;
    std::string objectTemplate;
    std::string mnemonicFrameName;
    std::string mnemonicFrameTemplate;
    std::string mnemonicFrameParent;
    std::string technicalProgramName;
    std::string technicalProgramParent;

    time_t start, end;
    time(&start);

    int id_subject = 0;// ID созданного тех. объекта
    int evkl_subject = 0;// ID созданной группы событий
    int id_techprog = 0;// ID созданной тех. программы
    int resultCreateMnemonicFrame = 0; //результат выполнения создания видеокадра
    int resultObjectOnTechnicalProgram = 0; //результат выполнения создания видеокадра

    for (int i : rows)
    {
        Sleep(1);
        index = tableView->model()->index(i, 0);

        mark = tableData->getObject(index).Mark.toString().toStdString();
        if (mark.size() == 0) continue; //если марка пустая, идем на следующую итерацию

        name = tableData->getObject(index).Name.toString().toStdString();
        description = tableData->getObject(index).Description.toString().toStdString();
        objectType = tableData->getObject(index).ObjectType.toString().toStdString();
        digital = tableData->getObject(index).Digital.toString().toStdString();//добавлен для цифры
        signature = tableData->getObject(index).Signature.toString().toStdString();
        controller = tableData->getObject(index).Controller.toString().toStdString();
        plcAdress = tableData->getObject(index).PlcAdress.toString().toStdString();//добавлен для цифры
        plcVarname = tableData->getObject(index).PlcVarname.toString().toStdString();//добавлен для цифры
        resource = tableData->getObject(index).Resource.toString().toStdString();
        eventGroup = tableData->getObject(index).EventGroup.toString().toStdString();
        KKS = tableData->getObject(index).KKS.toString().toStdString();
        objectTemplate = tableData->getObject(index).ObjectTemplate.toString().toStdString();
        mnemonicFrameName = tableData->getObject(index).MnemonicFrameName.toString().toStdString();
        mnemonicFrameTemplate = tableData->getObject(index).MnemonicFrameTemplate.toString().toStdString();
        mnemonicFrameParent = tableData->getObject(index).MnemonicFrameParent.toString().toStdString();
        technicalProgramName = tableData->getObject(index).TechnicalProgramName.toString().toStdString();
        technicalProgramParent = tableData->getObject(index).TechnicalProgramParent.toString().toStdString();

        logString = tr("Transfer line: ", "txt_connect") + QString::number(i+1) + tr(", object: ") + QString::fromStdString(mark);
        QStandardItem* itemRoot = new QStandardItem(QIcon(":/pic/check.png"), logString);
        textTreeLogsModel->appendRow(itemRoot);
        logs << logString;

        logs.debug(QString::fromStdString(mark) + "    "+QString::fromStdString(name) + "    "+ QString::fromStdString(description) + "    "+ QString::fromStdString(objectType) + "    "+ QString::fromStdString(digital) + "    "+
                   QString::fromStdString(signature) + "    "+ QString::fromStdString(controller) + "    "+ QString::fromStdString(plcAdress) + "    "+ QString::fromStdString(plcVarname) + "    "+
                   QString::fromStdString(resource) + "    "+ QString::fromStdString(eventGroup) + "    "+ QString::fromStdString(KKS) + "    "+ QString::fromStdString(objectTemplate) + "    "+
                   QString::fromStdString(mnemonicFrameName) + "    "+ QString::fromStdString(mnemonicFrameTemplate) + "    "+ QString::fromStdString(mnemonicFrameParent) + "    "+ QString::fromStdString(technicalProgramName) + "    "+
                   QString::fromStdString(technicalProgramParent));

        //начинаем создавать объекты если марка не пустая
        try	{
            if(eventGroup.size() != 0) {
                evkl_subject = dataBase.create_new_event(eventGroup, mnemonicFrameParent);
                QStandardItem* itemEvent = new QStandardItem(QIcon(":/pic/check.png"), "");
                if(evkl_subject == 2){
                    itemEvent->setText("Наименование группы событий пустое");
                    itemEvent->setIcon(QIcon(":/pic/warning.png"));
                    itemRoot->setIcon(QIcon(":/pic/warning.png"));
                }
                itemEvent->setText("Группа событий создана");
                itemRoot->appendRow(itemEvent);
                qDebug() << "Создали группу событий с ID: " + QString::number(evkl_subject);
            }

            id_subject = 0; //обнуляем ID на каждом цикле

            if(name.size() != 0) {
                QStandardItem* itemSubject = new QStandardItem(QIcon(":/pic/warning.png"), "");

                if (digital == "драйвер"){
                    qDebug() << "Создаем драйвер";
                    id_subject = dataBase.create_new_driver(mark, name, description, signature, objectType, controller, KKS, objectTemplate);

                    switch (id_subject){
                    case 1: itemSubject->setText("Драйвер не был создан! Ошибка в определении провайдера"); break;
                    case 2: itemSubject->setText("Драйвер не был создан! Ошибка в определении типа исполнения драйвера (локальный/внутренний)"); break;
                    case 3: itemSubject->setText("Драйвер не был создан! Ошибка в марке либо марка не уникальна"); break;
                    case 4: itemSubject->setText("Драйвер не был создан! Ошибка в определении типа драйвера"); break;
                    case 10: itemSubject->setText("Марка драйвера не уникальна! Поля объекта были обновлены");
                        itemSubject->setIcon(QIcon(":/pic/check.png"));
                        break;
                    default: itemSubject->setText("Драйвер успешно создан");
                        itemSubject->setIcon(QIcon(":/pic/check.png"));
                        break;
                    }

                }
                else if (digital == "цифровой"){
                    qDebug() << "Создаем цифровую переменную";
                    id_subject = dataBase.create_new_digital_object(controller, objectType, mark, name, plcAdress,
                                                                    plcVarname, resource, eventGroup, description, KKS, signature);

                    switch (id_subject){
                    case 1: itemSubject->setText("Переменная не создана! Ошибка в определении контроллера"); break;
                    case 2: itemSubject->setText("Переменная не создана! Ошибка в определении типа переменной"); break;
                    case 3: itemSubject->setText("Переменная не создана! KKS не уникален"); break;
                    case 4: itemSubject->setText("Переменная не создана! Ошибка в марке объекта"); break;
                    case 10: itemSubject->setText("Марка переменной не уникальна! Поля объекта были обновлены");
                        itemSubject->setIcon(QIcon(":/pic/check.png"));
                        break;
                    default: itemSubject->setText("Цифровая переменная успешно создана");
                        itemSubject->setIcon(QIcon(":/pic/check.png"));
                        break;
                    }

                }
                else{
                    qDebug() << "Создаем isa объект";
                    id_subject = dataBase.create_new_object(controller, resource, objectTemplate, objectType, mark, name, eventGroup, description, KKS, signature);

                    switch (id_subject){
                    case 1: itemSubject->setText("Объект не создан! Ошибка в определении контроллера или ресурса"); break;
                    case 2: itemSubject->setText("Объект не создан! Ошибка в определении типа объекта или шаблона"); break;
                    case 3: itemSubject->setText("Объект не создан! KKS не уникален"); break;
                    case 10: itemSubject->setText("Марка объекта не уникальна! Поля объекта были обновлены");
                        itemSubject->setIcon(QIcon(":/pic/check.png"));
                        break;
                    default: itemSubject->setText("Объект успешно создан");
                        itemSubject->setIcon(QIcon(":/pic/check.png"));
                        break;
                    }
                }

                itemRoot->appendRow(itemSubject);
            }

            if(id_subject < 9) {  //выходим из цикла если при создании объекта была ошибка
                itemRoot->setIcon(QIcon(":/pic/warning.png"));
                continue;
            }


            //создаем мнемокадр
            if(mnemonicFrameName.size() != 0) {
                resultCreateMnemonicFrame = dataBase.copy_page_with_object(mnemonicFrameTemplate, mnemonicFrameName, mnemonicFrameParent,
                                                                           std::to_string(id_subject), eventGroup, description);

                QStandardItem* itemMnemonic = new QStandardItem(QIcon(":/pic/warning.png"), "");
                itemRoot->appendRow(itemMnemonic);
                switch (resultCreateMnemonicFrame){
                case 1: itemMnemonic->setText("Кадр не создан! Имя не уникально"); break;
                case 2: itemMnemonic->setText("Кадр не создан! Шаблон для копирования кадра не найден"); break;
                case 3: itemMnemonic->setText("Кадр не создан! Пустое имя"); break;
                default: itemMnemonic->setText("Кадр успешно создан");
                    itemMnemonic->setIcon(QIcon(":/pic/check.png"));
                    break;
                }
            }

            //создаем тех. программу, кладем туда созданный объект
            if(technicalProgramName.size() != 0) {
                id_techprog = dataBase.create_new_technical_programm(technicalProgramName, 300, 200, technicalProgramParent, controller, resource);

                QStandardItem* itemTechprog = new QStandardItem(QIcon(":/pic/warning.png"), "");
                itemRoot->appendRow(itemTechprog);
                switch (id_techprog){
                case 1: itemTechprog->setText("Тех. программа уже существует");
                    itemTechprog->setIcon(QIcon(":/pic/check.png"));
                    break;
                case 4: itemTechprog->setText("Тех. программа не создана! Ресурс или контроллер не найден"); break;
                default: itemTechprog->setText("Кадр успешно создан");
                    itemTechprog->setIcon(QIcon(":/pic/check.png"));
                    break;
                }

                resultObjectOnTechnicalProgram = dataBase.object_on_technological_program(technicalProgramName, controller, resource, objectTemplate, objectType, mark);

                QStandardItem* itemOnTechprog = new QStandardItem(QIcon(":/pic/warning.png"), "");
                itemRoot->appendRow(itemOnTechprog);
                switch (resultObjectOnTechnicalProgram){
                case 1: itemOnTechprog->setText("Объект не помещен на тех. программу! Ошибка в определении шаблона объекта"); break;
                case 2: itemOnTechprog->setText("Объект не помещен на тех. программу! Ошибка в определении тех. программы"); break;
                case 3: itemOnTechprog->setText("Объект не помещен на тех. программу! Ошибка в определении объекта (не найден)"); break;
                case 4: itemOnTechprog->setText("Объект не помещен на тех. программу! Объект уже ранее был привязан"); break;
                default: itemOnTechprog->setText("Объект успешно размещен");
                    itemOnTechprog->setIcon(QIcon(":/pic/check.png"));
                    break;
                }

            }

        }
        catch (const std::exception&){
            logString = tr("Unhandled exception when creating an object");
            QStandardItem* item = new QStandardItem(QIcon(":/pic/check.png"), logString);
            itemRoot->appendRow(item);
            itemRoot->setIcon(QIcon(":/pic/warning.png"));
            logs << logString;
        }
    }

    time(&end);
    double seconds = difftime(end, start);
    logs.debug("Время выполнения равно: " + QString::number(seconds) + " секунд");

    emit finishFillingToDB();
    //    textEditLogs->append(tr("Uploading completed!", "txt_connect"));
}

bool EES::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o)
    if (e->type() == QEvent::MouseButtonRelease) {
        if (static_cast<QMouseEvent*>(e)->button() == Qt::RightButton) {
            return true;
        }
    }
    return false;
}
