#pragma once
#include <QFile>
#include <QString>
#include "module.h"

class Persistence {
private:
	const QString _CONFIG_PATH;	 // 持久化路径

public:
	Persistence();
	ErrCode updateInfoByLocalFile(QStringList &_localRepos, QString &_token);  // 从本地文件中更新信息
	ErrCode saveInfoToLocalFile(QStringList _localRepos, QString _token);	   // 保存信息到本地文件
};
