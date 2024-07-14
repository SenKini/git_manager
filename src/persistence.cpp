#include "persistence.h"

Persistence *Persistence::_instance = new Persistence;

Persistence *Persistence::getInstance() {
    return _instance;
}

bool Persistence::readInfoByLocalFile(QStringList &localRepos, QStringList &tokens) {
    QFile saveFile(_SAVE_PATH);

    if (saveFile.open(QFile::ReadOnly)) {
        QString buf = "";

        while (true) {
            buf = saveFile.readLine();
            if (buf != "\n")
                localRepos.push_back(buf);
            else
                break;
        }
        while (true) {
            buf = saveFile.readLine();
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
            saveFile.write(repo.toUtf8() + "\n");
        dataStream << "\n";
        for (auto token : tokens)
            saveFile.write(token.toUtf8() + "\n");
        dataStream << "\n";

        return true;
    }
    else {
        errorMess = "save user info failed.";
        errorType = PERSISTENT_ERROR;

        return false;
    }
}