#pragma once

#include "Logger.h"
#include ".\ibpp\ibpp.h"
#include <exception>
#include "Windows.h"
#include <iostream>
#include <any>
#include <ctime> //���� ��� ������ �������� ������� � ���


class BaseSQL
{
public:
	//������, ����������� �������� ������� ������ � �� (������/������)
	BaseSQL();
	BaseSQL(std::string webserver_, std::string path_, std::string user_, std::string pass_);
	void setConnectParam(std::string webserver_, std::string path_, std::string user_, std::string pass_);
	~BaseSQL();
	bool connected();
	void disconnect(); //���������� �� ��

protected:
	//������, ��������������� ��� �������� �������
	Logger logs{ "logsSQL" };
	std::string query; //����� ���������� ��� ������ ��������
	
	
	void write(std::string query);
	void read(std::string query, std::string table, std::string coll_answer, std::any &answ);
	void read(std::string query, std::string table, std::string coll_answer, std::vector <std::any> &answ);

	std::string any_to_str(std::any parameter);//��������� ����� �����������
	void IntToBinary(int num, std::string  &buf);//��������� ����� �����������

private:
	//��������� ����������, ����������� ��� ������ ������
	std::string webserver;
	std::string path;
	std::string user;
	std::string pass;

	IBPP::Database Database;//������ ��

	int connect();//����������� � ��
	
	std::string get_type_name(std::string table, std::string coll);//�����, ������������ ��� �������� �� ������� ��
	

};
