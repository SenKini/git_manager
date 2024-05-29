#include "logger.h"

Logger *Logger::_instance = new Logger;

Logger::Logger()
    : _terminal(true), _filePath("log.txt") {}

void Logger::setTarget(bool target, std::string filePath) {
    _terminal = target;
    filePath = _terminal ? "" : filePath;
}

void Logger::addLog(std::string type, std::string info) {
    if (_terminal)
        std::cout << "[" << type << "] " << info << std::endl;
    else {
        std::ofstream ofs;
        std::string buf = "[" + type + "] " + info;
        ofs.open(_filePath, std::ios::binary|std::ios::app);
        ofs << buf << '\n';
    }
}

Logger *logger = Logger::getInstance();
