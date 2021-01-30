#include "EES.h"
#pragma warning(disable : 4715)

EES::EES(QWidget *parent)
	: QWidget(parent)
{
	
	//инициализируем элементы графического интерфейса
	gridLayout = new QGridLayout;
	splitterVertical = new QSplitter(Qt::Vertical);
	tableData = new ObjectProject;
	tableView = new QTableView;
	buttonLoad = new QPushButton;
	buttonUpLoad = new QPushButton;
	buttonConnect = new QPushButton;
	textEditLogs = new QTextEdit;
	txtPath = new QTextEdit;
//	dataBase = new SQL_to_FB;
	
	textEditLogs->setMinimumHeight(100);
	//Установили запрет на редактирование текста
	textEditLogs->setReadOnly(true);

	txtPath->setMaximumHeight(25);
	buttonConnect->setMaximumHeight(25);
	//свяжем модель таблицы с представлением
	tableView->setModel(tableData);
	tableView->setMinimumHeight(100);
	//разделим основные элементы сплиттером
	splitterVertical->addWidget(tableView);
	splitterVertical->addWidget(textEditLogs);
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
	//тригер запрета редактирования таблицы
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	retranslateUi();
	//отобразим грид с виджетами
	setLayout(gridLayout);

	logs.open("./logs/logsQT.txt", std::ios_base::app);//открываем(создаем) файл
	
	//создадим таймер для ежесекндного вычитывания данных из логов
	timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerLogs()));
	//timer->start(1000); // И запустим таймер

}

void EES::retranslateUi()
{
	setWindowTitle(QApplication::translate("EESClass", "Excel Export Scada", nullptr));
	buttonLoad->setText(QApplication::translate("buttonLoad", "Open file", nullptr));
	buttonUpLoad->setText(QApplication::translate("buttonUpLoad", "Upload", nullptr));
	buttonConnect->setText(QApplication::translate("buttonConnect", "Connect", nullptr));

	//Зададим переводимые хедеры
	tableData->setHeaderData(0, Qt::Horizontal, QObject::tr("Mark"));
	tableData->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
	tableData->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
	tableData->setHeaderData(3, Qt::Horizontal, QObject::tr("Object Type"));
	tableData->setHeaderData(4, Qt::Horizontal, QObject::tr("Signature"));
	tableData->setHeaderData(5, Qt::Horizontal, QObject::tr("Controller"));
	tableData->setHeaderData(6, Qt::Horizontal, QObject::tr("Resource"));
	tableData->setHeaderData(7, Qt::Horizontal, QObject::tr("Group events"));
	tableData->setHeaderData(8, Qt::Horizontal, QObject::tr("KKS"));
	tableData->setHeaderData(9, Qt::Horizontal, QObject::tr("Template object"));
	tableData->setHeaderData(10, Qt::Horizontal, QObject::tr("Mnemonic Frame Name"));
	tableData->setHeaderData(11, Qt::Horizontal, QObject::tr("Mnemonic Frame Template"));
	tableData->setHeaderData(12, Qt::Horizontal, QObject::tr("Mnemonic Frame Parent"));
	tableData->setHeaderData(13, Qt::Horizontal, QObject::tr("Technical Program Name"));
	tableData->setHeaderData(14, Qt::Horizontal, QObject::tr("Technical Program Parent"));
} // retranslateUi

void EES::on_buttonLoad_clicked()
{
//	textEditLogs->append("Clicked buttonLoad");

	//добавить очистку модели
	tableData->delData();

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

		setDataToModel(fileName);
	}

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

	std::vector <bool> isTechProg;//проверка присутвия тех. программы ДО проливки

	try
	{
		if (dataBase.connected() &&
			tableData->columnCount() > 1)// && tableData->rowCount() > 1)
		{
			textEditLogs->append(tr("Upload START!", "txt_connect"));
			logs << tr("Upload START!", "txt_connect").toStdString() << std::endl;

			for (size_t j = 0; j < tableData->rowCount(); j++)//проверка присутвия тех. программы ДО проливки
			{
				index = tableView->model()->index(j, 0);
				technicalProgramName = tableData->getObject(index).getTechnicalProgramName().toString().toLocal8Bit();
				id_techprog = dataBase.presenceObj(technicalProgramName,"programm");
				(id_techprog != 1 && id_techprog == 0) ? isTechProg.push_back(true) : isTechProg.push_back(false);
			}

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

				//начинаем создавать объекты
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

				//	добавить условие проверки созданной тех. программы в данном цикле работы, если да, 
				//	то создаем много объектов на одной, если нет, то не создаем объекты на программе
				//if (id_techprog != 4 && id_techprog != 1)//тех. программа была или создана успешно
				//{

				//нужна проверка - есть объект на тех.программе или нет.
					dataBase.object_on_technological_program(technicalProgramName, controller, resource, objectTemplate, objectType, mark);
				//}
			}

			textEditLogs->append(tr("Uploading completed!", "txt_connect"));
		}
		else
		{
			textEditLogs->append(tr("Connection failed or data not loaded!", "txt_connect"));
		}
	}
	catch (const std::exception&)
	{

	}
	
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

			if (fileName.isEmpty())
			{
				textEditLogs->append(tr("No data base chosen!", "txt_connect"));
			}

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

void EES::setDataToModel(QString fileName)
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
			rowData.setData(nCol-1, value);
			//кончили заполнять
		}
		//запись в модель
		tableData->addObject(rowData);
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

	//зачем-то надо, канонично сразу не закомменитировал, теперь не помн
	workbook->dynamicCall("Close(Boolean)", false);
	//закрываем приложение
	mExcel->dynamicCall("Quit(void)");

	delete(mExcel);

}

//void EES::create_page(SQL_to_FB &DB_conn, std::string page_name, std::string parent_page, std::string template_page, std::string event_group, int id_subject)
//{
//	DB_conn.create_new_page(page_name, 660, 600, parent_page, template_page, "120:120:120", "window");
//	DB_conn.create_new_obj_onpage("", 131, 90, 120, 9, page_name, "VKLN_G", "Шина_DC", "0:92:0", "0:92:0", 1, id_subject);
//	DB_conn.create_new_obj_onpage("", 112, 90, 0, 0, page_name, "VKLN_G", "Выключатель+ЗН 6кВ_стрелка", "0:92:0", "0:92:0", 1, id_subject);
//	DB_conn.create_new_obj_onpage("", 40, 139, 0, 0, page_name, "VKLN_G", "Плакаты выключатель", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 75, 203, 100, 41, page_name, "VKLN_G", "Подпись_VKL", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 10, 289, 0, 0, page_name, "VKLN_G", "Плакаты заземлитель", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 41, 357, 80, 41, page_name, "VKLN_G", "Подпись_ZN", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 120, 470, 0, 0, page_name, "VKLN_G", "Наименование_VKL", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 115, 60, 0, 0, page_name, "RAD_G", "АДм Стандарт+ед.изм", "120:120:120", "120:120:120", 0);
//	DB_conn.create_new_obj_onpage("", 390, 90, 0, 0, page_name, "EKRA_103", "ЭКРА", "120:120:120", "120:120:120", 0);
//
////	DB_conn.create_new_prim_onpage("", "text", "[Ячейка 1][12][Arial][255:255:255]", 187, 101, 75, 16, page_name, 0, "120:120:120", "120:120:120", 1);
//	DB_conn.create_new_prim_onpage("event", "text", "[СОБЫТИЯ][10][Arial][0:0:0]", 410, 600, 117, 29, page_name, 1, "192:192:192", "0:0:0", 0);
//	DB_conn.create_new_prim_onpage("", "image", "Иконка История событий", 535, 600, 31, 32, page_name, 1, "192:192:192", "0:0:0", 2);
//	DB_conn.create_new_prim_onpage("butevent", "button", "[ ][12][Arial][255:255:255]", 529, 595, 43, 38, page_name, 0, "120:120:120", "120:120:120", 2);
//
//	//DB_conn.create_new_event(page_name, parent_page);
//
//	DB_conn.create_new_anim_onpage("[foncolor][event][Пред_2][1]", "0:128:255", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[foncolor][event][пред][1]", "255:255:0", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[foncolor][event][авар][1]", "255:0:0", page_name, "event", event_group);
//
//	DB_conn.create_new_anim_onpage("[flashfoncolor][event][Пред_2][1]", "0;128:128:128", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[flashfoncolor][event][пред][1]", "0;128:128:128", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[flashfoncolor][event][авар][1]", "0;128:128:128", page_name, "event", event_group);
//
//	DB_conn.create_new_anim_onpage("[textcolor][event][Пред_2][1]", "0:0:0", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[textcolor][event][пред][1]", "0:0:0", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[textcolor][event][авар][1]", "0:0:0", page_name, "event", event_group);
//
//	DB_conn.create_new_rec_onpage("event", page_name, "butevent", event_group, "[5][1][0][0]");
//}
