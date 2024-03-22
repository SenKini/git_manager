#include "persistence.h"

Persistence::Persistence()
	: _CONFIG_PATH("config.ini") {}

ErrCode Persistence::saveInfoToLocalFile(QStringList _localRepos, QString _token) {
	QFile saveFile(_CONFIG_PATH);
	ErrCode res = OK;

	if (saveFile.open(QFile::WriteOnly)) {
		QDataStream dataStream(&saveFile);
		dataStream << _token << _localRepos;  // 写入 token 和 本地仓库
	}
	else
		res = FILE_ERROR;
	saveFile.close();

	return res;
}

ErrCode Persistence::updateInfoByLocalFile(QStringList &_localRepos, QString &_token) {
	QFile saveFile(_CONFIG_PATH);
	ErrCode res = OK;

	if (saveFile.open(QFile::ReadOnly)) {
		QDataStream dataStream(&saveFile);
		dataStream >> _token >> _localRepos;  // 读取 token 和 本地仓库
	}
	else
		res = FILE_ERROR;
	saveFile.close();

	return res;
}