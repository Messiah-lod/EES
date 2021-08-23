#pragma once

#include "Logger.h"
#include ".\ibpp\ibpp.h"
#include <exception>
#include "Windows.h"
#include <iostream>
#include <any>
#include <ctime> //либа для вывода текущего времени в лог


class BaseSQL
{
public:
	//методы, позволяющие получить простой доступ к БД (чтение/запись)
	BaseSQL();
	BaseSQL(std::string webserver_, std::string path_, std::string user_, std::string pass_);
	void setConnectParam(std::string webserver_, std::string path_, std::string user_, std::string pass_);
	~BaseSQL();
	bool connected();
	void disconnect(); //отключение от БД

protected:
	//методы, предназначенные для дочерних классов
	Logger logs{ "logsSQL" };
	std::string query; //общая переменная для записи запросов
	
	
	void write(std::string query);
	void read(std::string query, std::string table, std::string coll_answer, std::any &answ);
	void read(std::string query, std::string table, std::string coll_answer, std::vector <std::any> &answ);

	std::string any_to_str(std::any parameter);//сервисный метод конвертации
	void IntToBinary(int num, std::string  &buf);//сервисный метод конвертации

private:
	//локальные переменные, необходимые для работы класса
	std::string webserver;
	std::string path;
	std::string user;
	std::string pass;

	IBPP::Database Database;//объект БД

	int connect();//подключение к БД
	
	std::string get_type_name(std::string table, std::string coll);//метод, возвращающий тип значения из таблицы БД
	

};
