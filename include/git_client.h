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
#include "error_mess.h"
#include "logger.h"

class GitClient : public QObject {
    Q_OBJECT

protected:
    QString _token;            // 用户 token
    QJsonDocument _baseInfo;   // 用户基础信息
    QJsonDocument _reposInfo;  // 远程仓库信息
    QByteArray _headImage;     // 用户头像信息

    virtual void initRequest(QNetworkRequest &request) = 0;  // 初始化请求
    virtual QString getInfoUrl() = 0;              // 获取基本信息的请求 URL
    virtual QString getRepoUrl() = 0;              // 获取仓库信息的请求 URL

public:
    GitClient(const QString t);
    bool init();                       // 初始化
    bool updateBaseInfo();             // 更新基础信息
    bool updateRepoInfo();             // 更新仓库信息
    QStringList getRemoteReposName();  // 获取远程仓库名
    QString getLoginName();            // 获取登录名
    int getFollowerNum();              // 获取粉丝数
    QByteArray getHeadImage();         // 获取头像数据

Q_SIGNALS:
    void hadBaseInfo();  // 已经获取了基础信息
    void hadRepoInfo();  // 已经获取了仓库信息
};