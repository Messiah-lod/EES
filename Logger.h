#pragma once

#include <fstream>
#include <string>
#include "Windows.h"
#include <iostream>
#include <ctime> //либа для вывода текущего времени в лог


class Logger
{
public:
	
	Logger(const char* nameLogFile);
	~Logger();
	
    bool operator<<(const std::string message);


private:

	Logger Warning();//Проглатывает месседжи, НЕ использовать
	Logger Error();//Проглатывает месседжи, НЕ использовать


	Logger() {};
	std::string timeToLogs();
	std::string fileName;
	std::string m_msg;
	std::ofstream logs;
	//Набор переменных для вывода даты в лог
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80]; // строка, в которой будет храниться текущее время
};
