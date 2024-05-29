#include "git.h"

QString Git::getCurText() {
    QByteArray outData = _process.readAllStandardOutput();
    QStringList branches = QString(outData).split('\n');
    for (auto branch : branches)
        if (branch[0] == '*')
            return branch.mid(2);

    return QString();
}

bool Git::generalCommand(QStringList args) {
    _process.start("git", args);
    if (_process.waitForFinished() && (_process.exitCode() == 0))
        return true;
    else {
        errorMess = _process.errorString();
        errorType = GIT_ERROR;
        return false;
    }
}

bool Git::updateCurBranch() {
    QStringList args;
    args << "branch";

    if (generalCommand(args)) {
        _curBranch = getCurText();
        return true;
    }
    else
        return false;
}

bool Git::updateCurRemote() {
    QStringList args;

    args << "remote";

    if (generalCommand(args)) {
        _curBranch = getCurText();
        return true;
    }
    else
        return false;
}

Git::Git(QString procDir) {
    _process.setWorkingDirectory(procDir);
}

bool Git::addAndCommit(QString mess) {
    QStringList args;
    args << "add"
         << ".";

    if (generalCommand(args)) {
        args.clear();
        args << "commit"
             << "-m" << mess;

        return generalCommand(args);
    }
    else
        return false;
}

bool Git::push() {
    if (updateCurBranch()) {
        QStringList args;
        args << "push" << _curRemote << _curBranch;

        return generalCommand(args);
    }
    else
        return false;
}

bool Git::pull() {
    if (updateCurBranch()) {
        QStringList args;

        args << "pull" << _curRemote << _curBranch;

        _process.start("git", args);
        if (_process.waitForStarted() && (_process.exitCode() == 0))
            return true;
        else
            errorMess = _process.errorString();
    }

    return false;
}

bool Git::clone(QString url) {
    QStringList args;
    args << "clone" << url;

    return generalCommand(args);
}

QString Git::getBranchAll() {
    QStringList args;
    args << "branch";

    return generalCommand(args) ? _process.readAllStandardOutput() : QString();
}

bool Git::setCurBranch(QString branch) {
    QStringList args;
    args << "checkout"
         << "-b" << branch;

    return generalCommand(args) ? updateCurBranch() : false;
}
