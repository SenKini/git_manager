#include "git_client.h"

extern Logger *logger;

GitClient::GitClient(const QString t)
    : _token(t) {}

bool GitClient::init() {
    return updateBaseInfo() & updateRepoInfo();  // ��ȡ������Ϣ
}

bool GitClient::updateBaseInfo() {
    logger->addLog("info", "update local repos in GitClient");
    auto baseInfoManager = new QNetworkAccessManager;
    QUrl infoUrl(getInfoUrl());
    QNetworkRequest request(infoUrl);  // ������Ϣ����
    initRequest(request);
    baseInfoManager->get(request);

    connect(baseInfoManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        logger->addLog("info", "Received message for base info");
        if (reply->error() == QNetworkReply::NoError) {
            QJsonParseError jsonErr;

            _baseInfo = QJsonDocument::fromJson(reply->readAll(), &jsonErr);  // �����յı���ת��Ϊ json
            if (jsonErr.error == QJsonParseError::NoError) {
                // ����ͷ������
                auto headImageManager = new QNetworkAccessManager;
                QNetworkRequest request(QUrl(_baseInfo["avatar_url"].toString()));
                headImageManager->get(request);

                connect(headImageManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
                    logger->addLog("info", "Received message for head image");
                    _headImage = reply->readAll();
                    if (!_headImage.isEmpty()) {
                        emit hadBaseInfo();  // �ɹ����գ���������ź�
                        return true;
                    }
                    else
                        errorMess = reply->errorString();
                });  // ����ͷ������
            }
            else
                errorMess = jsonErr.errorString();
        }
        else
            errorMess = reply->errorString();
    });  // ���ջ�����Ϣ

    return false;
}

bool GitClient::updateRepoInfo() {
    logger->addLog("info", "update repos info in GitClient");
    auto repoInfoManager = new QNetworkAccessManager;
    QUrl repoUrl(getRepoUrl());
    QNetworkRequest request(repoUrl);  // �ֿ���Ϣ����
    initRequest(request);
    repoInfoManager->get(request);

    connect(repoInfoManager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        logger->addLog("info", "Received message for repos info");
        if (reply->error() == QNetworkReply::NoError) {
            QJsonParseError jsonErr;

            _reposInfo = QJsonDocument::fromJson(reply->readAll(), &jsonErr);
            if (jsonErr.error == QJsonParseError::NoError) {
                emit hadRepoInfo();  // �ɹ����գ���������ź�
                return true;
            }
            else
                errorMess = jsonErr.errorString();
        }
        else
            errorMess = reply->errorString();
    });  // ���յ���Ӧ����ʱִ��

    return false;
}

QStringList GitClient::getRemoteReposName() {
    QJsonArray reposArray(_reposInfo.array());  // �ֿ���Ϣ����
    QJsonObject objBuf;                         // json object ������
    QStringList res;                            // ���

    // �����ֿ����飬��ȡ���вֿ������
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
