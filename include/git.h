#pragma once
#include <qstring.h>
#include <qprocess.h>
#include "error_mess.h"

class Git {
private:
    QString _curBranch;  // 当前分支
    QString _curRemote;  // 当前远程仓库
    QProcess _process;   // 系统控制台调用

    QString getCurText();                   // 获取 git 指令返回的列表中的信息
    bool generalCommand(QStringList args);  // 一般的指令模板
    bool updateCurBranch();                 // 更新当前分支
    bool updateCurRemote();                 // 更新当前仓库

public:
    Git(QString procDir);
    bool addAndCommit(QString mess);    // 添加并提交
    bool push();                        // 推送
    bool pull();                        // 拉取
    bool clone(QString url);            // 克隆
    QString getBranchAll();             // 获取所有分支
    bool setCurBranch(QString branch);  // 设置当前分支
};
