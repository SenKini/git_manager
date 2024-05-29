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

    bool readInfoByLocalFile(QStringList &localRepos, QStringList &tokens);  // �ӱ����ļ��ж�ȡ��Ϣ
    bool saveInfoToLocalFile(QStringList localRepos, QStringList tokens);    // ������Ϣ�������ļ�
};
