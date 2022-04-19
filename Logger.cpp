#include "Logger.h"



Logger::Logger(const char* nameLogFile)
{
	CreateDirectoryW(L"logs", NULL);
	fileName = nameLogFile;
}

Logger Logger::Warning()
{
	logs.open("./logs/" + fileName, std::ios_base::app);//открываем(создаем) файл с записью в конец
	if (logs.is_open()) {
		logs << "************  WARNING!!!  ***************" << std::endl;
		logs.close();
	}
	return Logger();
}

Logger Logger::Error()
{
	logs.open("./logs/" + fileName, std::ios_base::app);//открываем(создаем) файл с записью в конец
	if (logs.is_open()) {
		logs << "************  ERROR!!!  ***************" << std::endl;
		logs.close();
	}
	return Logger();
}


Logger::~Logger()
{
}


bool Logger::operator<<(const std::string message)
{
    logs.open("./logs/"+ fileName, std::ios_base::app);//открываем(создаем) файл с записью в конец
    if(!logs.is_open()) return false;
    logs << timeToLogs() << message << std::endl;
    logs.close();
    return true;//вернет тру при успешном открытии файла и записи туда
}


std::string Logger::timeToLogs()
{
	time(&rawtime); // текущая дата в секундах
	timeinfo = localtime(&rawtime);// текущее локальное время, представленное в структуре
	strftime(buffer, sizeof(buffer), "%d.%m.%Y %X", timeinfo); // форматируем строку времени
	return "(" + std::string(buffer) + "." + std::to_string(GetTickCount() % 1000) + ")	";
}

