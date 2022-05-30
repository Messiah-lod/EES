#include "EES.h"
//#pragma warning(disable : 4715)

EES::EES(QWidget *parent)
	: QWidget(parent)
{
	
	//инициализируем элементы графического интерфейса
	gridLayout = new QGridLayout;
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
	textEditLogs = new QTextEdit;
	txtPath = new QTextEdit;

	textEditLogs->setMinimumHeight(100);
	//Установили запрет на редактирование текста
	textEditLogs->setReadOnly(true);

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
	splitterVertical->addWidget(textEditLogs);
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
	gridLayout->addWidget(splitterVertical, 2, 0, 1, 5);
	//Связь сигналов кнопок с методами
	QObject::connect(buttonLoad, SIGNAL(clicked()), this, SLOT(on_buttonLoad_clicked()));
	QObject::connect(buttonUpLoad, SIGNAL(clicked()), this, SLOT(on_buttonUpLoad_clicked()));
	QObject::connect(buttonConnect, SIGNAL(clicked()), this, SLOT(on_buttonConnect_clicked()));
    QObject::connect(tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onRowChangeTable(QModelIndex,QModelIndex)), Qt::QueuedConnection);
    QObject::connect(tableViewLink->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onRowChangeLink(QModelIndex,QModelIndex)), Qt::QueuedConnection);

    //связка для закрытия вкладки или смены
//	QObject::connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
//    QObject::connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(closeTab(int)));//сигнал при смене вкладке, отдает индекс выбранной вкладки

	retranslateUi();
	//отобразим грид с виджетами
	setLayout(gridLayout);

    //создадим таймер для ежесекундного вычитывания данных из логов
	timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerLogs()));

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

}

void EES::retranslateUi()
{
    setWindowTitle(QApplication::translate("EESClass", "Excel Export Scada v2", nullptr));
	buttonLoad->setText(QApplication::translate("buttonLoad", "Open file", nullptr));
	buttonUpLoad->setText(QApplication::translate("buttonUpLoad", "Upload", nullptr));
	buttonConnect->setText(QApplication::translate("buttonConnect", "Connect", nullptr));

	tabWidget->setTabText(tabWidget->indexOf(tab_tableData), QApplication::translate("tab_tableData", "Filling", nullptr));
	tabWidget->setTabText(tabWidget->indexOf(tab_tableDataLink), QApplication::translate("tab_tableDataLink", "Link", nullptr));

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
		textEditLogs->append(tr("No file chosen!", "txt_connect"));
	}
	else
	{
		textEditLogs->append((tr("File open: ", "txt_connect")
			+ fileName));

        //запуск заполнения модели в отдельном потоке
        std::thread t1(&EES::setDataToModel, this, fileName, name.toInt());
        t1.detach();
  //      setDataToModel(fileName, name.toInt());
	}
//    QMessageBox::information(this, tr("Info"), tr("Data uploaded successfully"));
}

void EES::on_buttonUpLoad_clicked()
{
    if(!on_buttonConnect_clicked()) return;
    QString logString;
    int pointStart = 0, pointEnd = 0;

    //механизм заливки только выделенных строк
    if(numberOfRows.size() != 0){
        std::sort(numberOfRows.begin(), numberOfRows.end());
        std::vector<int>::iterator it;
        it = std::unique(numberOfRows.begin(), numberOfRows.end());
        numberOfRows.resize(distance(numberOfRows.begin(),it));
        for (int i = 0; i < numberOfRows.size(); ++i) {
            logs.debug(QString::number(numberOfRows[i]));
        }

        pointStart = numberOfRows[0];
        pointEnd = numberOfRows[numberOfRows.size()-1]+1;
    }
    else{
        pointStart = 0;
        pointEnd = tableData->rowCount();
    }
    logs<< "Начало выгрузки: " + QString::number(pointStart) + " , конец выгрузки: " + QString::number(pointEnd) + " , количество строк для загрузки: " + QString::number(pointEnd-pointStart);

    logString = tr("Upload START!", "txt_connect");
    textEditLogs->append(logString);
    logs << logString;

    //запуск заполнения модели в отдельном потоке
    std::thread t1(&EES::fillingToDB, this, pointStart, pointEnd);
    t1.detach();

//    fillingToDB(pointStart, pointEnd);
}

bool EES::on_buttonConnect_clicked()
{
    std::string tmp = txtPath->toPlainText().toStdString();
	QString fileName;

	//процедура для открытия диалогового окна проводника для выбора файла,
	//если пользователь сам не ввел путь
	if (tmp.size() == 0)
	{
			fileName = QFileDialog::getOpenFileName(this,
				tr("Open file"), "", tr("Data base (*.gdb)"));
			tmp = fileName.toLocal8Bit();
			txtPath->setText(fileName);

			if (fileName.isEmpty()) textEditLogs->append(tr("No data base chosen!", "txt_connect"));
	}
	
	initializingConnection(tmp);
    SQL_to_FB dataBase(webserver, path, "SYSDBA", "masterkey");

    if (dataBase.connected())
        {
            textEditLogs->append(tr("Connection successful!", "txt_connect"));
            textEditLogs->append(tr("Database connected: ", "txt_connect")
                + txtPath->toPlainText());
            return true;
        }
    else
        {
            textEditLogs->append(tr("Connection failed!", "txt_connect"));
            logs << (tr("Connection failed!", "txt_connect") + txtPath->toPlainText());
            return false;
        }
}

void EES::slotTimerLogs()
{
	//реализовать метод добавления только новых строк из лога
//	file.setFileName("./logs/logsQT.txt");
//	if (file.open(QIODevice::ReadOnly))
//	{
//		temp = file.readAll();
//		textEditLogs->append(temp);
//		file.close();
//	}
}

void EES::closeTab(int index)
{
	//QMessageBox::warning(this, "Active filter", "Close tab index = " + QString::number(index));
    tabWidget->removeTab(index);//сам виджет жив, можно в любой момент вернуть обратно
}

void EES::onRowChangeTable(QModelIndex, QModelIndex)
{
    numberOfRows.clear();
    int numberRow;
    QModelIndexList cntSelRow1 = tableView->selectionModel()->selectedIndexes();
    for (int var = 0; var < cntSelRow1.size(); ++var) {
        numberRow = cntSelRow1[var].row();
        numberOfRows.push_back(numberRow);
    }
}

void EES::onRowChangeLink(QModelIndex, QModelIndex)
{
    numberOfRows.clear();
    int numberRow;
    QModelIndexList cntSelRow1 = tableViewLink->selectionModel()->selectedIndexes();
    for (int var = 0; var < cntSelRow1.size(); ++var) {
        numberRow = cntSelRow1[var].row();
        numberOfRows.push_back(numberRow);
    }
}

int EES::initializingConnection(std::string _path)
{
	webserver.clear();
	path.clear();
	//Запишем WEB-сервер в строку параметра
	for (size_t i = 0; i < _path.size(); i++)
	{
		if (_path[i] != ':') webserver.push_back(_path[i]);
		else break;
	}
	//вырежем web-сервер из параметра "путь"
	try
	{
		_path.erase(0, webserver.size() + 1);
	}
	catch (std::out_of_range&)  //oops str is too short!!! 
	{
		return 2;
	}
	path = _path;	
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
}

void EES::fillingToDB(int pointStart, int pointEnd)
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
    bool isISAObject = false;

    for (int i = pointStart; i < pointEnd; i++)
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

        logString = tr("Transfer line: ", "txt_connect") + QString::number(i) + tr(", object: ") + QString::fromStdString(mark);
        textEditLogs->append(logString);
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
                if(evkl_subject == 1 || evkl_subject == 2) logs << "Ошибка при создании группы собыйти";
                qDebug() << "Создали группу событий с ID: " + QString::number(evkl_subject);
            }
            id_subject = 0; //обнуляем ID на каждом цикле

            if(name.size() != 0) {
                if (digital == "драйвер"){
                    qDebug() << "Создаем драйвер";
                    isISAObject = false;
                    id_subject = dataBase.create_new_driver(mark, name, description, signature, objectType, controller, KKS, objectTemplate);
                }
                else if (digital == "цифровой"){
                    qDebug() << "Создаем цифровую переменную";
                    isISAObject = false;
                    id_subject = dataBase.create_new_digital_object(controller, objectType, mark, name, plcAdress,
                                                                    plcVarname, resource, eventGroup, description, KKS, signature);
                }
                else{
                    qDebug() << "Создаем isa объект";
                    id_subject = dataBase.create_new_object(controller, resource, objectTemplate, objectType, mark, name, eventGroup, description, KKS, signature);
                    if(id_subject != 0) isISAObject = true;
                }
            }

            if(id_subject != 0) {
                //это вписать в шапку дерева в textEditLogs (treeView)
                logString = tr("Create new or update ") + QString::fromStdString(digital) + ", id = " + QString::number(id_subject);
                textEditLogs->append(logString);
                logs << logString;

                //создаем мнемокадр
                if(mnemonicFrameName.size() != 0)
                    resultCreateMnemonicFrame = dataBase.copy_page_with_object(mnemonicFrameTemplate, mnemonicFrameName, mnemonicFrameParent,
                                                                               std::to_string(id_subject), eventGroup, description);
                //создаем тех. программу, кладем туда созданный объект
                if(technicalProgramName.size() != 0) {
                    id_techprog = dataBase.create_new_technical_programm(technicalProgramName, 300, 200, technicalProgramParent, controller, resource);
                    resultObjectOnTechnicalProgram = dataBase.object_on_technological_program(technicalProgramName, controller, resource, objectTemplate, objectType, mark);
                }
            }
        }
        catch (const std::exception&){
            logString = tr("Unhandled exception when creating an object");
            textEditLogs->append(logString);
            logs << logString;
        }
    }

    time(&end);
    double seconds = difftime(end, start);
    logs.debug("Время выполнения равно: " + QString::number(seconds) + " секунд");
    textEditLogs->append(tr("Uploading completed!", "txt_connect"));
}
