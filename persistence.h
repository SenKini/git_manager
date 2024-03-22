#pragma once
#include <QFile>
#include <QString>
#include "module.h"

class Persistence {
private:
	const QString _CONFIG_PATH;	 // �־û�·��

public:
	Persistence();
	ErrCode updateInfoByLocalFile(QStringList &_localRepos, QString &_token);  // �ӱ����ļ��и�����Ϣ
	ErrCode saveInfoToLocalFile(QStringList _localRepos, QString _token);	   // ������Ϣ�������ļ�
};
