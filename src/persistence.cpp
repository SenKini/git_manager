#include "persistence.h"

Persistence *Persistence::_instance = new Persistence;

Persistence *Persistence::getInstance() {
    return _instance;
}

bool Persistence::readInfoByLocalFile(QStringList &localRepos, QStringList &tokens) {
    QFile saveFile(_SAVE_PATH);

    if (saveFile.open(QFile::ReadOnly)) {
        QDataStream dataStream(&saveFile);
        QString buf = "";

        while (true) {
            dataStream >> buf;
            if (buf != "\n")
                localRepos.push_back(buf);
            else
                break;
        }
        while (true) {
            dataStream >> buf;
            if (buf != "\n")
                tokens.push_back(buf);
            else
                break;
        }

        return true;
    }
    else {
        errorMess = "read user info failed.";
        errorType = PERSISTENT_ERROR;

        return false;
    }
}

bool Persistence::saveInfoToLocalFile(QStringList localRepos, QStringList tokens) {
    QFile saveFile(_SAVE_PATH);

    if (saveFile.open(QFile::WriteOnly)) {
        QDataStream dataStream(&saveFile);

        for (auto repo : localRepos)
            dataStream << repo;
        dataStream << "\n";
        for (auto token : localRepos)
            dataStream << token;
        dataStream << "\n";

        return true;
    }
    else {
        errorMess = "save user info failed.";
        errorType = PERSISTENT_ERROR;

        return false;
    }
}