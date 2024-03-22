#pragma once
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QByteArray>
#include "module.h"

class GitHubClient : public QObject {
	Q_OBJECT

private:
	QString _token;							  // 用户 token
	QJsonDocument _baseInfo;				  // 用户基础信息
	QJsonDocument _reposInfo;				  // 远程仓库信息
	QByteArray _headImage;					  // 用户头像信息
	QNetworkAccessManager _baseInfoManager;	  // 发送基础信息请求
	QNetworkAccessManager _repoInfoManager;	  // 发送仓库信息请求
	QNetworkAccessManager _headImageManager;  // 发送头像数据请求

	void initRequest(QNetworkRequest &request);	 // 初始化请求

public:
	GitHubClient(const QString token);
	bool init();					   // 初始化
	bool updateBaseInfo();			   // 更新基础信息
	bool updateRepoInfo();			   // 更新仓库信息
	QStringList getRemoteReposName();  // 获取远程仓库名
	QString getLoginName();			   // 获取登录名
	int getFollowerNum();			   // 获取粉丝数
	QByteArray getHeadImage();		   // 获取头像数据

Q_SIGNALS:
	void hadBaseInfo();	 // 已经获取了基础信息
	void hadRepoInfo();	 // 已经获取了仓库信息
};
