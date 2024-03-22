#pragma once
#include <QString>
#include <QProcess>
#include "module.h"

class Git {
private:
	QString _curBranch;	 // 当前分支
	QString _curRemote;	 // 当前远程仓库
	QProcess _process;	 // system 调用

	QString getCurText();				 // 获取列表中当前分支、仓库
	bool pushCommand(QStringList args);	 // 向控制台发送指令
	bool updateCurBranch();				 // 更新当前分支
	bool updateCurRemote();				 // 更新当前仓库

public:
	Git(QString proc);
	bool addAndCommit(QString mess);	// 添加并提交
	bool push();						// 上传
	bool pull();						// 拉取
	bool clone(QString url);			// 克隆
	QString getBranchAll();				// 获取所有分支
	bool setCurBranch(QString branch);	// 设置当前分支
};
