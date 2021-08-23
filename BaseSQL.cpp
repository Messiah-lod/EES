#include "BaseSQL.h"

BaseSQL::BaseSQL()
{
	webserver = "";
	path = "";
	user = "";
	pass = "";
}

BaseSQL::BaseSQL(std::string webserver_, std::string path_, std::string user_, std::string pass_)
{
	webserver = webserver_;
	path = path_;
	user = user_;
	pass = pass_;

	connect();
}


BaseSQL::~BaseSQL()
{
	disconnect();
}

int BaseSQL::connect()
{//После получения данных о базе и пользователе создаем подключение

	Database = IBPP::DatabaseFactory(webserver, path, user, pass);
	logs << "Попытка подключения к базе данных";
	logs << webserver + path;
	try
	{
		Database->Connect();
	}
	catch (std::exception& exp)
	{
		logs << exp.what();
		logs << "Заданы неверные параметры подключения к базе данных!";
		return 1;
	}

	bool isSucessConn = Database->Connected();
	if (isSucessConn) return 0;
	else return 2;

}

void BaseSQL::disconnect()
{
	Database->Disconnect();
}

bool BaseSQL::connected()
{
	return (Database->Connected());
}

void BaseSQL::setConnectParam(std::string webserver_, std::string path_, std::string user_, std::string pass_)
{
	webserver = webserver_;
	path = path_;
	user = user_;
	pass = pass_;
}

std::string BaseSQL::any_to_str(std::any parameter)
{
	try {
		if (parameter.type().hash_code() == 3440116983)
		{
			return std::to_string(std::any_cast <int> (parameter));
		}
		else if (parameter.type().hash_code() == 3406561745)
		{
			return std::to_string(std::any_cast <double> (parameter));
		}
		else if (parameter.type().hash_code() == 4027145609)
		{
			return std::any_cast <const char*> (parameter);
		}
		else if (parameter.type().hash_code() == 1628147040)
		{
			return std::to_string(std::any_cast <bool> (parameter));
		}
		else if (parameter.type().hash_code() == 1604120214)
		{
			return std::any_cast<std::string> (parameter);
		}
		else if (parameter.type().hash_code() == 3708558887)
		{
			logs << "Попытка обработки пустого значения ANY и преобразования в строку!";
			return std::string();
		}
		else
		{
			logs << "Исключение в методе any_to_str:";
			logs << "Попытка вывода неизвестного типа переменной!";
			logs << "hash code параметра:" + parameter.type().hash_code();
			return std::string();
		}
	}
	catch (const std::bad_any_cast & e) {
		logs << "Исключение в методе any_to_str:";
		logs << e.what();
		return std::string();
	}
}

void BaseSQL::IntToBinary(int num, std::string  &buf)
{
	do
	{
		if (num % 2 == 0) buf += '0';
		else buf += '1';
		num /= 2;
	} while (num > 0);

	while (buf.size() < 8)
	{
		buf += '0';
	}

	std::reverse(buf.begin(), buf.end());
}

void BaseSQL::write(std::string query)
{
	IBPP::Transaction tr2;
	tr2 = IBPP::TransactionFactory(Database, IBPP::amWrite);//, IBPP::ilConcurrency,
//		IBPP::lrWait, IBPP::TFF(0));

	if (Database->Connected())
	{
		tr2->Start();
		if (tr2->Started())
		{
			//		std::cout << "Транзакция выполнена" << std::endl;
		}

		IBPP::Statement st1 = IBPP::StatementFactory(Database, tr2);

		try
		{
			st1->Execute(query);//отправляем SQL-запрос
//			st1->Execute("COMMIT WORK;");
			st1->Close();
		}
		catch (std::exception& exp)//
		{
			logs << "Исключение в методе write:";
			logs << exp.what();
			//		return 2;
		}
		tr2->Commit();    // Or tr->Rollback();
	}
	else
	{
		logs << "Нет подключения к базе данных!";
	}
}

void BaseSQL::read(std::string query, std::string table, std::string coll_answer, std::any &answ)
{
	std::string type_answ;

	std::string tmp_s; //строка временного хранения считанных данных GET
	int tmp_i; //int временного хранения считанных данных GET

	answ = 0;

	IBPP::Transaction tr;
	tr = IBPP::TransactionFactory(Database, IBPP::amRead);

	if (Database->Connected())
	{
		tr->Start();
		if (tr->Started())
		{//		std::cout << "Транзакция выполнена" << std::endl;
		}
		IBPP::Statement st = IBPP::StatementFactory(Database, tr);

		type_answ = get_type_name(table, coll_answer);
		//logs << type_answ << std::endl;

		try
		{
			st->Execute(query);//отправляем SQL-запрос
			for (int i = 0; i < coll_answer.length(); i++)
			{
				if (coll_answer[i] == '"')
				{
					coll_answer.erase(i, 1);
				}
			}
			while (st->Fetch())//пока получаем данные...
			{
				//записываем в вектор параметр
				if (type_answ == "VARCHAR")
				{
					//	logs << "ЧАР" << std::endl;
					tmp_s.clear();
					st->Get(coll_answer, tmp_s);
					answ = tmp_s;
				}
				else if (type_answ == "INTEGER" || type_answ == "SHORT")
				{
					//	logs << "ИНТ" << std::endl;
					st->Get(coll_answer, tmp_i);
					answ = tmp_i;
				}
				else
				{
					logs << "Исключение в методе read: Параметр не найден!";
				}
			}
			st->Close();
		}
		catch (std::exception& exp)
		{
			logs << "Исключение в методе read:";
			logs << "Запрос вида:" + query;
			logs << exp.what();
		}
		tr->Commit();
	}
}

void BaseSQL::read(std::string query, std::string table, std::string coll_answer, std::vector <std::any> &answ)
{
	std::string type_answ;

	std::string tmp_s; //строка временного хранения считанных данных GET
	int tmp_i; //int временного хранения считанных данных GET

	//answ = 0;

	IBPP::Transaction tr;
	tr = IBPP::TransactionFactory(Database, IBPP::amRead);

	if (Database->Connected())
	{
		tr->Start();
		if (tr->Started())
		{//		std::cout << "Транзакция выполнена" << std::endl;
		}
		IBPP::Statement st = IBPP::StatementFactory(Database, tr);

		type_answ = get_type_name(table, coll_answer);
		//		std::cout << type_answ << std::endl;

		try
		{
			st->Execute(query);//отправляем SQL-запрос
			for (int i = 0; i < coll_answer.length(); i++)
			{
				if (coll_answer[i] == '"')
				{
					coll_answer.erase(i, 1);
				}
			}
			while (st->Fetch())//пока получаем данные...
			{
				//записываем в вектор параметр
				if (type_answ == "VARCHAR")
				{
					//										std::cout << "ЧАР" << std::endl;
					tmp_s.clear();
					st->Get(coll_answer, tmp_s);
					answ.push_back(tmp_s);
				}
				else if (type_answ == "INTEGER" || type_answ == "SHORT")
				{
					//					std::cout << "ИНТ" << std::endl;
					st->Get(coll_answer, tmp_i);
					answ.push_back(tmp_i);
				}
			}
			st->Close();
		}
		catch (std::exception& exp)
		{
			logs << "Исключение в методе read:";
			logs << "Запрос вида:" + query;
			logs << exp.what();
		}
		tr->Commit();
	}
}


std::string BaseSQL::get_type_name(std::string table, std::string coll)
{
	//набор переменных для метода get_type_name
	std::vector <std::string> all_coll_name;
	std::vector <int> all_coll_type;
	std::string all_coll_name_;
	int all_coll_type_ = 0;



	for (int i = 0; i < coll.length(); i++)
	{
		if (coll[i] == '"')
		{
			coll.erase(i, 1);
		}
	}

	all_coll_name.clear();
	all_coll_type.clear();

	std::string  query1, query2;
	query.clear();
	query1 = "select R.RDB$RELATION_NAME,  R.RDB$FIELD_NAME, F.RDB$FIELD_TYPE from RDB$FIELDS F, RDB$RELATION_FIELDS R where F.RDB$FIELD_NAME = R.RDB$FIELD_SOURCE and R.RDB$SYSTEM_FLAG = 0";
	query2 = "order by R.RDB$RELATION_NAME, R.RDB$FIELD_POSITION";
	query = query1 + ' ' + "and RDB$RELATION_NAME = '" + table.c_str() + "' " + query2;

	IBPP::Transaction tr1;
	tr1 = IBPP::TransactionFactory(Database, IBPP::amRead);

	if (Database->Connected())
	{
		tr1->Start();

		if (tr1->Started())
		{
			//					std::cout << "Транзакция выполнена" << std::endl;
		}

		IBPP::Statement st = IBPP::StatementFactory(Database, tr1);

		try
		{
			st->Execute(query);//отправляем SQL-запрос
			while (st->Fetch())//пока получаем данные...
			{
				st->Get("RDB$FIELD_NAME", all_coll_name_);
				//удалим лишние пробелы
				for (int k = 0; k < all_coll_name_.size(); k++) {
					if (all_coll_name_[k] == ' ') {
						all_coll_name_.erase(k, 1);
						k--;
					}
				}
				all_coll_name.push_back(all_coll_name_);

				st->Get("RDB$FIELD_TYPE", all_coll_type_);
				all_coll_type.push_back(all_coll_type_);
			}
			st->Close();
		}
		catch (std::exception& exp)
		{
			logs << exp.what();
		}
		tr1->Commit();
	}
	else
	{
		logs << "Исключение в методе get_type_name:";
		logs << "Отсутствует подключение к базе данных!";
	}

	//logs << coll << std::endl;
	for (size_t i = 0; i < all_coll_name.size(); i++)
	{
		if (all_coll_name[i] == coll)
		{
			if (all_coll_type[i] == 7)
			{
				return "SHORT";
			}
			else if (all_coll_type[i] == 8)
			{
				return "INTEGER";
			}
			else if (all_coll_type[i] == 9)
			{
				return "QUAD";
			}
			else if (all_coll_type[i] == 10)
			{
				return "FLOAT";
			}
			else if (all_coll_type[i] == 12)
			{
				return "DATE";
			}
			else if (all_coll_type[i] == 13)
			{
				return "TIME";
			}
			else if (all_coll_type[i] == 14)
			{
				return "TEXT";
			}
			else if (all_coll_type[i] == 16)
			{
				return "INT64";
			}
			else if (all_coll_type[i] == 23)
			{
				return "BOOLEAN";
			}
			else if (all_coll_type[i] == 27)
			{
				return "DOUBLE";
			}
			else if (all_coll_type[i] == 35)
			{
				return "TIMESTAMP";
			}
			else if (all_coll_type[i] == 37)
			{
				return "VARCHAR";
			}
			else if (all_coll_type[i] == 40)
			{
				return "CSTRING";
			}
			else if (all_coll_type[i] == 45)
			{
				return "BLOB_ID";
			}
			else if (all_coll_type[i] == 261)
			{
				return "BLOB";
			}
			else
			{
				logs << "Исключение в методе get_type_name: Type not defined";
				return "Type not defined";
			}
		}
	}
	return "Таблица или колонка с требуемым именем не найдена!";

}
