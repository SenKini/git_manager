#pragma once
#include <qstring.h>
#include <qfile.h>
#include "error_mess.h"

class Persistence {
private:
    Persistence() = default;
    ~Persistence() = default;
    static Persistence *_instance;

    const QString _SAVE_PATH = "save.ini";

public:
    Persistence(const Persistence &) = delete;
    Persistence &operator=(const Persistence &) = delete;
    static Persistence *getInstance();

    bool readInfoByLocalFile(QStringList &localRepos, QStringList &tokens);  // 从本地文件中读取信息
    bool saveInfoToLocalFile(QStringList localRepos, QStringList tokens);    // 保存信息到本地文件
};
