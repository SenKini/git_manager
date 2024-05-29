#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Logger {
private:
    static Logger *_instance;
    bool _terminal;         // 0 ��ʾ�洢���ļ���1 ��ʾ���������̨
    std::string _filePath;  // �ļ�·����������������̨��Ϊ��
    Logger();
    ~Logger() = default;

public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    static Logger *getInstance() {
        return _instance;
    }

    void setTarget(bool target, std::string filePath = "log.txt");
    void addLog(std::string type, std::string info);
};
