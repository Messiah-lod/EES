#include "EES.h"
#pragma warning(disable : 4715)

EES::EES(QWidget *parent)
	: QWidget(parent)
{
	
	//�������������� �������� ������������ ����������
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
	//���������� ������ �� �������������� ������
	textEditLogs->setReadOnly(true);

	txtPath->setMaximumHeight(25);
	buttonConnect->setMaximumHeight(25);
	//������ ������ ������� � ��������������
	tableView->setModel(tableData);
	tableView->setMinimumHeight(100);
	//�������� �������� �������� ����������
	splitterVertical->addWidget(tableView);
	splitterVertical->addWidget(textEditLogs);
	//������� �� ���� ����������� ��������
	gridLayout->addWidget(txtPath, 0, 0, 1, 4);
	gridLayout->addWidget(buttonConnect, 0, 4, 1, 1);
	gridLayout->addWidget(buttonLoad, 1,0,1,1);
	gridLayout->addWidget(buttonUpLoad, 1, 1, 1, 1);
	gridLayout->addWidget(splitterVertical, 2, 0, 1, 5);
	//����� �������� ������ � ��������
	QObject::connect(buttonLoad, SIGNAL(clicked()), this, SLOT(on_buttonLoad_clicked()));
	QObject::connect(buttonUpLoad, SIGNAL(clicked()), this, SLOT(on_buttonUpLoad_clicked()));
	QObject::connect(buttonConnect, SIGNAL(clicked()), this, SLOT(on_buttonConnect_clicked()));
	//������ ������� �������������� �������
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	retranslateUi();
	//��������� ���� � ���������
	setLayout(gridLayout);

	logs.open("./logs/logsQT.txt", std::ios_base::app);//���������(�������) ����
	
	//�������� ������ ��� ������������ ����������� ������ �� �����
	timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerLogs()));
	//timer->start(1000); // � �������� ������

}

void EES::retranslateUi()
{
	setWindowTitle(QApplication::translate("EESClass", "Excel Export Scada", nullptr));
	buttonLoad->setText(QApplication::translate("buttonLoad", "Open file", nullptr));
	buttonUpLoad->setText(QApplication::translate("buttonUpLoad", "Upload", nullptr));
	buttonConnect->setText(QApplication::translate("buttonConnect", "Connect", nullptr));

	//������� ����������� ������
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

	//�������� ������� ������
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
	std::string digital;//�������� ��� �����
	std::string signature;
	std::string controller;
	std::string plcAdress;//�������� ��� �����
	std::string plcVarname;//�������� ��� �����
	std::string resource;
	std::string eventGroup;
	std::string KKS;
	std::string objectTemplate;
	std::string mnemonicFrameName;
	std::string mnemonicFrameTemplate;
	std::string mnemonicFrameParent;
	std::string technicalProgramName;
	std::string technicalProgramParent;

	int id_subject = 0;// ID ���������� ���. �������
	int evkl_subject = 0;// ID ��������� ������ �������
	int id_techprog = 0;// ID ��������� ���. ���������

	std::vector <bool> isTechProg;//�������� ��������� ���. ��������� �� ��������

	try
	{
		if (dataBase.connected() &&
			tableData->columnCount() > 1)// && tableData->rowCount() > 1)
		{
			textEditLogs->append(tr("Upload START!", "txt_connect"));
			logs << tr("Upload START!", "txt_connect").toStdString() << std::endl;

			for (size_t j = 0; j < tableData->rowCount(); j++)//�������� ��������� ���. ��������� �� ��������
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
				digital = tableData->getObject(index).getDigital().toString().toLocal8Bit();//�������� ��� �����
				signature = tableData->getObject(index).getSignature().toString().toLocal8Bit();
				controller = tableData->getObject(index).getController().toString().toLocal8Bit();
				plcAdress = tableData->getObject(index).getPlcAdress().toString().toLocal8Bit();//�������� ��� �����
				plcVarname = tableData->getObject(index).getPlcVarname().toString().toLocal8Bit();//�������� ��� �����
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

				//�������� ��������� �������
				evkl_subject = dataBase.create_new_event(eventGroup, mnemonicFrameParent);
				if (digital == "�������")
				{
					id_subject = dataBase.create_new_driver(mark, name, description, signature, objectType, controller, KKS, objectTemplate);
				}
				else if (digital == "��������")
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

				//	�������� ������� �������� ��������� ���. ��������� � ������ ����� ������, ���� ��, 
				//	�� ������� ����� �������� �� �����, ���� ���, �� �� ������� ������� �� ���������
				//if (id_techprog != 4 && id_techprog != 1)//���. ��������� ���� ��� ������� �������
				//{

				//����� �������� - ���� ������ �� ���.��������� ��� ���.
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

	//��������� ��� �������� ����������� ���� ���������� ��� ������ �����,
	//���� ������������ ��� �� ���� ����
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
			//����� ����������� ���� � ������ ��� � ���
			textEditLogs->append(tr("Database connected: ", "txt_connect")
				+ txtPath->toPlainText());
		}
		else
		{
			textEditLogs->append(tr("Connection failed!", "txt_connect"));
		}


	//std::ofstream logs;
	//logs.open("./logs/logsSQL.txt", std::ios_base::app);//���������(�������) ����
	//logs << tmp.c_str() << std::endl;

	//QTextCodec* codec = QTextCodec::codecForName("CP-1251");
	//QMessageBox::warning(this, "Active filter", QString::number(tmp.size()));


}

void EES::slotTimerLogs()
{
	//����������� ����� ���������� ������ ����� ����� �� ����
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

	//������� WEB-������ � ������ ���������
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
	//������� web-������ �� ��������� "����"
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
	//�������� ���. ������� ���������
	//QString tmp_s = QApplication::applicationDirPath();
	// �������� ��������� �� Excel
	QAxObject *mExcel = new QAxObject("Excel.Application", 0);
	// �� �����
	QAxObject *workbooks = mExcel->querySubObject("Workbooks");
	// �� ����������, ������ ������� ����
	QAxObject *workbook = workbooks->querySubObject("Open(const QString&)", fileName);
	// �� �����
	QAxObject *mSheets = workbook->querySubObject("Sheets");
	// ���������, ����� ���� �������
	QAxObject *StatSheet = mSheets->querySubObject("Item(int)", 1);

	//���-�� ������ � �����
	//int countSheet = mSheets->property("Count").toInt();
	//���������� ������������ �� ����� ����� ����� �������� � ������� ������ ����
	QAxObject* usedRange = StatSheet->querySubObject("UsedRange");
	QAxObject* rows = usedRange->querySubObject("Rows");
	int countRows = rows->property("Count").toInt() - 1;//�������� 1, �.�.��������� �� ���������

	//��������������, ��������:
	QAxObject * usedRange1 = StatSheet->querySubObject("UsedRange");
	QAxObject* columns = usedRange1->querySubObject("Columns");
//	int countCols = columns->property("Count").toInt();
	int countCols = tableData->columnCount();

	//���� ���������� ����� � �������� ����� ��������� ������� ���������� �� ���������.�������� ��� ��� :
//	tableData->setColumnCount(countCols);
//	tableData->setRowCount(countRows);

	ObjectProjectData rowData;

	for (int nRow = 0; nRow < countRows; ++nRow)
	{
		for (int nCol = 1; nCol <= countCols; ++nCol)
		{
			QAxObject* cell = StatSheet->querySubObject("Cells(int,int)", nRow + 2, nCol+1);
			//����� ������ � ��������� ������
//			cell->setProperty("Value", QVariant("Some value"));
			//����� ������
			QVariant value = cell->property("Value");
			
			//�������� ������ ������
			rowData.setData(nCol-1, value);
			//������� ���������
		}
		//������ � ������
		tableData->addObject(rowData);
	}

	/*

	

	//	QTableView treeView1;
	//	treeView1.setModel(modelEvent);
		//������ ������� �������������� �������
	//	treeView1.setEditTriggers(QAbstractItemView::NoEditTriggers);
	//	treeView1.show();

		// ��������� ��������� �� ������ [row][col] ((!)��������� � �������)
	//	QAxObject* cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 1);
		// ������� �������� ���������� data (����� ���, ���������� � QVariant) � ���������� ������
	//	cell->setProperty("Value", QVariant("Some value"));
	//	mExcel->setProperty("DisplayAlerts", 0);

		//������� excel ��� ������ ��������� ��������� �������� �� ����
	//	mExcel->setProperty("DisplayAlerts", 0);
		//��������� ��� ���� ��� ��������������� ������ � ������� � ����������, ���� ����� ��    //��������� � ������ �����
	//	workbook->dynamicCall("SaveAs (const QString&)", tmp_s);

	//	mExcel->setProperty("DisplayAlerts", 1);

	*/

	//�����-�� ����, ��������� ����� �� ����������������, ������ �� ����
	workbook->dynamicCall("Close(Boolean)", false);
	//��������� ����������
	mExcel->dynamicCall("Quit(void)");

	delete(mExcel);

}

//void EES::create_page(SQL_to_FB &DB_conn, std::string page_name, std::string parent_page, std::string template_page, std::string event_group, int id_subject)
//{
//	DB_conn.create_new_page(page_name, 660, 600, parent_page, template_page, "120:120:120", "window");
//	DB_conn.create_new_obj_onpage("", 131, 90, 120, 9, page_name, "VKLN_G", "����_DC", "0:92:0", "0:92:0", 1, id_subject);
//	DB_conn.create_new_obj_onpage("", 112, 90, 0, 0, page_name, "VKLN_G", "�����������+�� 6��_�������", "0:92:0", "0:92:0", 1, id_subject);
//	DB_conn.create_new_obj_onpage("", 40, 139, 0, 0, page_name, "VKLN_G", "������� �����������", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 75, 203, 100, 41, page_name, "VKLN_G", "�������_VKL", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 10, 289, 0, 0, page_name, "VKLN_G", "������� �����������", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 41, 357, 80, 41, page_name, "VKLN_G", "�������_ZN", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 120, 470, 0, 0, page_name, "VKLN_G", "������������_VKL", "120:120:120", "120:120:120", 0, id_subject);
//	DB_conn.create_new_obj_onpage("", 115, 60, 0, 0, page_name, "RAD_G", "��� ��������+��.���", "120:120:120", "120:120:120", 0);
//	DB_conn.create_new_obj_onpage("", 390, 90, 0, 0, page_name, "EKRA_103", "����", "120:120:120", "120:120:120", 0);
//
////	DB_conn.create_new_prim_onpage("", "text", "[������ 1][12][Arial][255:255:255]", 187, 101, 75, 16, page_name, 0, "120:120:120", "120:120:120", 1);
//	DB_conn.create_new_prim_onpage("event", "text", "[�������][10][Arial][0:0:0]", 410, 600, 117, 29, page_name, 1, "192:192:192", "0:0:0", 0);
//	DB_conn.create_new_prim_onpage("", "image", "������ ������� �������", 535, 600, 31, 32, page_name, 1, "192:192:192", "0:0:0", 2);
//	DB_conn.create_new_prim_onpage("butevent", "button", "[ ][12][Arial][255:255:255]", 529, 595, 43, 38, page_name, 0, "120:120:120", "120:120:120", 2);
//
//	//DB_conn.create_new_event(page_name, parent_page);
//
//	DB_conn.create_new_anim_onpage("[foncolor][event][����_2][1]", "0:128:255", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[foncolor][event][����][1]", "255:255:0", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[foncolor][event][����][1]", "255:0:0", page_name, "event", event_group);
//
//	DB_conn.create_new_anim_onpage("[flashfoncolor][event][����_2][1]", "0;128:128:128", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[flashfoncolor][event][����][1]", "0;128:128:128", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[flashfoncolor][event][����][1]", "0;128:128:128", page_name, "event", event_group);
//
//	DB_conn.create_new_anim_onpage("[textcolor][event][����_2][1]", "0:0:0", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[textcolor][event][����][1]", "0:0:0", page_name, "event", event_group);
//	DB_conn.create_new_anim_onpage("[textcolor][event][����][1]", "0:0:0", page_name, "event", event_group);
//
//	DB_conn.create_new_rec_onpage("event", page_name, "butevent", event_group, "[5][1][0][0]");
//}
