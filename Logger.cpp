#include "Logger.h"



Logger::Logger(const char* nameLogFile)
{
	CreateDirectoryW(L"logs", NULL);
	fileName = nameLogFile;
}

Logger Logger::Warning()
{
	logs.open("./logs/" + fileName, std::ios_base::app);//���������(�������) ���� � ������� � �����
	if (logs.is_open()) {
		logs << "************  WARNING!!!  ***************" << std::endl;
		logs.close();
	}
	return Logger();
}

Logger Logger::Error()
{
	logs.open("./logs/" + fileName, std::ios_base::app);//���������(�������) ���� � ������� � �����
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
    logs.open("./logs/"+ fileName, std::ios_base::app);//���������(�������) ���� � ������� � �����
    if(!logs.is_open()) return false;
    logs << timeToLogs() << message << std::endl;
    logs.close();
    return true;//������ ��� ��� �������� �������� ����� � ������ ����
}


std::string Logger::timeToLogs()
{
	time(&rawtime); // ������� ���� � ��������
	timeinfo = localtime(&rawtime);// ������� ��������� �����, �������������� � ���������
	strftime(buffer, sizeof(buffer), "%d.%m.%Y %X", timeinfo); // ����������� ������ �������
	return "(" + std::string(buffer) + "." + std::to_string(GetTickCount() % 1000) + ")	";
}

