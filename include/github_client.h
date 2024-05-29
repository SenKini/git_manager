#pragma once
#include <QString>
#include "git_client.h"

class GithubClient : public GitClient {
protected:
    virtual void initRequest(QNetworkRequest &request);  // 初始化请求
    virtual QString getInfoUrl();                        // 获取基本信息的请求 URL
    virtual QString getRepoUrl();                        // 获取仓库信息的请求 URL

public:
    GithubClient(const QString t);
};