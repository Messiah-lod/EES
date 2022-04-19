#pragma once

#include <fstream>
#include <string>
#include "Windows.h"
#include <iostream>
#include <ctime> //���� ��� ������ �������� ������� � ���


class Logger
{
public:
	
	Logger(const char* nameLogFile);
	~Logger();
	
    bool operator<<(const std::string message);


private:

	Logger Warning();//������������ ��������, �� ������������
	Logger Error();//������������ ��������, �� ������������


	Logger() {};
	std::string timeToLogs();
	std::string fileName;
	std::string m_msg;
	std::ofstream logs;
	//����� ���������� ��� ������ ���� � ���
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80]; // ������, � ������� ����� ��������� ������� �����
};
