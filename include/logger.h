#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Logger {
private:
    static Logger *_instance;
    bool _terminal;         // 0 表示存储到文件，1 表示输出到控制台
    std::string _filePath;  // 文件路径，如果输出到控制台则为空
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
