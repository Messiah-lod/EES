#include "EES.h"
#pragma warning(disable : 4715)

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

	tableData = new ObjectProject;
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
	//связка для закрытия вкладки или смены
//	QObject::connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
//	QObject::connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(closeTab(int)));//сигнал при смене вкладке, отдает индекс выбранной вкладки

	retranslateUi();
	//отобразим грид с виджетами
	setLayout(gridLayout);

	logs.open("./logs/logsQT.txt", std::ios_base::app);//открываем(создаем) файл
	
	//создадим таймер для ежесекндного вычитывания данных из логов
	timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerLogs()));

	//подгоним ширину столбцов под хедеры
	tableView->resizeColumnsToContents();
	tableViewLink->resizeColumnsToContents();
}

void EES::retranslateUi()
{
	setWindowTitle(QApplication::translate("EESClass", "Excel Export Scada", nullptr));
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

		setDataToModel(fileName, name.toInt());
	}
	tableView->resizeColumnsToContents();
	tableViewLink->resizeColumnsToContents();
}

void EES::on_buttonUpLoad_clicked()
{
	QModelIndex index;
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

	int id_subject = 0;// ID созданного тех. объекта
	int evkl_subject = 0;// ID созданной группы событий
	int id_techprog = 0;// ID созданной тех. программы

	try	{
		if (dataBase.connected() &&
			tableData->columnCount() > 1)// && tableData->rowCount() > 1)
		{
			textEditLogs->append(tr("Upload START!", "txt_connect"));
			logs << tr("Upload START!", "txt_connect").toStdString() << std::endl;

			for (size_t i = 0; i < tableData->rowCount(); i++)
			{
				index = tableView->model()->index(i, 0);

				mark = tableData->getObject(index).getMark().toString().toLocal8Bit();
				name = tableData->getObject(index).getName().toString().toLocal8Bit();
				description = tableData->getObject(index).getDescription().toString().toLocal8Bit();
				objectType = tableData->getObject(index).getObjectType().toString().toLocal8Bit();
				digital = tableData->getObject(index).getDigital().toString().toLocal8Bit();//добавлен для цифры
				signature = tableData->getObject(index).getSignature().toString().toLocal8Bit();
				controller = tableData->getObject(index).getController().toString().toLocal8Bit();
				plcAdress = tableData->getObject(index).getPlcAdress().toString().toLocal8Bit();//добавлен для цифры
				plcVarname = tableData->getObject(index).getPlcVarname().toString().toLocal8Bit();//добавлен для цифры
				resource = tableData->getObject(index).getResource().toString().toLocal8Bit();
				eventGroup = tableData->getObject(index).getEventGroup().toString().toLocal8Bit();
				KKS = tableData->getObject(index).getKKS().toString().toLocal8Bit();
				objectTemplate = tableData->getObject(index).getObjectTemplate().toString().toLocal8Bit();
				mnemonicFrameName = tableData->getObject(index).getMnemonicFrameName().toString().toLocal8Bit();
				mnemonicFrameTemplate = tableData->getObject(index).getMnemonicFrameTemplate().toString().toLocal8Bit();
				mnemonicFrameParent = tableData->getObject(index).getMnemonicFrameParent().toString().toLocal8Bit();
				technicalProgramName = tableData->getObject(index).getTechnicalProgramName().toString().toLocal8Bit();
				technicalProgramParent = tableData->getObject(index).getTechnicalProgramParent().toString().toLocal8Bit();

				textEditLogs->append(tr("transfer object ", "txt_connect") + " " + tableData->getObject(index).getMark().toString());
				logs << tr("transfer object: ", "txt_connect").toStdString() << mark << std::endl;

				//начинаем создавать объекты если марка не пустая
				if (mark.size() != 0) {
					evkl_subject = dataBase.create_new_event(eventGroup, mnemonicFrameParent);
					if (digital == "драйвер")
					{
						id_subject = dataBase.create_new_driver(mark, name, description, signature, objectType, controller, KKS, objectTemplate);
					}
					else if (digital == "цифровой")
					{
						id_subject = dataBase.create_new_digital_object(controller, objectType, mark, name, plcAdress,
							plcVarname, resource, eventGroup, description, KKS, signature);
					}
					else
					{
						id_subject = dataBase.create_new_object(controller, resource, objectTemplate, objectType, mark, name, eventGroup, description, KKS, signature);
					}

					logs << tr("Create new object, id = ", "txt_connect").toStdString() << std::to_string(id_subject) << std::endl;

					//				textEditLogs->append(("id new subject: " + std::to_string(id_subject)).c_str());
					dataBase.copy_page_with_object(mnemonicFrameTemplate, mnemonicFrameName, mnemonicFrameParent, std::to_string(id_subject), eventGroup, description);
					id_techprog = dataBase.create_new_technical_programm(technicalProgramName, 300, 200, technicalProgramParent, controller, resource);

					logs << tr("Create new technical programm, id = ", "txt_connect").toStdString() << std::to_string(id_techprog) << std::endl;

					dataBase.object_on_technological_program(technicalProgramName, controller, resource, objectTemplate, objectType, mark);
				}
			}

			textEditLogs->append(tr("Uploading completed!", "txt_connect"));
		}
		else
		{
			textEditLogs->append(tr("Connection failed or data not loaded!", "txt_connect"));
		}
	}
	catch (const std::exception&){}
	
}

void EES::on_buttonConnect_clicked()
{
	std::string tmp = txtPath->toPlainText().toLocal8Bit();
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

	if (dataBase.connected())
		{
			textEditLogs->append(tr("Connection successful!", "txt_connect"));
			//Метод вычитывания пути и записи его в лог
			textEditLogs->append(tr("Database connected: ", "txt_connect")
				+ txtPath->toPlainText());
		}
	else
		{
			textEditLogs->append(tr("Connection failed!", "txt_connect"));
		}


	//std::ofstream logs;
	//logs.open("./logs/logsSQL.txt", std::ios_base::app);//открываем(создаем) файл
	//logs << tmp.c_str() << std::endl;

	//QTextCodec* codec = QTextCodec::codecForName("CP-1251");
	//QMessageBox::warning(this, "Active filter", QString::number(tmp.size()));
//		tabWidget->addTab(tab_tableData, QString());
//		tabWidget->addTab(tab_tableDataLink, QString());
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
//	tabWidget->removeTab(index);//сам виджет жив, можно в любой момент вернуть обратно
}

int EES::initializingConnection(std::string _path)
{
	std::string webserver;

	//Запишем WEB-сервер в строку параметра
	for (size_t i = 0; i < _path.size(); i++)
	{
		if (_path[i] != ':')
		{
			webserver.push_back(_path[i]);
		}
		else
		{
			break;
		}
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

	dataBase.setConnectParam(webserver, _path,
		"SYSDBA", "masterkey");
	dataBase.connect();
	
}

void EES::setDataToModel(QString fileName, int currentTab)
{
	//Получили дир. запуска программы
	//QString tmp_s = QApplication::applicationDirPath();
	// получаем указатель на Excel
	QAxObject *mExcel = new QAxObject("Excel.Application", 0);
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
	QAxObject * usedRange1 = StatSheet->querySubObject("UsedRange");
	QAxObject* columns = usedRange1->querySubObject("Columns");
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

}
