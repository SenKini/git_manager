#include "git_client.h"

extern Logger *logger;

GitClient::GitClient(const QString t)
    : _token(t) {}

bool GitClient::init() {
    return updateBaseInfo() & updateRepoInfo();  // 获取基本信息
}

bool GitClient::updateBaseInfo() {
    logger->addLog("info", "update local repos in GitClient");
    auto baseInfoManager = new QNetworkAccessManager;
    QUrl infoUrl(getInfoUrl());
    QNetworkRequest request(infoUrl);  // 基础信息请求
    initRequest(request);
    baseInfoManager->get(request);

    connect(baseInfoManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        logger->addLog("info", "Received message for base info");
        if (reply->error() == QNetworkReply::NoError) {
            QJsonParseError jsonErr;

            _baseInfo = QJsonDocument::fromJson(reply->readAll(), &jsonErr);  // 将接收的报文转换为 json
            if (jsonErr.error == QJsonParseError::NoError) {
                // 更新头像数据
                auto headImageManager = new QNetworkAccessManager;
                QNetworkRequest request(QUrl(_baseInfo["avatar_url"].toString()));
                headImageManager->get(request);

                connect(headImageManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
                    logger->addLog("info", "Received message for head image");
                    _headImage = reply->readAll();
                    if (!_headImage.isEmpty()) {
                        emit hadBaseInfo();  // 成功接收，发送完成信号
                        return true;
                    }
                    else
                        errorMess = reply->errorString();
                });  // 接收头像数据
            }
            else
                errorMess = jsonErr.errorString();
        }
        else
            errorMess = reply->errorString();
    });  // 接收基本信息

    return false;
}

bool GitClient::updateRepoInfo() {
    logger->addLog("info", "update repos info in GitClient");
    auto repoInfoManager = new QNetworkAccessManager;
    QUrl repoUrl(getRepoUrl());
    QNetworkRequest request(repoUrl);  // 仓库信息请求
    initRequest(request);
    repoInfoManager->get(request);

    connect(repoInfoManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        logger->addLog("info", "Received message for repos info");
        if (reply->error() == QNetworkReply::NoError) {
            QJsonParseError jsonErr;

            _reposInfo = QJsonDocument::fromJson(reply->readAll(), &jsonErr);
            if (jsonErr.error == QJsonParseError::NoError) {
                emit hadRepoInfo();  // 成功接收，发送完成信号
                return true;
            }
            else
                errorMess = jsonErr.errorString();
        }
        else
            errorMess = reply->errorString();
    });  // 当收到相应报文时执行

    return false;
}

QStringList GitClient::getRemoteReposName() {
    QJsonArray reposArray(_reposInfo.array());  // 仓库信息数组
    QJsonObject objBuf;                         // json object 缓冲区
    QStringList res;                            // 结果

    // 遍历仓库数组，提取所有仓库的名字
    for (auto repo : reposArray) {
        objBuf = repo.toObject();
        res.push_back(objBuf["name"].toString());
    }

    return res;
}

QString GitClient::getLoginName() {
    return _baseInfo["login"].toString();
}

int GitClient::getFollowerNum() {
    return _baseInfo["followers"].toInt();
}

QByteArray GitClient::getHeadImage() {
    return _headImage;
}
