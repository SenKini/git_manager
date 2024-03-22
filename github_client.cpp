#include "github_client.h"

void GitHubClient::initRequest(QNetworkRequest &request) {
	request.setRawHeader("Accept", "application/vnd.github+json");
	request.setRawHeader("X-GitHub-Api-Version", "2022-11-28");
	request.setRawHeader("User-Agent", "Qt/6.6");
	request.setRawHeader("Authorization", "Bearer " + _token.toUtf8());
}

bool GitHubClient::updateBaseInfo() {
	bool res = false;

	QNetworkRequest request(QUrl("https://api.github.com/user"));  // 基础信息请求
	initRequest(request);
	_baseInfoManager.get(request);

	connect(&_baseInfoManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
		if (reply->error() == QNetworkReply::NoError) {
			QJsonParseError jsonErr;

			_baseInfo = QJsonDocument::fromJson(reply->readAll(), &jsonErr);  // 将接收的报文转换为 json
			if (jsonErr.error == QJsonParseError::NoError) {
				// 更新头像数据
				QNetworkRequest request(QUrl(_baseInfo["avatar_url"].toString()));
				_headImageManager.get(request);

				connect(&_headImageManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
					_headImage = reply->readAll();
					if (!_headImage.isEmpty()) {
						res = true;
						emit hadBaseInfo();	 // 成功接收，发送完成信号
					}
					else
						errorMess = reply->errorString();
				});
			}
			else
				errorMess = jsonErr.errorString();
		}
		else
			errorMess = reply->errorString();
	});	 // 当收到相应报文时执行

	return res;
}

bool GitHubClient::updateRepoInfo() {
	bool res = false;

	QNetworkRequest request(QUrl("https://api.github.com/user/repos"));	 // 仓库信息请求
	initRequest(request);
	_repoInfoManager.get(request);

	connect(&_repoInfoManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
		if (reply->error() == QNetworkReply::NoError) {
			QJsonParseError jsonErr;

			_reposInfo = QJsonDocument::fromJson(reply->readAll(), &jsonErr);
			if (jsonErr.error == QJsonParseError::NoError) {
				res = true;
				emit hadRepoInfo();	 // 成功接收，发送完成信号
			}
			else
				errorMess = jsonErr.errorString();
		}
		else
			errorMess = reply->errorString();
	});	 // 当收到相应报文时执行

	return res;
}

GitHubClient::GitHubClient(const QString token)
	: _token(token) {
}

bool GitHubClient::init() {
	return updateBaseInfo() && updateRepoInfo();  // 获取基本信息
}

QStringList GitHubClient::getRemoteReposName() {
	QJsonArray reposArray(_reposInfo.array());	// 仓库信息数组
	QStringList res;							// 结果

	// 遍历仓库数组，提取所有仓库的名字
	for (auto repo : reposArray) {
		QJsonObject jsonObj = repo.toObject();

		res.push_back(jsonObj["name"].toString());
	}

	return res;
}

QString GitHubClient::getLoginName() {
	return _baseInfo["login"].toString();
}

int GitHubClient::getFollowerNum() {
	return _baseInfo["followers"].toInt();
}

QByteArray GitHubClient::getHeadImage() {
	return _headImage;
}
